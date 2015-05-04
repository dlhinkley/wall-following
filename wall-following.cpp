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
		scanCogInit = 0,
		keyboardCogInit = 0;
int initialized = 0;

unsigned int sstack[40 + 20]; // If things get weird make this number bigger!
unsigned int kstack[40 + 30]; // If things get weird make this number bigger!


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
		//dprint(term,"Ping %d = %d\n",i,ping);
		//sumsq+= data[i] * data[i];
	}
	int mean = sum / qty;
	//int varience = sumsq / size - mean * mean;
	//dprint(term,"The mean is: %d\n\n", mean);
	//printf("Variance is: %d\n", varience);
	//  printf("Returned mean= %d\n", mean);
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

/*
 * Run keyboard polling in a seperate cog to prevent other actions from blocking keystrokes
 */
void keyboardCog(void *par) {

	dprint(term,"DEBUG:keyboardCog Start\n");

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

		//if ( c == 'x' || c == 't' || c == 's' || c == 'r' || c  == 'l' || c == 'b' || c == 'f' )  dprint(term,"DEBUG:Command c=%d\n",c);

		//    calcCoordinates();  // Check for ticks

		keyboardCogInit = 1; // Cog initilized
	}
	dprint(term,"DEBUG:keyboardCog End\n");
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

class Condition {




	public:
		enum CondValue { WAY_TOO_CLOSE = 0, JUST_RIGHT, WAY_TOO_FAR, NONE };
		CondValue condition;
		Condition();
		void setCondition(int);
		int distance;
};
Condition::Condition() {
	this->condition = NONE;
}

void Condition::setCondition(int distance) {

	this->distance = distance;
	// 6 | 0 - 2
	if ( this->distance  >= 0 && this->distance  <= TARGET_WALL_DIST  ) {

		this->condition  = this->JUST_RIGHT;
	}
	// Buffer either way depending on last condition to prevent flapping quickly
	// 6 | 3 - 4
	else if ( this->distance  >= TARGET_WALL_DIST + 1  && this->distance  <= TARGET_WALL_DIST + 2 ) {

		if ( this->condition != this->JUST_RIGHT ) {

			this->condition  = this->WAY_TOO_FAR;
		}
		else {

			this->condition = this->JUST_RIGHT;
		}
	}

	// 6 | 10 - ~
	else if ( this->distance  >= TARGET_WALL_DIST + 3  ) {

		this->condition  = this->WAY_TOO_FAR;
	}
	//dprint(term,"setCondition distance=%d condition=%d\n",distance,this->condition);
}
class Action {
	int prevSpeedRight;
	int prevSpeedLeft;
	int speed;
	int speedMax;
	int speedSlow;
	enum ActionCommands {RIGHT_WAY_CLOSE = 0, RIGHT_JUST_RIGHT, RIGHT_WAY_FAR, INIT };

	void turnRightCorner(  );
	void driveSpeed( int, int);
	void driveGoto( int, int);

public:
	Action();
	void trapped();
	void right90();
	void left90();
	void backward();
	void forward();
	void backUp90Right();
	void rightJustRight();
	void leftJustRight();
	void leftWayClose();
	void rightWayFar();
	void rightWayClose();
	void action();
	void drive();
	void goLeft();
	void goStop();
	void goRight();
	void goBackward();
	void goForward();
	void setDistances(int,int,int);
	int speedLeft;
	int speedRight;
	int left;
	int right;
	int ahead;
	int correct;

	Location loc;
	ActionCommands prevCmd;

};
Action::Action() {

	this->speedLeft = 0;
	this->speedRight = 0;
	this->prevSpeedRight = 0;
	this->prevSpeedLeft = 0;
	this->correct = 4;
	this->speed = 20;
	this->speedMax = 40;
	this->speedSlow = 5;
	this->prevCmd = INIT;
}
/**
 * Save the distances since they may change while correcting
 */
