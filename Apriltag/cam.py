import pyb, sensor, image, time, math
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
clock = time.clock()
f_x = (2.8 / 3.984) * 160
f_y = (2.8 / 2.952) * 120
c_x = 160 * 0.5
c_y = 120 * 0.5
def degrees(radians):
   return (180 * radians) / math.pi
def april_tag_detact():
	cnt = 0
	img = sensor.snapshot()
	for tag in img.find_apriltags(fx=f_x, fy=f_y, cx=c_x, cy=c_y):
		img.draw_rectangle(tag.rect(), color = (255, 0, 0))
		img.draw_cross(tag.cx(), tag.cy(), color = (0, 255, 0))
		print_args = (degrees(tag.y_rotation()))
		cnt = 1
		uart.write(("#%d!" % print_args).encode())
	if cnt is 0:
		uart.write(("#z!").encode())
uart = pyb.UART(3,9600,timeout_char=1000)
uart.init(9600,bits=8,parity = None, stop=1, timeout_char=1000)
tmp = ""
while(True):
	a = uart.readline()
	if a is not None:
		tmp += a.decode()
		print(a.decode())
	if tmp == "april\0":
		april_tag_detact()
		uart.readchar()
		tmp = ""
