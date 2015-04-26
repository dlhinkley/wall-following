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


static volatile int ahead = 0,
		left = 0,
		right = 0,
		isRunning = 1,
		isWallFollowing = 0,
		scanCogInitA = 0,
		scanCogInitL = 0,
		scanCogInitR = 0,
		keyboardCogInit = 0;

int speed = 20;
int speedMax = 40;
int correctLil = 2;
int correctWay = 4;
int speedSlow = 5;
int minWallDist = 10;

int speedLeft = 0;
int speedRight = 0;
int prevSpeedRight = 0;
int prevSpeedLeft = 0;

unsigned int sstackA[40 + 20]; // If things get weird make this number bigger!
unsigned int sstackL[40 + 20]; // If things get weird make this number bigger!
unsigned int sstackR[40 + 20]; // If things get weird make this number bigger!
unsigned int kstack[40 + 30]; // If things get weird make this number bigger!

int ticksLeft, ticksRight, ticksLeftOld, ticksRightOld;
static double trackWidth, distancePerCount;

static volatile double heading = 0.0, x = 0.0, y = 0.0, degHeading;


void driveSpeed(int left,int right) {

	if ( left != prevSpeedLeft || right != prevSpeedRight ) {

		drive_speed(left, right);
		prevSpeedLeft = left;
		prevSpeedRight = right;

		dprint(term,"new driveSpeed left=%d right=%d\n", left, right);
	}

}

void goLeft() {
	driveSpeed(-speed, speed);
}
void goStop() {
	driveSpeed(0, 0);
}
void goRight() {
	driveSpeed(speed, -speed);
}
void goBackward() {
	driveSpeed(-speed, -speed);
}
void goForward() {
	driveSpeed(speed, speed);
}

/*
 * Drive to find the best wall
 */
/*
void findWall() {

  dprint(term,"findWall Start\n");
  dprint(term,"findWall ahead=%d minWallDist=%d\n", ahead, minWallDist );

  // Turn scan on

  // Wait until all sensors are read
  while ( ahead == 0 || left == 0 || right == 0 ) {

  }

  // If we're against the wall, backup
  if ( ahead < minWallDist ) {

    goBackward();
    // keep going backward  until not so close
    while ( ahead < minWallDist ) {

    }
  }
  // If we're not against the wall, go forward until at wall
  else {

    goForward();

    // Drive forward until find the wall
    while ( ahead > minWallDist ) {

      // wait for wall
    }
  }
  goStop();


  dprint(term,"findWall End\n");
}
 */
/**
 * Retrieves a ping by doing several pings, then getting an average.  The ping from time to time will get
 * a ping with a huge error
 */
int getPing(int port) {

	int qty = 5;
	int sum = 0;     // stores sum of elements
	//int sumsq = 0; // stores sum of squares

	for ( int i = 0; i < qty; i++ ) {

		int ping = ping_cm( port );
		sum += ping;
		//printf("Ping %d = %d\n",i,ping);
		//sumsq+= data[i] * data[i];
	}
	int mean = sum / qty;
	//int varience = sumsq / size - mean * mean;
	//printf("The mean is: %d\n", mean);
	//printf("Variance is: %d\n", varience);
	//  printf("Returned mean= %d\n", mean);

	return mean;
}


/*
 * Runs in cog
 */
void scanCogA(void *par) {

	//dprint(term,"scanCogA Start\n");

	while( isRunning ) {

		ahead = getPing(PINGA);
		scanCogInitA = 1; // scan Cog initilized
	}
	//dprint(term,"scanCogA End\n");
}
void scanCogL(void *par) {

	//dprint(term,"scanCogL Start\n");

	while( isRunning ) {

		left = getPing(PINGL);
		scanCogInitL = 1; // scan Cog initilized
	}
	// dprint(term,"scanCogL End\n");
}
void scanCogR(void *par) {

	//dprint(term,"scanCogR Start\n");

	while( isRunning ) {

		right = getPing(PINGR);
		scanCogInitR = 1; // scan Cog initilized
	}
	// dprint(term,"scanCogR End\n");
}
/*
 * Calculates the coordinates and heading of the robot based on the ticks from the servos
 */
/*
void calcCoordinates() {

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
 */
/*
 * Run keyboard polling in a seperate cog to prevent other actions from blocking keystrokes
 */
