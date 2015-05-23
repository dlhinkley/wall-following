/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials

propeller-elf-gcc -v GCC 4.6.1 (propellergcc_v1_0_0_2344)
propeller-elf-c++ -I . -L . -I /home/pi/SimpleIDE/Learn/Simple Libraries/Utility/libsimpletools -L /home/pi/SimpleIDE/Learn/Simple Libraries/Utility/libsimpletools/cmm/ -I /home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libsimpletext -L /home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libsimpletext/cmm/ -I /home/pi/SimpleIDE/Learn/Simple Libraries/Protocol/libsimplei2c -L /home/pi/SimpleIDE/Learn/Simple Libraries/Protocol/libsimplei2c/cmm/ -I /home/pi/SimpleIDE/Learn/Simple Libraries/Sensor/libping -L /home/pi/SimpleIDE/Learn/Simple Libraries/Sensor/libping/cmm/ -I /home/pi/SimpleIDE/Learn/Simple Libraries/Motor/libservo -L /home/pi/SimpleIDE/Learn/Simple Libraries/Motor/libservo/cmm/ -I /home/pi/SimpleIDE/Learn/Simple Libraries/Robotics/ActivityBot/libabdrive -L /home/pi/SimpleIDE/Learn/Simple Libraries/Robotics/ActivityBot/libabdrive/cmm/ -I /home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libfdserial -L /home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libfdserial/cmm/ -o cmm/wall-following.elf -Os -mcmm -m32bit-doubles -fno-exceptions -fno-rtti wall-following.cpp -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -labdrive -lfdserial -ltiny -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -labdrive -lfdserial -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -labdrive -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -ltiny -lsimpletools -lsimpletext -lsimplei2c -ltiny -lsimpletools -lsimpletext -ltiny -lsimpletools -ltiny
propeller-load -s cmm/wall-following.elf
propeller-elf-objdump -h cmm/wall-following.elf

Project Directory: /home/pi/SimpleIDE/wall-following/

propeller-elf-gcc -v GCC 4.6.1 (propellergcc_v1_0_0_2344)
propeller-elf-c++ -I . -L . -I /home/pi/SimpleIDE/Learn/Simple Libraries/Utility/libsimpletools -L /home/pi/SimpleIDE/Learn/Simple Libraries/Utility/libsimpletools/cmm/ -I /home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libsimpletext -L /home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libsimpletext/cmm/ -I /home/pi/SimpleIDE/Learn/Simple Libraries/Protocol/libsimplei2c -L /home/pi/SimpleIDE/Learn/Simple Libraries/Protocol/libsimplei2c/cmm/ -I /home/pi/SimpleIDE/Learn/Simple Libraries/Sensor/libping -L /home/pi/SimpleIDE/Learn/Simple Libraries/Sensor/libping/cmm/ -I /home/pi/SimpleIDE/Learn/Simple Libraries/Motor/libservo -L /home/pi/SimpleIDE/Learn/Simple Libraries/Motor/libservo/cmm/ -I /home/pi/SimpleIDE/Learn/Simple Libraries/Robotics/ActivityBot/libabdrive -L /home/pi/SimpleIDE/Learn/Simple Libraries/Robotics/ActivityBot/libabdrive/cmm/ -I /home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libfdserial -L /home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libfdserial/cmm/ -o cmm/wall-following.elf -Os -mcmm -m32bit-doubles -fno-exceptions -fno-rtti wall-following.cpp -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -labdrive -lfdserial -ltiny -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -labdrive -lfdserial -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -labdrive -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -ltiny -lsimpletools -lsimpletext -lsimplei2c -lping -ltiny -lsimpletools -lsimpletext -lsimplei2c -ltiny -lsimpletools -lsimpletext -ltiny -lsimpletools -ltiny
propeller-load -s cmm/wall-following.elf
propeller-elf-objdump -h cmm/wall-following.elf
Done. Build Succeeded!

propeller-load -Dreset=dtr -I /opt/parallax/propeller-load/ -b ACTIVITYBOARD cmm/wall-following.elf -r -p /dev/ttyUSB0Propeller Version 1 on /dev/ttyUSB0
Loading cmm/wall-following.elf to hub memory
16780 bytes sent

Verifying RAM ... 
OK

 */
