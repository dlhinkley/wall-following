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





unsigned int sstackA[40 + 20]; // If things get weird make this number bigger!
unsigned int sstackL[40 + 20]; // If things get weird make this number bigger!
unsigned int sstackR[40 + 20]; // If things get weird make this number bigger!
unsigned int kstack[40 + 30]; // If things get weird make this number bigger!

int ticksLeft, ticksRight, ticksLeftOld, ticksRightOld;
static double trackWidth, distancePerCount;

static volatile double heading = 0.0, x = 0.0, y = 0.0, degHeading;





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


		//if ( c == 'f' ) goForward();         // If 'f' then forward
		//if ( c == 'b' ) goBackward();       // If 'b' then backward
		//if ( c == 'l' ) goLeft();        // If 'l' then left
		//if ( c == 'r' ) goRight();        // If 'r' then right
		//if ( c == 's' ) goStop();           // If 's' then stop

		if ( c == 'x' ) isRunning = 0;
		if ( c == 'w' ) isWallFollowing = !isWallFollowing; // If 'w', toggle on/off

		if ( c == 'x' || c == 't' || c == 's' || c == 'r' || c  == 'l' || c == 'b' || c == 'f' )  dprint(term,"Command c=%d\n",c);

		//    calcCoordinates();  // Check for ticks

		keyboardCogInit = 1; // Cog initilized
	}
	dprint(term,"keyboardCog End\n");
}
class Action {
	int prevSpeedRight;
	int prevSpeedLeft;
	int correctLil;
	int correctWay;
	int speed;
	int speedMax;
	int speedSlow;
	int minWallDist;

	void turnRightCorner(  );
	void driveSpeed( int, int);

public:
	Action();
	void trapped();
	void right90();
	void left90();
	void backward();
	void forward();
	void backUp90Right();
	void rightOn();
	void leftOn();
	void leftWayClose();
	void rightLilFar();
	void rightWayFar();
	void rightWayClose();
	void rightLilClose();
	void leftLilClose();
	void action();
	void drive();
	void goLeft();
	void goStop();
	void goRight();
	void goBackward();
	void goForward();
	int speedLeft;
	int speedRight;
	int prevWallOnRight;
	int wallOnRight;

};
Action::Action() {

	speedLeft = 0;
	speedRight = 0;
	prevSpeedRight = 0;
	prevSpeedLeft = 0;
	correctLil = 2;
	correctWay = 4;
	speed = 20;
	speedMax = 40;
	speedSlow = 5;
	minWallDist = 10;
}

void Action::goLeft() {
	driveSpeed(-speed, speed);
}
void Action::goStop() {
	driveSpeed(0, 0);
}
void Action::goRight() {
	driveSpeed(speed, -speed);
}
void Action::goBackward() {
	driveSpeed(-speed, -speed);
}
void Action::goForward() {
	driveSpeed(speed, speed);
}

void Action::driveSpeed(int left,int right) {

	if ( left != prevSpeedLeft || right != prevSpeedRight ) {

		drive_speed(left, right);
		prevSpeedLeft = left;
		prevSpeedRight = right;

		dprint(term,"new driveSpeed left=%d right=%d\n", left, right);
	}

}
void Action::trapped() {

	// stop
	speedRight = 0;
	speedLeft  = 0;
}
void Action::right90() {

	drive_goto(26, -25);
}
void Action::left90() {

	drive_goto(-25, 26);
}
void Action::backUp90Right() {

	// backup until ahead is just right
	// turn 90 to right
	// drive forward

}
void Action::backward() {

	driveSpeed(-speed, -speed);
}
void Action::forward() {

	driveSpeed(speed, speed);
}