void Action::setDistances(int left, int ahead, int right) {

	this->left = left;
	this->ahead = ahead;
	this->right = right;
}
void Action::goLeft() {
	driveSpeed(-this->speed, this->speed);
}
void Action::goStop() {
	driveSpeed(0, 0);
}
void Action::goRight() {
	driveSpeed(this->speed, -this->speed);
}
void Action::goBackward() {
	driveSpeed(-this->speed, -this->speed);
}
void Action::goForward() {
	driveSpeed(this->speed, this->speed);
}
void Action::driveGoto(int left,int right) {

	//if ( left != prevSpeedLeft || right != prevSpeedRight ) {

		drive_goto(left, right);
	//	prevSpeedLeft = left;
	//	prevSpeedRight = right;

	//dprint(term,"DEBUG:new driveGoto left=%d right=%d\n", left, right);
	//}
	//loc.update();
}
void Action::driveSpeed(int left,int right) {

	if ( left != this->prevSpeedLeft || right != this->prevSpeedRight ) {

		drive_speed(left, right);
		this->prevSpeedLeft = left;
		this->prevSpeedRight = right;

		//dprint(term,"DEBUG:new driveSpeed left=%d right=%d\n", left, right);
	}
	//loc.update();
}
void Action::trapped() {

	// stop
	this->speedRight = 0;
	this->speedLeft  = 0;
}
void Action::right90() {

	driveGoto(26, -25);
}
void Action::left90() {

	driveGoto(-25, 26);
}
void Action::backUp90Right() {

	// backup until ahead is just right
	// turn 90 to right
	// drive forward

}
void Action::backward() {

	driveSpeed(-this->speed, -this->speed);
}
void Action::forward() {

	driveSpeed(this->speed, this->speed);
}

void Action::turnRightCorner(  ) {

	// Go straight until past corner
	int dist = 0.325 * TARGET_WALL_DIST + 50;

	driveGoto(dist,dist);

	// Turn right 90 degrees
	right90();

	// Go straight to go past edge
	driveGoto( 50, 50);

	// Keep going straigt
	forward();

}

void Action::leftJustRight() {

	// correct based on dynamic adjustment right or left of right
	// 15 - 14 = 1 too close so left should go faster
	// 15 - 16 = -1 too far so left should go slower
	int adj = (TARGET_WALL_DIST - this->left) ;

	driveSpeed(this->speed + adj, this->speed);
}
void Action::leftWayClose() {

	driveSpeed(this->speed + this->correct,this-> speed);
}


void Action::rightJustRight() {

	// correct based on dynamic adjustment right or left of right
	// 15 - 14 = 1 too close so left should go faster
	// 15 - 16 = -1 too far so left should go slower

//	if ( this->prevCmd == this->RIGHT_WAY_CLOSE ) {
//
//		driveSpeed(this->speed + adj, this->speed - adj);
//	}
//	else {

//	}
	int adj = (TARGET_WALL_DIST - this->right) * 2;
	driveSpeed(this->speed - adj, this->speed + adj);

	this->prevCmd = this->RIGHT_JUST_RIGHT;
}

void Action::rightWayClose() {

//	if ( this->prevCmd == this->RIGHT_WAY_CLOSE ) {
//
//		forward();
//	}
//	else {
//
//		driveGoto(-this->correct, this->correct); // 10 degrees
//	}
	int adj = (TARGET_WALL_DIST - this->right) ;
	driveSpeed(this->speed - adj, this->speed);

	this->prevCmd = this->RIGHT_WAY_CLOSE;
}

