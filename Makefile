
all: build load

build1:
	mkdir -p cmm && /opt/parallax/bin/propeller-elf-c++ -I . -L . -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Convert/libadcDCpropab/' -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Utility/libsimpletools' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Utility/libsimpletools/cmm/' -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libsimpletext' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libsimpletext/cmm/' -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Protocol/libsimplei2c' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Protocol/libsimplei2c/cmm/' -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Sensor/libping' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Sensor/libping/cmm/' -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Motor/libservo' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Motor/libservo/cmm/' -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Robotics/ActivityBot/libabdrive' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Robotics/ActivityBot/libabdrive/cmm/' -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libfdserial' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libfdserial/cmm/' -o cmm/wall-following.elf -Os -mcmm -m32bit-doubles -fno-exceptions -fno-rtti wall-following.cpp -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -labdrive -lfdserial -ltiny -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -labdrive -lfdserial -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -labdrive -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -ltiny -lsimpletools -lsimpletext -lsimplei2c -ltiny -lsimpletools -lsimpletext -ltiny -lsimpletools -ltiny

build:
	mkdir -p cmm && /opt/parallax/bin/propeller-elf-c++ -I . -L . -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Utility/libsimpletools' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Utility/libsimpletools/cmm/' -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libsimpletext' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libsimpletext/cmm/' -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Protocol/libsimplei2c' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Protocol/libsimplei2c/cmm/' -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Sensor/libping' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Sensor/libping/cmm/' -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Motor/libservo' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Motor/libservo/cmm/' -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Robotics/ActivityBot/libabdrive' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Robotics/ActivityBot/libabdrive/cmm/' -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libfdserial' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libfdserial/cmm/' -I '/home/pi/SimpleIDE/Learn/Simple Libraries/Convert/libadcDCpropab' -L '/home/pi/SimpleIDE/Learn/Simple Libraries/Convert/libadcDCpropab/cmm/' -o 'cmm/wall-following.elf' -Os -mcmm -m32bit-doubles -fno-exceptions -fno-rtti wall-following.cpp -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -labdrive -lfdserial -ladcDCpropab -ltiny -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -labdrive -lfdserial -ladcDCpropab -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -labdrive -lfdserial -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -labdrive -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -ltiny -lsimpletools -lsimpletext -lsimplei2c -ltiny -lsimpletools -lsimpletext -ltiny -lsimpletools -ltiny

load:
	/opt/parallax/bin/propeller-load -Dreset=dtr -I /opt/parallax/propeller-load/ -b ACTIVITYBOARD cmm/wall-following.elf -r -p /dev/ttyUSB0

eprom:
	/opt/parallax/bin/propeller-load -Dreset=dtr -I /opt/parallax/propeller-load/ -b ACTIVITYBOARD cmm/wall-following.elf -e -r -p /dev/ttyUSB0

term:
	minicom -b 115200 -o -D /dev/ttyUSB0 

copy:
	scp dist/WallFollowing.jar pi@10.0.0.27:~/SimpleIDE/wall-following/dist/

java:
	cd dist && java -Djava.library.path=/usr/lib/jni  -cp .:lib/ -jar WallFollowing.jar