#include "simpletools.h"                      // Include simple tools
#include "ping.h"                      // Include simple tools
#include "servo.h"                      // Include simple tools
#include "abdrive.h"
#include "adcDCpropab.h"                            // Include adcDCpropAB

terminal *term;                               // For full duplex serial terminal


int PINGA = 16, 
	PINGL = 9, 
	PINGL45 = 10, 
	PINGR = 11, 
	PINGR45 = 8, 
	TURRET = 17,
        IR_PORT = 0;

const int TARGET_WALL_DIST = 4;

static volatile int ahead = 0,
                aheadIr = 0,
		left = 0,
		left45 = 0,
		right = 0,
		right45 = 0,
                turret = 0,
		isRunning = 1;

unsigned int scanStackA[40 + 20]; 
unsigned int scanStackIr[40 + 20]; 
//unsigned int scanStackR[40 + 20]; // If things get weird make this number bigger!

int isWallFollowing = 0,
    speedLeft = 0,
    speedRight = 0;


/**
 * Retrieves a ping by doing several pings, then getting an average.  The ping from time to time will get
 * a ping with a huge error
 */
int getPing(int port) {

	int qty = 3;
	int sum = 0;     // stores sum of elements

	for ( int i = 0; i < qty; i++ ) {

		int ping = ping_cm( port );
		sum += ping;
	}
	int mean = sum / qty;

	return mean;
}
int getIr(int irPort) {
    
    float ad0;
    float avg;

    avg = 0.0;
    for (int i=0; i < 5; i++) {                 // Get average of 3 readings
      ad0 = adc_volts(irPort);                     //   from ADC0
      avg = avg + ad0;
      pause(55);
    }      
    int dist = (int) (avg / 5 * 100);
    return dist;
}
void scanCogIr(void *par) {

    while( isRunning ) {

            aheadIr = getIr(IR_PORT);
    }
}
void scanCogA(void *par) {

    while( isRunning ) {

            right = getPing(PINGR);
            ahead = getPing(PINGA);
            left = getPing(PINGL);
            left45 = getPing(PINGL45); 
            right45 = getPing(PINGR45); 
    }
}


/**
 * Given an array with the current command and a numeric value argument to the command,
 * execute the command
 */
void executeCommand(char args[][20]) {

	dprint(term,"DEBUG:executeCommand cmdbuf=%s arg1=%s arg2=%s\n",args[0], args[1], args[2]);

	if ( strcmp(args[0],"speed") == 0 ) {
		drive_setMaxSpeed( atoi(args[0]));
	}
        // drive_speed right left
	else if ( strcmp(args[0],"drive_speed") == 0 ) {
		drive_speed( atoi(args[1]), atoi(args[2]) );
	}
        // drive_goto right left
	else if ( strcmp(args[0],"drive_goto") == 0 ) {
		drive_speed( atoi(args[1]), atoi(args[2]) );
	}
	else if ( strcmp(args[0],"left") == 0  ) {
                int val = atoi(args[1]);
		int steps = val * 0.284; // convert angle into degree
		drive_speed(steps, -steps);
	}
	else if ( strcmp(args[0],"right") == 0  ) {
                int val = atoi(args[1]);
		int steps = val * 0.284; // convert angle into degree
		drive_speed(steps, -steps);
	}
	else if ( strcmp(args[0],"slow") == 0 ) {
		drive_rampStep(0, 0);        // Slow
	}
	else if ( strcmp(args[0],"stop") == 0  ) {
		drive_ramp(0, 0);        // Stop
	}
}
/*
 * Run keyboard polling in a seperate cog to prevent other actions from blocking keystrokes
 */