void Action::turnRightCorner(  ) {

	// Go straight until past corner
	int dist = 0.325 * minWallDist + 50;

	drive_goto(dist,dist);

	// Turn right 90 degrees
	right90();

	// Go straight to go past edge
	drive_goto( 50, 50);

	// Keep going straigt
	forward();

}
void Action::rightOn() {

	// correct based on dynamic adjustment right or left of right
	// 15 - 14 = 1 too close so right should go faster
	// 15 - 16 = -1 too far so right should go slower
	int adj = (minWallDist - right) ;

	driveSpeed(speed, speed + adj);
}
void Action::leftOn() {

	// correct based on dynamic adjustment right or left of right
	// 15 - 14 = 1 too close so left should go faster
	// 15 - 16 = -1 too far so left should go slower
	int adj = (minWallDist - left) ;

	driveSpeed(speed + adj, speed);
}
void Action::leftWayClose() {

	driveSpeed(speed + correctWay, speed);
}
void Action::rightWayClose() {

	driveSpeed(speed, speed + correctWay);
}
void Action::leftLilClose() {

	driveSpeed(speed, speed + correctLil);

}
void Action::rightLilClose() {

	driveSpeed(speed, speed + correctLil);

}
void Action::rightLilFar() {

	driveSpeed(speed + correctLil, speed);
}
void Action::rightWayFar( ) {

	// If we detected a wall multiple times, it's a corner.
	//
	if ( prevWallOnRight > 10 ) {

		turnRightCorner();
	}
	else {

		forward();
	}
}
void Action::action() {

}

enum CondValue { WAY_TOO_CLOSE, LIL_TOO_CLOSE, JUST_RIGHT, LIL_TOO_FAR, WAY_TOO_FAR };

class Condition {

	int minWallDist;

	CondValue cond;


	public:
		Condition();
		void setCondition(int);
		CondValue getCondition();

};
Condition::Condition() {

	minWallDist = 10;
}
CondValue Condition::getCondition() {

	return cond;
}
void Condition::setCondition(int distance) {

	if ( distance  >= 0 && distance  <= minWallDist - 7 ) { cond  = WAY_TOO_CLOSE; } // 15 | 5 - 8

	if ( distance  >= minWallDist - 6 && distance  <= minWallDist - 3 ) { cond = LIL_TOO_CLOSE; } // 15 | 9 - 12

	if ( distance  >= minWallDist - 2 && distance  <= minWallDist + 2 ) { cond  = JUST_RIGHT; } // 15 | 13 - 17

	if ( distance  >= minWallDist + 3 && distance  <= minWallDist + 6 ) { cond  = LIL_TOO_FAR; } // 15 | 18 - 21

	if ( distance  >= minWallDist + 7  ) { cond  = WAY_TOO_FAR; } // 15 | 22 - ~
}

