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

terminal *term;                               // For full duplex serial terminal


int PINGA = 0,
		PINGL = 16,
		PINGR = 17;

const int TARGET_WALL_DIST = 4;

static volatile int ahead = 0,
		left = 0,
		right = 0,
		isRunning = 1,
		isWallFollowing = 0,
		scanCogInit = 0;

int initialized = 0,
		speedLeft = 0,
		speedRight = 0;

unsigned int sstack[40 + 20]; // If things get weird make this number bigger!


/**
 * Retrieves a ping by doing several pings, then getting an average.  The ping from time to time will get
 * a ping with a huge error
 */
int getPing(int port) {

	int qty = 5;
	int sum = 0;     // stores sum of elements

	for ( int i = 0; i < qty; i++ ) {

		int ping = ping_cm( port );
		sum += ping;
	}
	int mean = sum / qty;

	return mean;
}


/*
 * Runs in cog
 */
void scanCog(void *par) {

	//dprint(term,"scanCogR Start\n");

	while( isRunning ) {

		right = getPing(PINGR);

		left = getPing(PINGL);

		ahead = getPing(PINGA);
		scanCogInit = 1; // scan Cog initilized
	}
	// dprint(term,"scanCogR End\n");
}

/**
 * Given an array with the current command and a numeric value argument to the command,
 * execute the command
 */
void executeCommand(char cmdbuf[], int  val, int val2) {

	dprint(term,"DEBUG:executeCommand cmdbuf=%s val1=%d val2=%d\n",cmdbuf, val, val2);

	if ( strcmp(cmdbuf,"speed") == 0 ) {
		drive_setMaxSpeed(val);
	}
	else if ( strcmp(cmdbuf,"up") == 0 ) {
		drive_goto(val,val);
	}
	else if ( strcmp(cmdbuf,"down") == 0 ) {
		drive_goto(-val,-val);
	}
	else if ( strcmp(cmdbuf,"left") == 0  ) {
		int steps = val * 0.284; // convert angle into degree
		drive_goto(-steps,steps);
	}
	else if ( strcmp(cmdbuf,"right") == 0  ) {
		int steps = val * 0.284; // convert angle into degree
		drive_goto(steps,-steps);
	}
	else if ( strcmp(cmdbuf,"slow") == 0 ) {
		drive_rampStep(0, 0);        // Slow
	}
	else if ( strcmp(cmdbuf,"stop") == 0  ) {
		drive_ramp(0, 0);        // Stop
	}
}

/*
 * Run keyboard polling in a seperate cog to prevent other actions from blocking keystrokes
 */
void pollSerial() {

	char c = 0;                                   // Stores character input

	if (fdserial_rxReady(term) != 0) { // Non blocking check for data in the input buffer

		char sbuf[40]; // A Buffer long enough to hold the longest line  may send.
		int count = 0;
		while (count < 40) {
			sbuf[count] = readChar(term);
			if (sbuf[count] == '\r' || sbuf[count] == '\n') // Read until return
				break;
			count++;
		}

		char cmdbuf[20];                             // Command buffer
		char valbuf1[4];                              // Text value buffer
		char valbuf2[4];                              // Text value buffer
		int  val1, val2;
		int i = 0;                                 // Declare index variable
		// Parse command
		while(!isalpha(sbuf[i])) i++;             // Find 1st command char

		sscan(&sbuf[i], "%s", cmdbuf);            // Command -> buffer

		i += strlen(cmdbuf);                     // Idx up by command char count

		// Parse distance argument
		while(!isdigit(sbuf[i])) i++;             // Find 1st digit after command

		sscan(&sbuf[i], "%s", valbuf1);            // Value -> buffer

		i += strlen(valbuf1);                     // Idx up by value char count

		val1 = atoi(valbuf1);                      // Convert string to value


		// Parse distance argument
		while(!isdigit(sbuf[i])) i++;             // Find 1st digit after command

		sscan(&sbuf[i], "%s", valbuf2);            // Value -> buffer

		i += strlen(valbuf2);                     // Idx up by value char count

		val2 = atoi(valbuf2);                      // Convert string to value


		executeCommand(cmdbuf, val1, val2);
	}

}
class Location {
	int ticksLeft, ticksRight, ticksLeftOld, ticksRightOld;
	double trackWidth, distancePerCount;
public:
	double heading, x, y, degHeading;
	Location();
	void update();

};
Location::Location() {
	heading = 0.0;
	x = 0.0;
	y = 0.0;
}
/*
 * Calculates the coordinates and heading of the robot based on the ticks from the servos
 */

void Location::update() {

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
	double deltaHeading = (double) (deltaTicksRight - deltaTicksLeft) * RadiansPerCount;

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

}


int main(){



//	freqout(5, 2000, 2000);               // Start beep - low battery reset alarm

	simpleterm_close();                         // Close default same-cog terminal
	term = fdserial_open(31, 30, 0, 115200);    // Set up other cog for terminal

	// Max drive_goto speed
	//drive_setMaxSpeed( speed );


	// Start the ping cog
	int scanCogPtr    = cogstart(&scanCog, NULL, sstack, sizeof(sstack));

	//int keyboardCogPtr = cogstart(&keyboardCog, NULL, kstack, sizeof(kstack));

	dprint(term,"DEBUG:main Start\n");

	while ( scanCogInit == 0  ) {

		// Wait for cogs to initialize
		pause(500);
		dprint(term,"DEBUG:main Waiting for Cog initialization\n");
	}
	if ( initialized == 0 ) {

		initialized = 1;
		dprint(term,"READY\n");
	}
	//dprint(term,"DEBUG:All Cogs initialized\n");

	drive_setRampStep(10);                      // 10 ticks/sec / 20 ms

	//drive_ramp(128, 128);                       // Forward 2 RPS

	int correction = 0;

	//findWall();

	//dprint(term,"DEBUG:main follow wall start\n");


	// Keep running
	while( isRunning ) {
		//dprint(term,"scanCog left=%d ahead=%d right=%d\n", left, ahead, right);


		pollSerial();

		dprint(term, "STATUS:right=%d\tahead=%d\tleft=%d\tspeedLeft=%d\tspeedRight=%d\n",
				      right,   ahead,   left,   speedLeft,   speedRight);

		pause(500);
	}
	dprint(term,"END\n");
	dprint(term,"DEBUG:main follow wall stop\n");

	servo_stop();                               // Stop servo process
	drive_goto(0,0);
	cogstop( scanCogPtr );
	//cogstop( keyboardCogPtr );

	dprint(term,"DEBUG:main End\n");
	return 0;
} 
