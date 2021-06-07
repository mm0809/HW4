#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D1, D0);
BufferedSerial cam(D10,D9); //tx,rx(D1, D0) or (D10, D9)

BBCar car(pin5, pin6, servo_ticker);

DigitalInOut ping(D11);
Timer t;

int angle_state = 0;
int renew = 0;
int angle = 0;

float get_dis() 
{
    float val;
    ping.output();
    ping = 0; wait_us(200);
    ping = 1; wait_us(5);
    ping = 0; wait_us(5);

    ping.input();
    while(ping.read() == 0);
    t.start();
    while(ping.read() == 1);
    val = t.read();
    printf("Ping = %lf\r\n", val*17700.4f);
    t.stop();
    t.reset();

    return val;
}

int recieve_thread(){
   int cnt = 0;
   while(1) {
      if(cam.readable()){
         char recv[1];
         cam.read(recv, sizeof(recv));
         if (recv[0] == '#') {
            angle_state = 0;
            cnt = 0;
         } else if (recv[0] == '!') {
             angle_state = 1;
             renew = 1;

             return cnt;
         } else if (recv[0] == 'z') {
             cnt = -1;
         } else if (recv[0] >= '0' && recv[0] <= '9') {
            cnt *= 10;
            cnt += recv[0] - '0';
         }
         // print out the recieved data
         //pc.write(recv, sizeof(recv));
         //printf("\r\n");
      }
   }
}

void ask_for_apriltag_angle(void) 
{
    angle_state = 0;
    char s[] = "april\0";
    cam.write(s, sizeof(s));
    //printf("send april\r\n");
    ThisThread::sleep_for(500ms);

    angle = recieve_thread();
}

int main() {
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");

   int angle_adjust_done = 0;

   cam.set_baud(9600);


    fputc('s', devout);
   int start = 0;
   while(1) {
        int flag = 0;
       while (angle_adjust_done == 0) {

            ask_for_apriltag_angle();                   // detact apriltag
            fputc('c', devout);
            fprintf(devout, "current: %d\n", angle);

            if ((angle <= 360 && angle >= 355) || (angle >= 0 && angle <= 5)) {
                float dis = get_dis();
                fprintf(devout, "dis: %lf\n", dis*17700.4f);
                angle_adjust_done = 1;
                break;
            }
            if (renew == 1 && angle <= 355 && angle != -1) {
                start = 1;

                car.servo0.set_speed(-20);
                car.servo1.set_speed(-20);
                ThisThread::sleep_for(100);
                //car.turn(50, 0.7);
                //ThisThread::sleep_for(300); // 500

                //car.stop();
                //ThisThread::sleep_for(200);

                //car.goStraight(100);
                //ThisThread::sleep_for(200);

                //car.stop();
                //ThisThread::sleep_for(200);

                //car.servo0.set_speed(30);
                //car.servo1.set_speed(30);
                //ThisThread::sleep_for(200);

                car.stop();
                ThisThread::sleep_for(200);
                
                renew = 0;
            } else{
                //flag = 1;
                car.servo0.set_speed(20);
                car.servo1.set_speed(20);
                ThisThread::sleep_for(100);

                car.stop();
                ThisThread::sleep_for(200);
            }
       }
   }

   while (1) {
      memset(buf, 0, 256);
      for( int i = 0; ; i++ ) {
         char recv = fgetc(devin);
         if(recv == '\n') {
            printf("\r\n");
            break;
         }
         buf[i] = fputc(recv, devout);
      }
   RPC::call(buf, outbuf);
   }
}


