#include "bbcar.h"

#define NORMAL_FACTOR (0.4) // 0.375

// servo0: left side
// servo1: right side

BBCar::BBCar( PwmOut &pin_servo0, PwmOut &pin_servo1, Ticker &servo_ticker ):servo0(pin_servo0), servo1(pin_servo1){
    servo0.set_speed(0);
    servo1.set_speed(0);
    servo_ticker.attach(callback(this, &BBCar::controlWheel), 20ms);
}

void BBCar::park(int dir, int d1, int d2, int left_turn_time) {
    switch (dir) {
        case 1:
            goStraight(100);
            ThisThread::sleep_for(d2 * 6 * 10);

            // left turn 90 degree
            stop();
            ThisThread::sleep_for(400);
            
            turn(70, 0.7);
            ThisThread::sleep_for(left_turn_time);

            goStraight(100);
            ThisThread::sleep_for(d1 * 8 * 10);

            stop();
            break;

        case 2:
            turn(70, 0.7);
            ThisThread::sleep_for(left_turn_time + 100);

            stop();
            ThisThread::sleep_for(400);

            if (d2 > 19) {
                goStraight(100);
                ThisThread::sleep_for((d2-19) * 5 * 10);
            }

            stop();
            ThisThread::sleep_for(400);
            
            turn(70, 0.7);
            ThisThread::sleep_for(left_turn_time + 100);

            stop();
            ThisThread::sleep_for(100);

            goStraight(100);
            ThisThread::sleep_for((d1 + 5) * 8 * 10);

            stop();
            break;

        case 3:
            turn(-70, 0.7);
            ThisThread::sleep_for(left_turn_time);

            stop();
            ThisThread::sleep_for(400);

            goStraight(100);
            ThisThread::sleep_for((d2 + 25) * 5 * 10);

            stop();
            ThisThread::sleep_for(300);

            turn(70, 0.7);
            ThisThread::sleep_for(left_turn_time + 100);

            stop();
            ThisThread::sleep_for(300);

            goStraight(100);
            ThisThread::sleep_for((d1 + 5) * 8 * 10);

            stop();
            break;

        case 4:
            goStraight(-100);
            ThisThread::sleep_for((d2 + 5) * 5 * 10);

            stop();
            ThisThread::sleep_for(400);

            turn(-70, 0.7);
            ThisThread::sleep_for(left_turn_time);

            stop();
            ThisThread::sleep_for(300);

            goStraight(100);
            ThisThread::sleep_for((d1 + 27) * 8 * 10);

            stop();
            break;

        case 5:
            if (d1 == 1)
                turn(70, 0.7);
            else if (d1 == 0)
                turn(-70, 0.7);
            ThisThread::sleep_for(left_turn_time);

            stop();
            break;

        default:
            break;
    }

    return;
}

void BBCar::controlWheel(){
    servo0.control();
    servo1.control();
}

void BBCar::stop(){
    servo0.set_factor(1);
    servo1.set_factor(1);
    servo0.set_speed(0);
    servo1.set_speed(0);
}

void BBCar::goStraight( double speed ){\
    servo0.set_factor(1);
    servo1.set_factor(NORMAL_FACTOR);
    servo0.set_speed(speed);
    servo1.set_speed(-speed);
}

void BBCar::setCalibTable( int len0, double pwm_table0[], double speed_table0[], int len1, double pwm_table1[], double speed_table1[] ){
    servo0.set_calib_table(len0, pwm_table0, speed_table0);
    servo1.set_calib_table(len1, pwm_table1, speed_table1);
}
void BBCar::goStraightCalib ( double speed ){
    servo0.set_factor(1);
    servo1.set_factor(1);
    servo0.set_speed_by_cm(speed);
    servo1.set_speed_by_cm(-speed);
}

/*	speed : speed value of servo
    factor: control the speed value with 0~1
            control left/right turn with +/-
*/
void BBCar::turn( double speed, double factor ){
    if(factor>0){
        servo0.set_factor(factor);
        servo1.set_factor(1);
    }
    else if(factor<0){
        servo0.set_factor(1);
        servo1.set_factor(-factor * NORMAL_FACTOR);
    }
    servo0.set_speed(speed*1.35);
    servo1.set_speed(-speed);

}

float BBCar::clamp( float value, float max, float min ){
    if (value > max) return max;
    else if (value < min) return min;
    else return value;
}

int BBCar::turn2speed( float turn ){
    return 25+abs(25*turn);
}


