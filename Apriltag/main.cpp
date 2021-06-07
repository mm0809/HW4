#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D1, D0);
BufferedSerial cam(D10,D9); //tx,rx(D1, D0) or (D10, D9)

BBCar car(pin5, pin6, servo_ticker);

Thread cam_receive_thread;

volatile int angle_state = 0;
volatile int angle = 0;


void recieve_thread(){
   while(1) {
      if(cam.readable()){
         char recv[1];
         cam.read(recv, sizeof(recv));
         if (recv[0] == '#') {
            angle_state = 0;
            angle = 0;
         } else if (recv[0] == '!')
             angle_state = 1;
         else {
            angle *= 10;
            angle += recv[0] - '0';
         }
         // print out the recieved data
         //pc.write(recv, sizeof(recv));
         //printf("\r\n");
      }
   }
}

void ask_for_apriltag_angle(void) 
{
    char s[] = "april\0";
    cam.write(s, sizeof(s));
    printf("send april\r\n");
    ThisThread::sleep_for(500ms);
    if (angle_state)
        printf("%d \r\n", angle);
    else
        printf("get angle fail\r\n");
}

int main() {
   char buf[256], outbuf[256];
   FILE *devin = fdopen(&xbee, "r");
   FILE *devout = fdopen(&xbee, "w");

   int angle_adjust_done = 0;

   cam.set_baud(9600);
   cam_receive_thread.start(recieve_thread);


   while(1) {
       while (angle_adjust_done == 0) {
            char angle_out[5];

            ask_for_apriltag_angle();                   // detact apriltag
            fprintf(devout, "current: %d\n", angle);    // send current angle to xbee

            if (angle <= 355) {

            } else {
                angle_adjust_done = 1;
            }
            fprintf(devout, "done current: %d\n", angle);    // send current angle to xbee
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


