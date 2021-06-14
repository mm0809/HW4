# HW4

## How to set up adn run mu program
1.  Compile the program
```
$ sudo mbed compile --source . --source ~/ee2405/mbed-os/ -m B_L4S5I_IOT01A -t GCC_ARM --profile tflite.json -f
```

## How it work

### Xbee controled BBcar
I divide this task into 4 case according to the direction. Each task has it own path. The car will determine the time of move by distance(d1, d2).
![](https://i.imgur.com/nVLDmrx.jpg)  

### Line following BBcar
The line which the BBcar following can be detected two lines(left side and right sied), so I use find_lines to find the two lines and caculate the mid point of two lines' end. According to the coordinate of the mid point, the camera will send the moving dirction to BBcar throught UART. 
![](https://i.imgur.com/ObLbLcX.jpg)

### BBcar position calibration
Use camera to measure the angle of Apriltag surface, then determine car should go right or left.  
Suppose the car should go left, the car will turn left and go straight for a while then turn right and ask the camera to measure again. This procedure will loop until the viewangle perpendicular to the Apriltag surface. 