void Action::rightWayFar( ) {

	int adj = (TARGET_WALL_DIST - this->right) * 3;
	driveSpeed(this->speed - adj, this->speed + adj);

//	if ( this->prevCmd == this->RIGHT_WAY_FAR ) {
//
//		forward();
//	}
//	else {
//
//		driveGoto(-this->correct, this->correct); // 10 degrees
//	}
	this->prevCmd = this->RIGHT_WAY_FAR;
}
void Action::action() {

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
	int scanCogPtr    = cogstart(&scanCog, NULL, sstack, sizeof(sstack));

	int keyboardCogPtr = cogstart(&keyboardCog, NULL, kstack, sizeof(kstack));

	dprint(term,"DEBUG:main Start\n");

	while ( scanCogInit == 0 || keyboardCogInit == 0 ) {

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

		if ( isWallFollowing   ) {

			leftCond.setCondition( left );
			aheadCond.setCondition( ahead );
			rightCond.setCondition( right );
			action.setDistances(leftCond.distance, aheadCond.distance, rightCond.distance);

/*
			if ( leftCond.condition == Condition::WAY_TOO_CLOSE ) {

				if ( aheadCond.condition == Condition::WAY_TOO_CLOSE ) {

					if ( rightCond.condition == Condition::WAY_TOO_CLOSE ) action.trapped();
					if ( rightCond.condition == Condition::JUST_RIGHT    ) action.trapped();
					if ( rightCond.condition == Condition::WAY_TOO_FAR   ) action.backUp90Right();
				}

				else if ( aheadCond.condition == Condition::JUST_RIGHT ) {

					if ( rightCond.condition == Condition::WAY_TOO_CLOSE ) action.trapped();
					if ( rightCond.condition == Condition::JUST_RIGHT    ) action.trapped();
					if ( rightCond.condition == Condition::WAY_TOO_FAR   ) action.backUp90Right();
				}

				else if ( aheadCond.condition == Condition::WAY_TOO_FAR ) {

					if ( rightCond.condition == Condition::WAY_TOO_CLOSE ) action.trapped();
					if ( rightCond.condition == Condition::JUST_RIGHT    ) action.trapped();
					if ( rightCond.condition == Condition::WAY_TOO_FAR   ) action.leftWayClose();
				}
			}
			else if ( leftCond.condition == Condition::JUST_RIGHT ) {

				if ( aheadCond.condition == Condition::WAY_TOO_CLOSE ) {

					if ( rightCond.condition == Condition::WAY_TOO_CLOSE ) action.trapped();
					if ( rightCond.condition == Condition::JUST_RIGHT    ) action.trapped();
					if ( rightCond.condition == Condition::WAY_TOO_FAR   ) action.backUp90Right();
				}

				else if ( aheadCond.condition == Condition::JUST_RIGHT ) {

					if ( rightCond.condition == Condition::WAY_TOO_CLOSE ) action.trapped();
					if ( rightCond.condition == Condition::JUST_RIGHT    ) action.trapped();
					if ( rightCond.condition == Condition::WAY_TOO_FAR   ) action.backUp90Right();
				}

				else if ( aheadCond.condition == Condition::WAY_TOO_FAR ) {

					if ( rightCond.condition == Condition::WAY_TOO_CLOSE ) action.backward();
					if ( rightCond.condition == Condition::JUST_RIGHT    ) action.right90();
					if ( rightCond.condition == Condition::WAY_TOO_FAR   ) action.rightWayFar(  );
				}
			}
			 if ( leftCond.condition == Condition::WAY_TOO_FAR ) {

				if ( aheadCond.condition == Condition::WAY_TOO_CLOSE ) {

					if ( rightCond.condition == Condition::WAY_TOO_CLOSE ) action.backward();
					if ( rightCond.condition == Condition::JUST_RIGHT    ) action.backward();
					if ( rightCond.condition == Condition::WAY_TOO_FAR   ) action.backward();
				}

				else if ( aheadCond.condition == Condition::JUST_RIGHT ) {

					if ( rightCond.condition == Condition::WAY_TOO_CLOSE ) action.backward();
					if ( rightCond.condition == Condition::JUST_RIGHT    ) action.left90();
					if ( rightCond.condition == Condition::WAY_TOO_FAR   ) action.rightWayFar(  );
				}


 */
//				 if ( aheadCond.condition == Condition::WAY_TOO_FAR ) {
//
					if ( rightCond.condition == Condition::WAY_TOO_CLOSE ) action.rightWayClose();
					if ( rightCond.condition == Condition::JUST_RIGHT    ) action.rightJustRight();
					if ( rightCond.condition == Condition::WAY_TOO_FAR   ) action.rightWayFar(  );
//				}

//			}


//			dprint(term, "x=%f y=%f heading=%f right=%d ahead=%d left=%d rightCond=%d aheadCond=%d leftCond=%d prevWallOnRight=%d wallOnRight=%d speedLeft=%d speedRight=%d\n",
//					      action.loc.x, action.loc.y, action.loc.heading, right,   ahead,   left,   (int)rightCond.condition,   (int)aheadCond.condition,   (int)leftCond.condition,   action.prevWallOnRight,   action.wallOnRight,   action.speedLeft,   action.speedRight);
			dprint(term, "STATUS:right=%d\tahead=%d\tleft=%d\trightCond=%d\taheadCond=%d\tleftCond=%d\tprevCmd=%d\tspeedLeft=%d\tspeedRight=%d\n",
					      rightCond.distance,   aheadCond.distance,   leftCond.distance,  (int)rightCond.condition,   (int)aheadCond.condition,   (int)leftCond.condition,   action.prevCmd,   action.speedLeft,   action.speedRight);

			pause(250);

		}
		//printf("Angle=%d Distance=%d\n", scanAngle[scanPtr], scanPing[scanPtr]);

	}
	dprint(term,"END\n");
	dprint(term,"DEBUG:main follow wall stop\n");

	servo_stop();                               // Stop servo process
	drive_goto(0,0);
	cogstop( scanCogPtr );
	cogstop( keyboardCogPtr );

	dprint(term,"DEBUG:main End\n");
	return 0;
} 