void pollSerial() {

    char c = 0; // Stores character input

    if (fdserial_rxReady(term) != 0) { // Non blocking check for data in the input buffer

        char args[3][20]; // Command buffer
        args[0][0] = 0;
        args[1][0] = 0;
        args[2][0] = 0;

        int count = 0;
        int argCnt = 0;
        int posPtr = 0;
        while (count < 40) {
            char c = readChar(term);

            if (c == '\r' || c == '\n') { // Read until return
                args[ argCnt ][ posPtr++ ] = 0;
                break;
            }                // Each space is a new argument
            else if (c == ' ') {

                args[ argCnt ][ posPtr++ ] = 0;

                argCnt++;
                posPtr = 0; // Beginning of array

            } else {

                args[ argCnt ][ posPtr++ ] = c;
            }
            count++;
        }

        executeCommand(args);
    }

}
class Location {
	int ticksLeft, ticksRight, ticksLeftOld, ticksRightOld;
	double heading, trackWidth, distancePerCount, x, y;
	double degHeading, coordFactor;

public:
	Location();
	void update();
	int getX();
	int getY();
	int getHeading();

};
Location::Location() {
	heading = 0.0;
	x = 0.0;
	y = 0.0;
	coordFactor = 70.0;
	degHeading = 0.0;
	trackWidth = 0.1058; // http://learn.parallax.com/activitybot/calculating-angles-rotation
	distancePerCount = 0.00325;
}
int Location::getX() {

	return (int) (x * coordFactor);
}
int Location::getHeading() {

	return (int) degHeading;
}
int Location::getY() {

	return (int) (y * coordFactor);
}
/*
 * Calculates the coordinates and heading of the robot based on the ticks from the servos
 */

void Location::update() {

	//dprint(term, "DEBUG:update start\n");

  double pi =  3.14159265359;

	ticksLeftOld = ticksLeft;
	ticksRightOld = ticksRight;
	drive_getTicks(&ticksLeft, &ticksRight);


	int deltaTicksLeft = ticksLeft - ticksLeftOld;
	int deltaTicksRight = ticksRight - ticksRightOld;

	double deltaDistance = 0.5f * (double) (deltaTicksLeft + deltaTicksRight) * distancePerCount;
	double deltaX = deltaDistance * (double) cos(heading);
	double deltaY = deltaDistance * (double) sin(heading);
	double RadiansPerCount = distancePerCount / trackWidth;
	double deltaHeading = (double) (deltaTicksLeft - deltaTicksRight) * RadiansPerCount;

	x += deltaX;
	y += deltaY;

	heading += deltaHeading;

	// limit heading to -Pi <= heading < Pi
	if (heading > pi) {
		heading -= 2.0 * pi;
	} else {
		if (heading <= -pi) {
			heading += 2.0 * pi;
		}
	}
   degHeading = heading * (180 / pi);
  if (degHeading < 0) degHeading += 360;


	///dprint(term, "DEBUG:update end\n");

}


int main(){

	Location location;
//	freqout(5, 2000, 2000);               // Start beep - low battery reset alarm
        adc_init(21, 20, 19, 18);                         // CS=21, SCL=20, DO=19, DI=18

	simpleterm_close();                         // Close default same-cog terminal
	term = fdserial_open(31, 30, 0, 115200);    // Set up other cog for terminal
	int scanCogPtrA    = cogstart(&scanCogA, NULL, scanStackA, sizeof(scanStackA));
	int scanCogPtrIr   = cogstart(&scanCogIr, NULL, scanStackIr, sizeof(scanStackIr));
//	int scanCogPtrR    = cogstart(&scanCogR, NULL, scanStackR, sizeof(scanStackR));

	while (left == 0 || right45 == 0 || left45 == 0 || right == 0 || ahead == 0 ) {

		pause (125); // wait for scanners initialized
	}

	dprint(term,"DEBUG:main Start\n");
	servo_angle(TURRET, 950);

	dprint(term,"READY\n");

	drive_setRampStep(10);                      // 10 ticks/sec / 20 ms

	// Keep running
	while( isRunning ) {

		pollSerial();

		location.update();

		dprint(term, "STATUS:%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
				      right, right45,  ahead, aheadIr, left45,  left,   speedLeft,   speedRight, location.getX(), location.getY(), location.getHeading(), turret);

		// Stop if you've run into something
		if ( left <= 5 || right <= 5 || left45 <= 5 || right45 <= 5 || ahead <= 5 ) {

			dprint(term,"DEBUG:WALL!!!!\n");
			drive_goto(0,0);
			freqout(5, 2000, 2000);
			dprint(term, "DEBUG:right=%d\tahead=%d\tleft=%d\n",
					      right,   ahead,   left);
		}

		pause(250);
	}
	dprint(term,"END\n");
	cogstop( scanCogPtrA );
	cogstop( scanCogPtrIr );
//	cogstop( scanCogPtrR );

	servo_stop();                               // Stop servo process
	drive_goto(0,0);

	return 0;
} 
