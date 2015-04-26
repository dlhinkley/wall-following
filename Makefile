
all: build load

build:
	/opt/parallax/bin/propeller-elf-gcc -I . -L . -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Utility/libsimpletools' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Utility/libsimpletools/cmm/' -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libsimpletext' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libsimpletext/cmm/' -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Protocol/libsimplei2c' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Protocol/libsimplei2c/cmm/' -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Sensor/libping' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Sensor/libping/cmm/' -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Motor/libservo' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Motor/libservo/cmm/' -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Robotics/ActivityBot/libabdrive' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Robotics/ActivityBot/libabdrive/cmm/' -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libfdserial' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libfdserial/cmm/' -o cmm/wall-following.elf -Os -mcmm -m32bit-doubles -fno-exceptions -std=c99 wall-following.c -lm -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -labdrive -lfdserial -lm -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -labdrive -lm -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -lm -lsimpletools -lsimpletext -lsimplei2c -lping -lm -lsimpletools -lsimpletext -lsimplei2c -lm -lsimpletools -lsimpletext -lm -lsimpletools -lm


load:
	/opt/parallax/bin/propeller-load -Dreset=dtr -I /opt/parallax/propeller-load/ -b ACTIVITYBOARD cmm/wall-following.elf -r -p /dev/ttyUSB0

term:
	minicom -b 115200 -o -D /dev/ttyUSB0
