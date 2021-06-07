import time
import serial
import sys,tty,termios
class _Getch:
    def __call__(self):
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch

cnt = 0
times = 1
time_scal = 0.01

def get():
    global times
    global time_scal

    left_turn_time = 1100
    direction = 1
    d1 = 0
    d2 = 0

    inkey = _Getch()
    while(1):
        k=inkey()
        if k!='':break
    if k=='\x1b':
        k2 = inkey()
        k3 = inkey()
        if k3=='A':
            print ("up", times)
            s.write("/goStraight/run 70 \n".encode())
            if times == 0:
                time.sleep(time_scal)
            else:
                for x in range(times):
                    time.sleep(time_scal)
                times = 0
        if k3=='B':
            print ("down", times)
            s.write("/goStraight/run -70 \n".encode())
            if times == 0:
                time.sleep(time_scal)
            else:
                for x in range(times):
                    time.sleep(time_scal)
                times = 0
        if k3=='C':
            print ("right", times)
            s.write("/turn/run 70 -0.7 \n".encode())
            if times == 0:
                time.sleep(time_scal)
            else:
                for x in range(times):
                    time.sleep(time_scal)
                times = 0
        if k3=='D':
            print ("left", times)
            #left_turn_time = input("left_turn_time: ")
            #front_or_back = input("front_or_back(1: front, 0: back): ")

            s.write("/turn/run 70 0.7 \n".encode())
            #s.write(("/park/run 5 " + str(front_or_back) + " 1 " + str(left_turn_time) +"\n").encode())
        s.write("/stop/run \n".encode())
    elif k=='t':
        print ("call rpc_park")
        left_turn_time = input("left_turn_time: ")
        direction = input("direction: ")
        d1 = input("d1: ")
        d2 = input("d2: ")

        s.write(("/park/run " + str(direction) + " " + str(d1) + " " + str(d2) + " " + str(left_turn_time) +"\n").encode())
    elif k=='q':
        print ("quit")
        return 0
    elif k >= '0' and k <= '9' :
        times *= 10
        times += ord(k) - ord('0')
    else:
        print ("not an arrow key!")
    return 1

if len(sys.argv) < 1:
    print ("No port input")
s = serial.Serial(sys.argv[1])


while True:
    a = s.readline()
    print("s.read: ", a.decode())

while get():
    i = 0
