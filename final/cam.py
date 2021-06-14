import sensor, image, time,pyb, math
enable_lens_corr = False # turn on for straighter lines...
sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE) # grayscale is faster
sensor.set_framesize(sensor.QQVGA)
#sensor.set_windowing([0,0,160,60])
sensor.skip_frames(time = 2000)
clock = time.clock()
last_state = 0
task_state = 1
# All lines also have `x1()`, `y1()`, `x2()`, and `y2()` methods to get their end-points
# and a `line()` method to get all the above as one 4 value tuple for `draw_line()`.
def find_line_test():
    global last_state
    cnt = 0
    img = sensor.snapshot()
    check_LED = pyb.LED(1)
    a = (0, 0, 160, 55)
    lines = img.find_lines(roi = a)
    print(len(lines))
    if (len(lines) == 2):
        rx1 = (lines[0].x1()+lines[1].x1())/2
        ry1 = (lines[0].y1()+lines[1].y1())/2
        rx2 = (lines[0].x2()+lines[1].x2())/2
        ry2 = (lines[0].y2()+lines[1].y2())/2
        args = (rx1, ry1, rx2, ry2)
        print("relatively: (%d, %d) (%d, %d)" % args)
        if (abs(rx1-rx2) <= 2 and rx1 <= 90 and rx1 >= 70):
            print("F")
            last_state = 0
            uart.write(("F").encode())
        elif (rx1 > 90 and rx1 > rx2):
            print("R")
            last_state = 1
            uart.write(("R").encode())
        elif (rx1 < 70):
            last_state = 2
            print("L")
            uart.write(("L").encode())
        else:
            print("F")
            uart.write(("F").encode())
    else:
        if (last_state == 1 or last_state == 3):
            last_state = 3
            print("L")
            uart.write(("L").encode())
        elif (last_state == 2 or last_state == 4):
            last_state = 4
            print("R")
            uart.write(("R").encode())
    for l in lines:
        img.draw_line(l.line(), color = (255, 0, 0))
        cnt = cnt + 1
        print(l.x1(), l.x2(), l.y1(), l.y2())
    if cnt == 2:
        check_LED.on()
    else:
        check_LED.off()


def degrees(radians):
    return (180 * radians) / math.pi

def april_tag_detact():
    cnt = 0
    f_x = (2.8 / 3.984) * 160
    f_y = (2.8 / 2.952) * 120
    c_x = 160 * 0.5
    c_y = 120 * 0.5
    img = sensor.snapshot()
    for tag in img.find_apriltags(fx=f_x, fy=f_y, cx=c_x, cy=c_y):
        img.draw_rectangle(tag.rect(), color = (255, 0, 0))
        img.draw_cross(tag.cx(), tag.cy(), color = (0, 255, 0))
        print_args = (degrees(tag.y_rotation()))
        cnt = 1
        print("#%d!" % print_args)
        #uart.write(("#%d!" % print_args).encode())
    if cnt is 0:
        uart.write(("#z!").encode())

uart = pyb.UART(3,9600,timeout_char=1000)
while(True):
    clock.tick()
    if enable_lens_corr: img.lens_corr(1.8) # for 2.8mm lens...

    #UART
    msg = uart.readline()
    if msg is not None:
        print(a.decode())

    #run function
    if (task_state == 1):
        find_line_test()
    elif (task_state == 2):
        april_tag_detact()
