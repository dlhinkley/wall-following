/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials
  propeller-elf-gcc -I . -L . -I /home/pi/SimpleIDE/Learn/Simple Libraries/Utility/libsimpletools -L /home/pi/SimpleIDE/Learn/Simple Libraries/Utility/libsimpletools/cmm/ -I /home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libsimpletext -L /home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libsimpletext/cmm/ -I /home/pi/SimpleIDE/Learn/Simple Libraries/Protocol/libsimplei2c -L /home/pi/SimpleIDE/Learn/Simple Libraries/Protocol/libsimplei2c/cmm/ -I /home/pi/SimpleIDE/Learn/Simple Libraries/Sensor/libping -L /home/pi/SimpleIDE/Learn/Simple Libraries/Sensor/libping/cmm/ -I /home/pi/SimpleIDE/Learn/Simple Libraries/Motor/libservo -L /home/pi/SimpleIDE/Learn/Simple Libraries/Motor/libservo/cmm/ -I /home/pi/SimpleIDE/Learn/Simple Libraries/Robotics/ActivityBot/libabdrive -L /home/pi/SimpleIDE/Learn/Simple Libraries/Robotics/ActivityBot/libabdrive/cmm/ -I /home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libfdserial -L /home/pi/SimpleIDE/Learn/Simple Libraries/Text Devices/libfdserial/cmm/ -o cmm/wall-following.elf -Os -mcmm -m32bit-doubles -fno-exceptions -std=c99 wall-following.c -lm -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -labdrive -lfdserial -lm -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -labdrive -lm -lsimpletools -lsimpletext -lsimplei2c -lping -lservo -lm -lsimpletools -lsimpletext -lsimplei2c -lping -lm -lsimpletools -lsimpletext -lsimplei2c -lm -lsimpletools -lsimpletext -lm -lsimpletools -lm

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
int speedSlow = 5;
int minWallDist = 10;

int speedLeft = 0;
int speedRight = 0;

unsigned int sstackA[40 + 20]; // If things get weird make this number bigger!
unsigned int sstackL[40 + 20]; // If things get weird make this number bigger!
unsigned int sstackR[40 + 20]; // If things get weird make this number bigger!
unsigned int kstack[40 + 30]; // If things get weird make this number bigger!

int ticksLeft, ticksRight, ticksLeftOld, ticksRightOld;
static double trackWidth, distancePerCount;

static volatile double heading = 0.0, x = 0.0, y = 0.0, degHeading;




