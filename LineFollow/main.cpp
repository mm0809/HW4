#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BufferedSerial xbee(D1, D0);
BufferedSerial cam(D10, D9);

BBCar car(pin5, pin6, servo_ticker);


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
    while(1) {
        if (cam.readable()) {
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
        }
    }
}
