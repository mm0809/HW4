#include "bbcar_rpc.h"
RPCFunction rpcStop(&RPC_stop, "stop");
RPCFunction rpcCtrl(&RPC_goStraight, "goStraight");
RPCFunction rpcTurn(&RPC_turn, "turn");

RPCFunction rpcF(&RPC_F, "F");
RPCFunction rpcL(&RPC_L, "L");
RPCFunction rpcR(&RPC_R, "R");

extern BBCar car;

void RPC_stop (Arguments *in, Reply *out)   {
    car.stop();
    return;
}

void RPC_goStraight (Arguments *in, Reply *out)   {
    int speed = in->getArg<double>();
    car.goStraight(speed);
    return;
}

void RPC_turn (Arguments *in, Reply *out)   {
    int speed = in->getArg<double>();
    double turn = in->getArg<double>();
    car.turn(speed,turn);
    return;
}

void RPC_F(Arguments *in, Reply *out) {
    car.goStraight(80);
    return;
}
void RPC_L(Arguments *in, Reply *out) {
    car.servo0.set_factor(1);
    car.servo1.set_factor(0.35);
    car.servo0.set_speed(in->getArg<double>());
    car.servo1.set_speed(in->getArg<double>());
}
void RPC_R(Arguments *in, Reply *out) {
    car.servo0.set_factor(1);
    car.servo1.set_factor(0.35);
    car.servo0.set_speed(in->getArg<double>());
    car.servo1.set_speed(in->getArg<double>());
    //car.servo0.set_speed(80);
    //car.servo1.set_speed(-70);
}