void goLeft() {
	drive_speed(-speed, speed);
}
void goStop() {
	drive_speed(0, 0);
}
void goRight() {
	drive_speed(speed, -speed);
}
void goBackward() {
	drive_speed(-speed, -speed);
}
void goForward() {
	drive_speed(speed, speed);
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

	int adj = 8;
	speedRight = speed;
	speedLeft = speed + adj;
}
void actionRightWayClose() {

	int adj = 8;
	speedRight = speed + adj;
	speedLeft = speed;
}
void actionLeftLilClose() {

	int adj = 5;
	speedRight = speed;
	speedLeft = speed + adj;
}
void actionRightLilClose() {

	int adj = 5;
	speedRight = speed + adj;
	speedLeft = speed;
}
void actionRightLilFar() {

	int adj = 5;
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

	freqout(5, 2000, 2000);               // Start beep - low battery reset alarm

	simpleterm_close();                         // Close default same-cog terminal
	term = fdserial_open(31, 30, 0, 115200);    // Set up other cog for terminal

	// Max drive_goto speed
	drive_setMaxSpeed( speed );


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
	int adjustLeft = 0;
	int adjustRight = 0;
	int prevErrorRight = 0;
	int prevErrorLeft = 0;
	int lastWallDistDiff = 0,
        wallOnRight = 0,
        prevWallOnRight = 0;

	// Keep running
	while( isRunning ) {
		//dprint(term,"scanCog left=%d ahead=%d right=%d\n", left, ahead, right);

		if ( isWallFollowing   ) {

			int wayTooClose = 1,
					lilTooClose = 2,
					justRight = 3,
					lilTooFar = 4,
					wayTooFar = 5,
					leftCond = 0,
					rightCond = 0,
					aheadCond = 0;


			if ( left  >= 0 && left  <= minWallDist - 7 ) leftCond  = wayTooClose; // 15 | 5 - 8
			if ( right >= 0 && right <= minWallDist - 7 ) rightCond = wayTooClose, prevWallOnRight = wallOnRight, wallOnRight++; // 15 | 5 - 8
			if ( ahead >= 0 && ahead <= minWallDist - 7 ) aheadCond = wayTooClose; // 15 | 5 - 8

			if ( left  >= minWallDist - 6 && left  <= minWallDist - 3 ) leftCond = lilTooClose; // 15 | 9 - 12
			if ( right >= minWallDist - 6 && right <= minWallDist - 3 ) rightCond = lilTooClose, prevWallOnRight = wallOnRight, wallOnRight++; // 15 | 9 - 12

			if ( left  >= minWallDist - 2 && left  <= minWallDist + 2 ) leftCond  = justRight; // 15 | 13 - 17
			if ( right >= minWallDist - 2 && right <= minWallDist + 2 ) rightCond = justRight, prevWallOnRight = wallOnRight, wallOnRight++; // 15 | 13 - 17
			if ( ahead >= minWallDist - 2 && ahead <= minWallDist + 2 ) aheadCond = justRight; // 15 | 13 - 17

			if ( left  >= minWallDist + 3 && left  <= minWallDist + 6 ) leftCond  = lilTooFar; // 15 | 18 - 21
			if ( right >= minWallDist + 3 && right <= minWallDist + 6 ) rightCond = lilTooFar, prevWallOnRight = wallOnRight, wallOnRight++; // 15 | 18 - 21

			if ( left  >= minWallDist + 7  ) leftCond  = wayTooFar; // 15 | 22 - ~
			if ( right >= minWallDist + 7  ) rightCond = wayTooFar, prevWallOnRight = wallOnRight, wallOnRight = 0; // 15 | 22 - ~
			if ( ahead >= minWallDist + 7  ) aheadCond = wayTooFar; // 15 | 22 - ~



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

right=3 ahead=43 left=82 rightCond=1 aheadCond=0 leftCond=1 prevWallOnRight=5 wallOnRight=5 sp
eedLeft=20 speedRight=28
right=3 ahead=43 left=82 rightCond=1 aheadCond=1 leftCond=2 prevWallOnRight=5 wallOnRight=5 sp
eedLeft=20 speedRight=28
right=3 ahead=43 left=82 rightCond=1 aheadCond=2 leftCond=3 prevWallOnRight=5 wallOnRight=5 sp
eedLeft=20 speedRight=28
right=3 ahead=43 left=79 rightCond=1 aheadCond=3 leftCond=4 prevWallOnRight=5 wallOnRight=5 sp
eedLeft=20 speedRight=28
right=3 ahead=41 left=79 rightCond=1 aheadCond=4 leftCond=5 prevWallOnRight=5 wallOnRight=5 sp
eedLeft=20 speedRight=28
right=3 ahead=41 left=76 rightCond=1 aheadCond=5 leftCond=6 prevWallOnRight=5 wallOnRight=5 sp
eedLeft=20 speedRight=28
right=3 ahead=40 left=76 rightCond=1 aheadCond=6 leftCond=7 prevWallOnRight=5 wallOnRight=5 sp
eedLeft=20 speedRight=28
right=3 ahead=40 left=76 rightCond=1 aheadCond=7 leftCond=8 prevWallOnRight=5 wallOnRight=5 sp
eedLeft=20 speedRight=28
right=4 ahead=30 left=76 rightCond=2 aheadCond=8 leftCond=9 prevWallOnRight=5 wallOnRight=5 sp
eedLeft=20 speedRight=25
right=4 ahead=30 left=64 rightCond=2 aheadCond=9 leftCond=10 prevWallOnRight=5 wallOnRight=5 s
peedLeft=20 speedRight=25
right=3 ahead=36 left=64 rightCond=1 aheadCond=10 leftCond=11 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=36 left=64 rightCond=1 aheadCond=11 leftCond=12 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=34 left=42 rightCond=1 aheadCond=12 leftCond=13 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=34 left=42 rightCond=1 aheadCond=13 leftCond=14 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=33 left=72 rightCond=1 aheadCond=14 leftCond=15 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=33 left=47 rightCond=1 aheadCond=15 leftCond=16 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=69 left=47 rightCond=1 aheadCond=16 leftCond=17 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=69 left=49 rightCond=1 aheadCond=17 leftCond=18 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=45 left=49 rightCond=1 aheadCond=18 leftCond=19 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=45 left=57 rightCond=1 aheadCond=19 leftCond=20 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=90 left=57 rightCond=1 aheadCond=20 leftCond=21 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=90 left=49 rightCond=1 aheadCond=21 leftCond=22 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=60 left=49 rightCond=1 aheadCond=22 leftCond=23 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=2 ahead=60 left=47 rightCond=1 aheadCond=23 leftCond=24 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=2 ahead=37 left=47 rightCond=1 aheadCond=24 leftCond=25 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=4 ahead=37 left=59 rightCond=2 aheadCond=25 leftCond=26 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=25
right=4 ahead=70 left=59 rightCond=2 aheadCond=26 leftCond=27 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=25
right=3 ahead=70 left=59 rightCond=1 aheadCond=27 leftCond=28 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=68 left=59 rightCond=1 aheadCond=28 leftCond=29 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=2 ahead=68 left=58 rightCond=1 aheadCond=29 leftCond=30 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=2 ahead=90 left=58 rightCond=1 aheadCond=30 leftCond=31 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=2 ahead=90 left=58 rightCond=1 aheadCond=31 leftCond=32 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=2 ahead=76 left=58 rightCond=1 aheadCond=32 leftCond=33 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=4 ahead=76 left=58 rightCond=2 aheadCond=33 leftCond=34 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=25
right=4 ahead=90 left=58 rightCond=2 aheadCond=34 leftCond=35 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=25
right=3 ahead=90 left=50 rightCond=1 aheadCond=35 leftCond=36 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=61 left=50 rightCond=1 aheadCond=36 leftCond=37 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=61 left=44 rightCond=1 aheadCond=37 leftCond=38 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=73 left=44 rightCond=1 aheadCond=38 leftCond=39 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=2 ahead=73 left=56 rightCond=1 aheadCond=39 leftCond=40 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=2 ahead=57 left=56 rightCond=1 aheadCond=40 leftCond=41 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=57 left=58 rightCond=1 aheadCond=41 leftCond=42 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=40 left=58 rightCond=1 aheadCond=42 leftCond=43 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=40 left=58 rightCond=1 aheadCond=43 leftCond=44 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=2 ahead=52 left=58 rightCond=1 aheadCond=44 leftCond=45 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=2 ahead=52 left=56 rightCond=1 aheadCond=45 leftCond=46 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=35 left=56 rightCond=1 aheadCond=46 leftCond=47 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=35 left=56 rightCond=1 aheadCond=47 leftCond=48 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=19 left=39 rightCond=1 aheadCond=48 leftCond=49 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
right=3 ahead=19 left=39 rightCond=1 aheadCond=49 leftCond=50 prevWallOnRight=5 wallOnRight=5 
speedLeft=20 speedRight=28
Command c=120

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
			dprint(term, "right=%d ahead=%d left=%d rightCond=%d aheadCond=%d leftCond=%d prevWallOnRight=%d wallOnRight=%d speedLeft=%d speedRight=%d\n", right, ahead, left, rightCond, prevWallOnRight, wallOnRight, aheadCond, leftCond, speedLeft, speedRight);
			pause(250);

			drive_speed(speedLeft, speedRight);

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