int main(){

	Condition leftCond;
	Condition aheadCond;
	Condition rightCond;
	Action action;

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


	// Keep running
	while( isRunning ) {
		//dprint(term,"scanCog left=%d ahead=%d right=%d\n", left, ahead, right);

		if ( isWallFollowing   ) {

			leftCond.setCondition( left );
			aheadCond.setCondition( ahead );
			rightCond.setCondition( right );


			if ( leftCond.getCondition() == WAY_TOO_CLOSE ) {

				if ( aheadCond.getCondition() == WAY_TOO_CLOSE ) {

					if ( rightCond.getCondition() == WAY_TOO_CLOSE ) action.trapped();
					if ( rightCond.getCondition() == LIL_TOO_CLOSE ) action.trapped();
					if ( rightCond.getCondition() == JUST_RIGHT    ) action.trapped();
					if ( rightCond.getCondition() == LIL_TOO_FAR   ) action.trapped();
					if ( rightCond.getCondition() == WAY_TOO_FAR   ) action.backUp90Right();
				}

				else if ( aheadCond.getCondition() == JUST_RIGHT ) {

					if ( rightCond.getCondition() == WAY_TOO_CLOSE ) action.trapped();
					if ( rightCond.getCondition() == LIL_TOO_CLOSE ) action.trapped();
					if ( rightCond.getCondition() == JUST_RIGHT    ) action.trapped();
					if ( rightCond.getCondition() == LIL_TOO_FAR   ) action.trapped();
					if ( rightCond.getCondition() == WAY_TOO_FAR   ) action.backUp90Right();
				}

				else if ( aheadCond.getCondition() == WAY_TOO_FAR ) {

					if ( rightCond.getCondition() == WAY_TOO_CLOSE ) action.trapped();
					if ( rightCond.getCondition() == LIL_TOO_CLOSE ) action.trapped();
					if ( rightCond.getCondition() == JUST_RIGHT    ) action.trapped();
					if ( rightCond.getCondition() == LIL_TOO_FAR   ) action.leftWayClose();
					if ( rightCond.getCondition() == WAY_TOO_FAR   ) action.leftWayClose();
				}
			}

			else if ( leftCond.getCondition() == LIL_TOO_CLOSE ) {

				if ( aheadCond.getCondition() == WAY_TOO_CLOSE ) {

					if ( rightCond.getCondition() == WAY_TOO_CLOSE ) action.trapped();
					if ( rightCond.getCondition() == LIL_TOO_CLOSE ) action.trapped();
					if ( rightCond.getCondition() == JUST_RIGHT    ) action.trapped();
					if ( rightCond.getCondition() == LIL_TOO_FAR   ) action.trapped();
					if ( rightCond.getCondition() == WAY_TOO_FAR   ) action.backUp90Right();
				}

				else if ( aheadCond.getCondition() == JUST_RIGHT ) {

					if ( rightCond.getCondition() == WAY_TOO_CLOSE ) action.trapped();
					if ( rightCond.getCondition() == LIL_TOO_CLOSE ) action.trapped();
					if ( rightCond.getCondition() == JUST_RIGHT    ) action.trapped();
					if ( rightCond.getCondition() == LIL_TOO_FAR   ) action.trapped();
					if ( rightCond.getCondition() == WAY_TOO_FAR   ) action.backUp90Right();
				}

				else if ( aheadCond.getCondition() == WAY_TOO_FAR ) {

					if ( rightCond.getCondition() == WAY_TOO_CLOSE ) action.trapped();
					if ( rightCond.getCondition() == LIL_TOO_CLOSE ) action.trapped();
					if ( rightCond.getCondition() == JUST_RIGHT    ) action.trapped();
					if ( rightCond.getCondition() == LIL_TOO_FAR   ) action.leftLilClose();
					if ( rightCond.getCondition() == WAY_TOO_FAR   ) action.leftLilClose();
				}
			}
			else if ( leftCond.getCondition() == JUST_RIGHT ) {

				if ( aheadCond.getCondition() == WAY_TOO_CLOSE ) {

					if ( rightCond.getCondition() == WAY_TOO_CLOSE ) action.trapped();
					if ( rightCond.getCondition() == LIL_TOO_CLOSE ) action.trapped();
					if ( rightCond.getCondition() == JUST_RIGHT    ) action.trapped();
					if ( rightCond.getCondition() == LIL_TOO_FAR   ) action.trapped();
					if ( rightCond.getCondition() == WAY_TOO_FAR   ) action.backUp90Right();
				}

				else if ( aheadCond.getCondition() == JUST_RIGHT ) {

					if ( rightCond.getCondition() == WAY_TOO_CLOSE ) action.trapped();
					if ( rightCond.getCondition() == LIL_TOO_CLOSE ) action.trapped();
					if ( rightCond.getCondition() == JUST_RIGHT    ) action.trapped();
					if ( rightCond.getCondition() == LIL_TOO_FAR   ) action.trapped();
					if ( rightCond.getCondition() == WAY_TOO_FAR   ) action.backUp90Right();
				}

				else if ( aheadCond.getCondition() == WAY_TOO_FAR ) {

					if ( rightCond.getCondition() == WAY_TOO_CLOSE ) action.backward();
					if ( rightCond.getCondition() == LIL_TOO_CLOSE ) action.backward();
					if ( rightCond.getCondition() == JUST_RIGHT    ) action.right90();
					if ( rightCond.getCondition() == LIL_TOO_FAR   ) action.rightLilFar();
					if ( rightCond.getCondition() == WAY_TOO_FAR   ) action.rightWayFar(  );
				}
			}
			else if ( leftCond.getCondition() == LIL_TOO_FAR ) {

				if ( aheadCond.getCondition() == WAY_TOO_CLOSE ) {

					if ( rightCond.getCondition() == WAY_TOO_CLOSE ) action.backward();
					if ( rightCond.getCondition() == LIL_TOO_CLOSE ) action.backward();
					if ( rightCond.getCondition() == JUST_RIGHT    ) action.backward();
					if ( rightCond.getCondition() == LIL_TOO_FAR   ) action.backward();
					if ( rightCond.getCondition() == WAY_TOO_FAR   ) action.backward();
				}

				else if ( aheadCond.getCondition() == JUST_RIGHT ) {

					if ( rightCond.getCondition() == WAY_TOO_CLOSE ) action.backward();
					if ( rightCond.getCondition() == LIL_TOO_CLOSE ) action.backward();
					if ( rightCond.getCondition() == JUST_RIGHT    ) action.left90();
					if ( rightCond.getCondition() == LIL_TOO_FAR   ) action.left90();
					if ( rightCond.getCondition() == WAY_TOO_FAR   ) action.rightWayFar(  );
				}

				else if ( aheadCond.getCondition() == WAY_TOO_FAR ) {

					if ( rightCond.getCondition() == WAY_TOO_CLOSE ) action.rightWayClose();
					if ( rightCond.getCondition() == LIL_TOO_CLOSE ) action.rightLilClose();
					if ( rightCond.getCondition() == JUST_RIGHT    ) action.rightOn();
					if ( rightCond.getCondition() == LIL_TOO_FAR   ) action.rightLilFar();
					if ( rightCond.getCondition() == WAY_TOO_FAR   ) action.rightWayFar(  );
				}
			}
			/*

			 */
			else if ( leftCond.getCondition() == WAY_TOO_FAR ) {

				if ( aheadCond.getCondition() == WAY_TOO_CLOSE ) {

					if ( rightCond.getCondition() == WAY_TOO_CLOSE ) action.backward();
					if ( rightCond.getCondition() == LIL_TOO_CLOSE ) action.backward();
					if ( rightCond.getCondition() == JUST_RIGHT    ) action.backward();
					if ( rightCond.getCondition() == LIL_TOO_FAR   ) action.backward();
					if ( rightCond.getCondition() == WAY_TOO_FAR   ) action.backward();
				}

				else if ( aheadCond.getCondition() == JUST_RIGHT ) {

					if ( rightCond.getCondition() == WAY_TOO_CLOSE ) action.backward();
					if ( rightCond.getCondition() == LIL_TOO_CLOSE ) action.backward();
					if ( rightCond.getCondition() == JUST_RIGHT    ) action.left90();
					if ( rightCond.getCondition() == LIL_TOO_FAR   ) action.left90();
					if ( rightCond.getCondition() == WAY_TOO_FAR   ) action.rightWayFar(  );
				}

				else if ( aheadCond.getCondition() == WAY_TOO_FAR ) {

					if ( rightCond.getCondition() == WAY_TOO_CLOSE ) action.rightWayClose();
					if ( rightCond.getCondition() == LIL_TOO_CLOSE ) action.rightLilClose();
					if ( rightCond.getCondition() == JUST_RIGHT    ) action.rightOn();
					if ( rightCond.getCondition() == LIL_TOO_FAR   ) action.rightLilFar();
					if ( rightCond.getCondition() == WAY_TOO_FAR   ) action.rightWayFar(  );
				}
			}


			dprint(term, "right=%d ahead=%d left=%d rightCond=%d aheadCond=%d leftCond=%d prevWallOnRight=%d wallOnRight=%d speedLeft=%d speedRight=%d\n",
					      right,   ahead,   left,   (int)rightCond.getCondition(),   (int)aheadCond.getCondition(),   (int)leftCond.getCondition(),   action.prevWallOnRight,   action.wallOnRight,   action.speedLeft,   action.speedRight);

			pause(250);

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
