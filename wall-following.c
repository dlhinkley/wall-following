/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"                      // Include simple tools
#include "ping.h"                      // Include simple tools
#include "servo.h"                      // Include simple tools
#include "abdrive.h"

terminal *term;                               // For full duplex serial terminal


int TUR = 17,       // Turet pin
    PING = 16;      // Ping pin

static volatile int ahead = 0, 
                    left = 0, 
                    right = 0,
                    leftMid = 0, 
                    rightMid = 0,
                    isRunning = 1,
                    turetCogInit = 0,
                    keyboardCogInit = 0;

int isTurningLeft = 0;
int isTuretOn = 0;

int speed = 16;
int speedSlow = 5;
int minWallDist = 20;

unsigned int tstack[256]; // If things get weird make this number bigger!
unsigned int kstack[256]; // If things get weird make this number bigger!

void goLeft();
void goStop();
void goRight();
void goBackward();
void goForward();
int getPing();
void keyboardCog(void *par);
void findWall();

void turetCog(void *par); // Use a cog to fill range variables with ping distances

int main(){

  simpleterm_close();                         // Close default same-cog terminal
  term = fdserial_open(31, 30, 0, 115200);    // Set up other cog for terminal
 
  // Start the ping cog
  int turetCogPtr    = cogstart(&turetCog, NULL, tstack, sizeof(tstack));
  int keyboardCogPtr = cogstart(&keyboardCog, NULL, kstack, sizeof(kstack));

  dprint(term,"main Start\n"); 

  while ( turetCogInit == 0 || keyboardCogInit == 0 ) {

    // Wait for cogs to initialize
    pause(500);
    dprint(term,"main Waiting for Cog initialization\n"); 
  }
  dprint(term,"main Cog initialized\n"); 

  drive_setRampStep(10);                      // 10 ticks/sec / 20 ms

  //drive_ramp(128, 128);                       // Forward 2 RPS

  int correction = 0;

  findWall();

  dprint(term,"main follow wall start\n"); 

  // Keep running
  while( isRunning ) {

    if ( isTuretOn == 1 ) {

/*
turetCog wait left=46 ahead=19 right=16
turetCog wait left=46 ahead=36 right=16
turetCog wait left=46 ahead=36 right=15
turetCog wait left=46 ahead=17 right=15
turetCog wait left=44 ahead=17 right=15

*/

      // Turn left if about to ht the wall
      //
      if ( ahead <= minWallDist ) {
  
        goLeft();
      }
      // If too far away turn to wall plus buffer
      else if ( right > minWallDist + 3 ) {

        goRight();
      }
      // If too close to wall plus buffer
      else if ( right <= minWallDist - 3 ) {

        goLeft();
      }
      // If room to go forward
      else if ( ahead > minWallDist ) {
  
        goForward();
        //isTurningLeft = 0;
      }
    }

    //printf("Angle=%d Distance=%d\n", scanAngle[scanPtr], scanPing[scanPtr]);
  }
  dprint(term,"main follow wall stop\n"); 

  servo_stop();                               // Stop servo process
  drive_goto(0,0);
  cogstop( turetCogPtr );
  cogstop( keyboardCogPtr );

  dprint(term,"main End\n"); 
  return 0;
}
/*
* Drive to find the best wall
*/
void findWall() {

  dprint(term,"findWall Start\n"); 
  dprint(term,"findWall ahead=%d minWallDist=%d\n", ahead, minWallDist ); 

  // Turn turet on
  isTuretOn = 1;

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
* Runs in cog
*/
void turetCog(void *par) {

  dprint(term,"turetCog Start\n"); 

  int scanDirection = 1;  // Direction of scan
  int scanPtr = 2;        // Nth angle being scanned
  
/*
  int scanAngle[] = {0 ,450, 900, 1350, 1800}; // Angles for the turet to point
  int scanPing[] = {0 ,0, 0, 0, 0}; // Holds the distance for each direction
  
  int rightPtr    = 0; // Angle for right
  int rightMidPtr = 1; // Angle for right mid
  int aheadPtr    = 2; // Angle for straight ahead
  int leftMidPtr  = 3; // Angle for left mid
  int leftPtr     = 4; // Angle for left
*/
  int scanAngle[] = {0, 900, 1800}; // Angles for the turet to point
  int scanPing[] = {0, 0, 0}; // Holds the distance for each direction
  
  int rightPtr    = 0; // Angle for right
  int aheadPtr    = 1; // Angle for straight ahead
  int leftPtr     = 2; // Angle for left

  while( isRunning ) {

    // If the turret is on, then scan
    //
    if ( isTuretOn == 1 ) {

      int numAngles = 3;
    
      // Set next angle
      // Change the position of the turet
      int angle = scanAngle[ scanPtr ];
    
      servo_angle(TUR, angle);  // Turn the servo
      scanPing[ scanPtr ] = getPing();  // Save the ping
    
    
      // If the ptr is greater than the number of positions, go the other way
      if ( scanPtr + scanDirection == numAngles) {

        scanDirection = -1;
      }
      // If the ptr is less than 0, go the other way
      else if ( scanPtr + scanDirection < 0 ) {

        scanDirection = 1;
      }   
      scanPtr += scanDirection;


      ahead    = scanPing[ aheadPtr ];
      left     = scanPing[ leftPtr ];
      right    = scanPing[ rightPtr ];
      //leftMid  = scanPing[ leftMidPtr ];
      //rightMid = scanPing[ rightMidPtr ];
      pause(50);
      //dprint(term,"turetCog wait left=%d leftMid=%d ahead=%d rightMid=%d right=%d\n", left, leftMid, ahead, rightMid, right ); 
      dprint(term,"turetCog wait left=%d ahead=%d right=%d\n", left, ahead, right ); 
    }
    else {

      servo_angle(TUR, 900);  // Turn the servo
      ahead = getPing();
      right     = 0;
      left      = 0;
      //rightMid  = 0;
      //leftMid   = 0;
    }
    turetCogInit = 1; // Turet Cog initilized
  }
  dprint(term,"turetCog End\n"); 
}
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
    if ( c == 't' ) {

      if ( isTuretOn == 0 ) {
          isTuretOn = 1;
      }
      else {

      isTuretOn = 0;
      }
    }
    if ( c == 'x' ) isRunning = 0;

    if ( c == 'x' || c == 't' || c == 's' || c == 'r' || c  == 'l' || c == 'b' || c == 'f' )  dprint(term,"Command c=%d\n",c); 

    keyboardCogInit = 1; // Cog initilized
  }
  dprint(term,"keyboardCog End\n"); 
}
/**
* Retrieves a ping by doing several pings, then getting an average.  The ping from time to time will get
* a ping with a huge error
*/
int getPing() {

  int qty = 5;
  int sum = 0;     // stores sum of elements
  //int sumsq = 0; // stores sum of squares

  for ( int i = 0; i < qty; i++ ) {

    int ping = ping_cm( PING );
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
 * Calculates the coordinates and heading of the robot based on the ticks from the servos
 */
void calcCoordinates(void) {

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
	if (heading > PI) {
		heading -= 2.0 * PI;
	} else {
		if (heading <= -PI) {
			heading += 2.0 * PI;
		}
	}
   degHeading = heading * (180 / PI);
  if (degHeading < 0) degHeading += 360;

}