void keyboardCog(void *par) {

	dprint(term,"keyboardCog Start\n");

	char c = 0;                                   // Stores character input

	while( isRunning ) {

		// Handle any keystroke navigation
		c = fdserial_rxTime(term, 50);            // Get character from terminal


		if ( c == 'f' ) goForward();         // If 'f' then forward
		if ( c == 'b' ) goBackward();       // If 'b' then backward
		if ( c == 'l' ) goLeft();        // If 'l' then left
		if ( c == 'r' ) goRight();        // If 'r' then right
		if ( c == 's' ) goStop();           // If 's' then stop

		if ( c == 'x' ) isRunning = 0;
		if ( c == 'w' ) isWallFollowing = !isWallFollowing; // If 'w', toggle on/off

		if ( c == 'x' || c == 't' || c == 's' || c == 'r' || c  == 'l' || c == 'b' || c == 'f' )  dprint(term,"Command c=%d\n",c);

		//    calcCoordinates();  // Check for ticks

		keyboardCogInit = 1; // Cog initilized
	}
	dprint(term,"keyboardCog End\n");
}


void actionTrapped() {

	// stop
	speedRight = 0;
	speedLeft  = 0;
}
void action90Right() {

	drive_goto(26, -25);
}
void action90Left() {

	drive_goto(-25, 26);
}
void actionBackUp90Right() {

	// backup until ahead is just right
	// turn 90 to right
	// drive forward

}
void actionBackward() {

	speedRight = -speed;
	speedLeft  = -speed;
}
void actionForward() {

	speedRight = speed;
	speedLeft  = speed;
}

void turnRightCorner(  ) {

	// Go straight until past corner
	int dist = 0.325 * minWallDist + 50;

	drive_goto(dist,dist);

	// Turn right 90 degrees
	action90Right();

	// Go straight to go past edge
	drive_goto( 50, 50);

	// Keep going straigt
	actionForward();

}
void actionRightOn() {

	// correct based on dynamic adjustment right or left of right
	// 15 - 14 = 1 too close so right should go faster
	// 15 - 16 = -1 too far so right should go slower
	int adj = (minWallDist - right) ;
	speedRight = speed + adj;
	speedLeft = speed;
}
void actionLeftOn() {

	// correct based on dynamic adjustment right or left of right
	// 15 - 14 = 1 too close so left should go faster
	// 15 - 16 = -1 too far so left should go slower
	int adj = (minWallDist - left) ;
	speedRight = speed;
	speedLeft = speed + adj;
}
void actionLeftWayClose() {

	int adj = correctWay;
	speedRight = speed;
	speedLeft = speed + adj;
}
void actionRightWayClose() {

	int adj = correctWay;
	speedRight = speed + adj;
	speedLeft = speed;
}
void actionLeftLilClose() {

	int adj = correctLil;
	speedRight = speed;
	speedLeft = speed + adj;
}
void actionRightLilClose() {

	int adj = correctLil;
	speedRight = speed + adj;
	speedLeft = speed;
}
void actionRightLilFar() {

	int adj = correctLil;
	speedRight = speed;
	speedLeft = speed + adj;
}
void actionRightWayFar( int prevWallOnRight ) {

	// If we detected a wall multiple times, it's a corner.
	//
	if ( prevWallOnRight > 10 ) {

		turnRightCorner();
	}
	else {

		actionForward();
	}
}
void action() {

}

