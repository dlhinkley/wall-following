/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials
*/
//#include "simpletools.h"                      // Include simple tools
//#include "ping.h"                      // Include simple tools
//#include "servo.h"                      // Include simple tools
//#include "abdrive.h"

terminal *term;                               // For full duplex serial terminal


int PINGA = 0,
    PINGL = 16,
    PINGR = 17;

double PI =  3.14159265359;

static volatile int ahead = 0,
                    left = 0,
                    right = 0,
                    isRunning = 1,
                    isWallFollowing = 0,
                    scanCogInitA = 0,
                    scanCogInitL = 0,
                    scanCogInitR = 0,
                    keyboardCogInit = 0;

int speed = 15;
int speedSlow = 5;
int minWallDist = 10;

unsigned int sstackA[256]; // If things get weird make this number bigger!
unsigned int sstackL[256]; // If things get weird make this number bigger!
unsigned int sstackR[256]; // If things get weird make this number bigger!
unsigned int kstack[256]; // If things get weird make this number bigger!

int ticksLeft, ticksRight, ticksLeftOld, ticksRightOld;
static double trackWidth, distancePerCount;

static volatile double heading = 0.0, x = 0.0, y = 0.0, degHeading;


void goLeft();
void goStop();
void goRight();
void goBackward();
void goForward();
int getPing(int);
void keyboardCog(void *par);
void findWall();

void calcCoordinates(void);
void scanCogA(void *par); // Use a cog to fill range variables with ping distances
void scanCogL(void *par); // Use a cog to fill range variables with ping distances
void scanCogR(void *par); // Use a cog to fill range variables with ping distances

int main(){

  simpleterm_close();                         // Close default same-cog terminal
  term = fdserial_open(31, 30, 0, 115200);    // Set up other cog for terminal

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

  int speedLeft = speed;
  int speedRight = speed;
  int adjustLeft = 0;
  int adjustRight = 0;
  int prevErrorRight = 0;
  int prevErrorLeft = 0;
  int lastWallDistDiff = 0;

  // Keep running
  while( isRunning ) {
    //dprint(term,"scanCog left=%d ahead=%d right=%d\n", left, ahead, right);

    if ( isWallFollowing   ) {


      int wayTooCloseLeft  = ( left  >= 0 && left  <= minWallDist - 7 ); // 15 | 5 - 8
      int wayTooCloseRight = ( right >= 0 && right <= minWallDist - 7 ); // 15 | 5 - 8
      int wayTooCloseAhead = ( ahead >= 0 && ahead <= minWallDist - 7 ); // 15 | 5 - 8

      int lilTooCloseLeft  = ( left  >= minWallDist - 6 && left  <= minWallDist - 3 ); // 15 | 9 - 12
      int lilTooCloseRight = ( right >= minWallDist - 6 && right <= minWallDist - 3 ); // 15 | 9 - 12
      int lilTooCloseAhead = ( ahead >= minWallDist - 6 && ahead <= minWallDist - 3 ); // 15 | 9 - 12

      int justRightLeft  = ( left  >= minWallDist - 2 && left  <= minWallDist + 2 ); // 15 | 13 - 17
      int justRightRight = ( right >= minWallDist - 2 && right <= minWallDist + 2 ); // 15 | 13 - 17
      int justRightAhead = ( ahead >= minWallDist - 2 && ahead <= minWallDist + 2 ); // 15 | 13 - 17

      int lilTooFarLeft  = ( left  >= minWallDist + 3 && left  <= minWallDist + 6 ); // 15 | 18 - 21
      int lilTooFarRight = ( right >= minWallDist + 3 && right <= minWallDist + 6 ); // 15 | 18 - 21
      int lilTooFarAhead = ( ahead >= minWallDist + 3 && ahead <= minWallDist + 6 ); // 15 | 18 - 21

      int wayTooFarLeft  = ( left  >= minWallDist + 7  ); // 15 | 22 - ~
      int wayTooFarRight = ( right >= minWallDist + 7  ); // 15 | 22 - ~
      int wayTooFarAhead = ( ahead >= minWallDist + 7  ); // 15 | 22 - ~

      // Turn left if about to ht the wall
      //
      if ( ahead <= minWallDist ) {

        speedLeft = -speed;
        speedRight = speed;
        //dprint(term,"main straight\n");
      }
      // If right is farther than the min distance away
      else if ( right > minWallDist ) {

        // If it's just a little off, make a minor correction
        if ( right < minWallDist * 2 ) {

          speedRight = speed - (right - minWallDist );
          speedLeft = speed;
          //dprint(term,"main light right\n");
        }
        // Otherwise it's a corner, turn sharp
        else {

          // Don't speed
          drive_setMaxSpeed( speed );

          // Go straight until past corner
          int dist = 0.325 * minWallDist + 50;

          drive_goto(dist,dist);

          // Turn right 90 degrees
          drive_goto(26, -25);

          // Go straight to go past edge
          drive_goto( 50, 50);

          // Keep going straigt
          speedRight = speed;
          speedLeft = speed;
          //dprint(term,"main sharp right\n");
        }

      }
      // If too far right turn slightly left
      else if ( right <= minWallDist ) {

        speedLeft = speed - (minWallDist - right );
        //dprint(term,"main too close\n");
      }
      // If room to go forward
      else if ( ahead > minWallDist ) {

        speedLeft = speed;
        speedRight = speed;
        //dprint(term,"main forward\n");
      }


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
/*
* Drive to find the best wall
*/
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
void scanCogA(void *par) {

  dprint(term,"scanCogA Start\n");

  while( isRunning ) {

    ahead = getPing(PINGA);
    scanCogInitA = 1; // scan Cog initilized
  }
  dprint(term,"scanCogA End\n");
}
void scanCogL(void *par) {

  dprint(term,"scanCogL Start\n");

  while( isRunning ) {

    left = getPing(PINGL);
    scanCogInitL = 1; // scan Cog initilized
  }
  dprint(term,"scanCogL End\n");
}
void scanCogR(void *par) {

  dprint(term,"scanCogR Start\n");

  while( isRunning ) {

    right = getPing(PINGR);
    scanCogInitR = 1; // scan Cog initilized
  }
  dprint(term,"scanCogR End\n");
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

    if ( c == 'x' ) isRunning = 0;
    if ( c == 'w' ) isWallFollowing = !isWallFollowing; // If 'w', toggle on/off

    if ( c == 'x' || c == 't' || c == 's' || c == 'r' || c  == 'l' || c == 'b' || c == 'f' )  dprint(term,"Command c=%d\n",c);

    calcCoordinates();  // Check for ticks

    keyboardCogInit = 1; // Cog initilized
  }
  dprint(term,"keyboardCog End\n");
}
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
