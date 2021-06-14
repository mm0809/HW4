#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D1, D0);
BufferedSerial cam(D10, D9);


DigitalInOut ping(D11); // Ping
BBCar car(pin5, pin6, servo_ticker);

Timer t;

int state = 0;
// state = 0: line follwing
// state = 1: Apriltage following

void clear_cam_UART_buf(void);
void ask_for_apriltag_angle(void);
char receive_from_cam(void);
float get_dis();

void goF(int speed);
void goL(int speed0, int speed1);
void goR(int speed0, int speed1);




int main() {
    //for xbee UART
    //char buf[256], outbuf[256];
    //FILE *devin = fdopen(&xbee, "r");
    //FILE *devout = fdopen(&xbee, "w");
    //while (1) {
    //   memset(buf, 0, 256);
    //   for( int i = 0; ; i++ ) {
    //      char recv = fgetc(devin);
    //      if(recv == '\n') {
    //         printf("\r\n");
    //         break;
    //      }
    //      buf[i] = fputc(recv, devout);
    //   }
    //RPC::call(buf, outbuf);
    //}
    //while(1) {
    //    ask_for_apriltag_angle();
    //    //char cmd = receive_from_cam();
    //}
    while(1) {
        if (state == 0 && cam.readable()) {
            char recv[1];
            cam.read(recv, sizeof(recv));
            printf("%c\r\n", recv[0]);

            if (recv[0] == 'F') {
                goF(80);
            } else if (recv[0] == 'L') {
                goL(40, -100);
            } else if (recv[0] == 'R') {
                goR(90, -60);
            }

            float distance = get_dis();
            if (state == 0 && distance <= 15) {
                state = 1;
                car.stop();
                clear_cam_UART_buf();
            }
        } else if (state == 1) {
            ask_for_apriltag_angle();
            char cmd = receive_from_cam();
            printf("april: %c\r\n", cmd);

            if (cmd == 'f') {

            } else if (cmd == 'l') {
                
            } else if (cmd == 'r') {

            }
        }
    }
} 
float get_dis() {
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

    return val*17700.4f;
}
void goF(int speed) {
    car.goStraight(speed);// 80
    return;
}
void goL(int speed0, int speed1) {
    car.servo0.set_factor(1);
    car.servo1.set_factor(0.35);
    car.servo0.set_speed(speed0);   //50
    car.servo1.set_speed(speed1);   //-80
}
void goR(int speed0, int speed1) {
    car.servo0.set_factor(1);
    car.servo1.set_factor(0.35);
    car.servo0.set_speed(speed0);   //80
    car.servo1.set_speed(speed1);   //-70
    //car.servo0.set_speed(80);
    //car.servo1.set_speed(-70);
}

void clear_cam_UART_buf(void)
{
    while (cam.readable()) {
        char recv[1];
        cam.read(recv, sizeof(recv));
    }
}

void ask_for_apriltag_angle(void) 
{
    char s[] = "a";
    cam.write(s, sizeof(s));
    while (t != 'K')
        char t = receive_from_cam();
    printf("cam check! %c\r\n", t);
}

char receive_from_cam(void)
{
    while (1) {
        if (cam.readable()) {
            char recv[1];
            cam.read(recv, sizeof(recv));

            return recv[0];
        }
    }
}