int main(){

//	freqout(5, 2000, 2000);               // Start beep - low battery reset alarm

	simpleterm_close();                         // Close default same-cog terminal
	term = fdserial_open(31, 30, 0, 115200);    // Set up other cog for terminal

	// Max drive_goto speed
	//drive_setMaxSpeed( speed );


	// Start the ping cog
	int scanCogPtrA    = cogstart(&scanCogA, NULL, sstackA, sizeof(sstackA));
	int scanCogPtrL    = cogstart(&scanCogL, NULL, sstackL, sizeof(sstackL));
	int scanCogPtrR    = cogstart(&scanCogR, NULL, sstackR, sizeof(sstackR));

	int keyboardCogPtr = cogstart(&keyboardCog, NULL, kstack, sizeof(kstack));

	dprint(term,"main Start\n");

	while ( scanCogInitA == 0 || scanCogInitL == 0 || scanCogInitR == 0 || keyboardCogInit == 0 ) {

		// Wait for cogs to initialize
		pause(500);
		dprint(term,"main Waiting for Cog initialization\n");
	}
	dprint(term,"All Cogs initialized\n");

	drive_setRampStep(10);                      // 10 ticks/sec / 20 ms

	//drive_ramp(128, 128);                       // Forward 2 RPS

	int correction = 0;

	//findWall();

	dprint(term,"main follow wall start\n");

	speedLeft = speed;
	speedRight = speed;

	int prevErrorRight   = 0;
	int prevErrorLeft    = 0;
	int lastWallDistDiff = 0;
	int prevWallOnRight  = 0;
	int adjustLeft  = 0;
	int adjustRight = 0;
	int wallOnRight = 0;
	int wayTooClose = 1;
	int lilTooClose = 2;
	int justRight   = 3;
	int lilTooFar   = 4;
	int wayTooFar   = 5;
	int leftCond    = 0;
	int rightCond   = 0;
	int aheadCond   = 0;

	// Keep running
	while( isRunning ) {
		//dprint(term,"scanCog left=%d ahead=%d right=%d\n", left, ahead, right);

		if ( isWallFollowing   ) {

			if ( left  >= 0 && left  <= minWallDist - 7 ) { leftCond  = wayTooClose; } // 15 | 5 - 8
			if ( right >= 0 && right <= minWallDist - 7 ) { rightCond = wayTooClose; prevWallOnRight = wallOnRight; wallOnRight++; } // 15 | 5 - 8
			if ( ahead >= 0 && ahead <= minWallDist - 7 ) { aheadCond = wayTooClose; } // 15 | 5 - 8

			if ( left  >= minWallDist - 6 && left  <= minWallDist - 3 ) { leftCond = lilTooClose; } // 15 | 9 - 12
			if ( right >= minWallDist - 6 && right <= minWallDist - 3 ) { rightCond = lilTooClose; prevWallOnRight = wallOnRight; wallOnRight++; } // 15 | 9 - 12

			if ( left  >= minWallDist - 2 && left  <= minWallDist + 2 ) { leftCond  = justRight; } // 15 | 13 - 17
			if ( right >= minWallDist - 2 && right <= minWallDist + 2 ) { rightCond = justRight; prevWallOnRight = wallOnRight; wallOnRight++; } // 15 | 13 - 17
			if ( ahead >= minWallDist - 2 && ahead <= minWallDist + 2 ) { aheadCond = justRight; } // 15 | 13 - 17

			if ( left  >= minWallDist + 3 && left  <= minWallDist + 6 ) { leftCond  = lilTooFar; } // 15 | 18 - 21
			if ( right >= minWallDist + 3 && right <= minWallDist + 6 ) { rightCond = lilTooFar; prevWallOnRight = wallOnRight; wallOnRight++; } // 15 | 18 - 21

			if ( left  >= minWallDist + 7  ) { leftCond  = wayTooFar; } // 15 | 22 - ~
			if ( right >= minWallDist + 7  ) { rightCond = wayTooFar; prevWallOnRight = wallOnRight; wallOnRight = 0; } // 15 | 22 - ~
			if ( ahead >= minWallDist + 7  ) { aheadCond = wayTooFar; } // 15 | 22 - ~



			if ( leftCond == wayTooClose ) {

				if ( aheadCond == wayTooClose ) {

					if ( rightCond == wayTooClose ) actionTrapped();
					if ( rightCond == lilTooClose ) actionTrapped();
					if ( rightCond == justRight   ) actionTrapped();
					if ( rightCond == lilTooFar   ) actionTrapped();
					if ( rightCond == wayTooFar   ) actionBackUp90Right();
				}

				else if ( aheadCond == justRight ) {

					if ( rightCond == wayTooClose ) actionTrapped();
					if ( rightCond == lilTooClose ) actionTrapped();
					if ( rightCond == justRight   ) actionTrapped();
					if ( rightCond == lilTooFar   ) actionTrapped();
					if ( rightCond == wayTooFar   ) actionBackUp90Right();
				}

				else if ( aheadCond == wayTooFar ) {

					if ( rightCond == wayTooClose ) actionTrapped();
					if ( rightCond == lilTooClose ) actionTrapped();
					if ( rightCond == justRight   ) actionTrapped();
					if ( rightCond == lilTooFar   ) actionLeftWayClose();
					if ( rightCond == wayTooFar   ) actionLeftWayClose();
				}
			}

			else if ( leftCond == lilTooClose ) {

				if ( aheadCond == wayTooClose ) {

					if ( rightCond == wayTooClose ) actionTrapped();
					if ( rightCond == lilTooClose ) actionTrapped();
					if ( rightCond == justRight   ) actionTrapped();
					if ( rightCond == lilTooFar   ) actionTrapped();
					if ( rightCond == wayTooFar   ) actionBackUp90Right();
				}

				else if ( aheadCond == justRight ) {

					if ( rightCond == wayTooClose ) actionTrapped();
					if ( rightCond == lilTooClose ) actionTrapped();
					if ( rightCond == justRight   ) actionTrapped();
					if ( rightCond == lilTooFar   ) actionTrapped();
					if ( rightCond == wayTooFar   ) actionBackUp90Right();
				}

				else if ( aheadCond == wayTooFar ) {

					if ( rightCond == wayTooClose ) actionTrapped();
					if ( rightCond == lilTooClose ) actionTrapped();
					if ( rightCond == justRight   ) actionTrapped();
					if ( rightCond == lilTooFar   ) actionLeftLilClose();
					if ( rightCond == wayTooFar   ) actionLeftLilClose();
				}
			}
			else if ( leftCond == justRight ) {

				if ( aheadCond == wayTooClose ) {

					if ( rightCond == wayTooClose ) actionTrapped();
					if ( rightCond == lilTooClose ) actionTrapped();
					if ( rightCond == justRight   ) actionTrapped();
					if ( rightCond == lilTooFar   ) actionTrapped();
					if ( rightCond == wayTooFar   ) actionBackUp90Right();
				}

				else if ( aheadCond == justRight ) {

					if ( rightCond == wayTooClose ) actionTrapped();
					if ( rightCond == lilTooClose ) actionTrapped();
					if ( rightCond == justRight   ) actionTrapped();
					if ( rightCond == lilTooFar   ) actionTrapped();
					if ( rightCond == wayTooFar   ) actionBackUp90Right();
				}

				else if ( aheadCond == wayTooFar ) {

					if ( rightCond == wayTooClose ) actionBackward();
					if ( rightCond == lilTooClose ) actionBackward();
					if ( rightCond == justRight   ) action90Right();
					if ( rightCond == lilTooFar   ) actionRightLilFar();
					if ( rightCond == wayTooFar   ) actionRightWayFar( prevWallOnRight );
				}
			}
			else if ( leftCond == lilTooFar ) {

				if ( aheadCond == wayTooClose ) {

					if ( rightCond == wayTooClose ) actionBackward();
					if ( rightCond == lilTooClose ) actionBackward();
					if ( rightCond == justRight   ) actionBackward();
					if ( rightCond == lilTooFar   ) actionBackward();
					if ( rightCond == wayTooFar   ) actionBackward();
				}

				else if ( aheadCond == justRight ) {

					if ( rightCond == wayTooClose ) actionBackward();
					if ( rightCond == lilTooClose ) actionBackward();
					if ( rightCond == justRight   ) action90Left();
					if ( rightCond == lilTooFar   ) action90Left();
					if ( rightCond == wayTooFar   ) actionRightWayFar( prevWallOnRight );
				}

				else if ( aheadCond == wayTooFar ) {

					if ( rightCond == wayTooClose ) actionRightWayClose();
					if ( rightCond == lilTooClose ) actionRightLilClose();
					if ( rightCond == justRight   ) actionRightOn();
					if ( rightCond == lilTooFar   ) actionRightLilFar();
					if ( rightCond == wayTooFar   ) actionRightWayFar( prevWallOnRight );
				}
			}
			/*

			 */
			else if ( leftCond == wayTooFar ) {

				if ( aheadCond == wayTooClose ) {

					if ( rightCond == wayTooClose ) actionBackward();
					if ( rightCond == lilTooClose ) actionBackward();
					if ( rightCond == justRight   ) actionBackward();
					if ( rightCond == lilTooFar   ) actionBackward();
					if ( rightCond == wayTooFar   ) actionBackward();
				}

				else if ( aheadCond == justRight ) {

					if ( rightCond == wayTooClose ) actionBackward();
					if ( rightCond == lilTooClose ) actionBackward();
					if ( rightCond == justRight   ) action90Left();
					if ( rightCond == lilTooFar   ) action90Left();
					if ( rightCond == wayTooFar   ) actionRightWayFar( prevWallOnRight );
				}

				else if ( aheadCond == wayTooFar ) {

					if ( rightCond == wayTooClose ) actionRightWayClose();
					if ( rightCond == lilTooClose ) actionRightLilClose();
					if ( rightCond == justRight   ) actionRightOn();
					if ( rightCond == lilTooFar   ) actionRightLilFar();
					if ( rightCond == wayTooFar   ) actionRightWayFar( prevWallOnRight );
				}
			}


			dprint(term, "right=%d ahead=%d left=%d rightCond=%d aheadCond=%d leftCond=%d prevWallOnRight=%d wallOnRight=%d speedLeft=%d speedRight=%d\n",
					      right,   ahead,   left,   rightCond,   aheadCond,   leftCond,   prevWallOnRight,   wallOnRight,   speedLeft,   speedRight);
			pause(250);

			driveSpeed(speedLeft, speedRight);

			//pause(250);
		}
		//printf("Angle=%d Distance=%d\n", scanAngle[scanPtr], scanPing[scanPtr]);

	}
	dprint(term,"main follow wall stop\n");

	servo_stop();                               // Stop servo process
	drive_goto(0,0);
	cogstop( scanCogPtrA );
	cogstop( scanCogPtrL );
	cogstop( scanCogPtrR );
	cogstop( keyboardCogPtr );

	dprint(term,"main End\n");
	return 0;
} 
