#include "bbcar_rpc.h"
RPCFunction rpcStop(&RPC_stop, "stop");
RPCFunction rpcCtrl(&RPC_goStraight, "goStraight");
RPCFunction rpcTurn(&RPC_turn, "turn");
RPCFunction rpcPark(&RPC_park, "park");

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

// 

void RPC_park (Arguments *in, Reply *out) {
    int dir = in->getArg<int>();
    int d1 = in->getArg<int>();
    int d2 = in->getArg<int>();
    int left_turn_time = in->getArg<int>();
    car.park(dir, d1, d2, left_turn_time);

    return;
}
