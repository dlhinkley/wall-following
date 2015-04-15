/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
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
                    scanCogInit = 0,
                    keyboardCogInit = 0;

int speed = 15;
int speedSlow = 5;
int minWallDist = 15;

unsigned int sstack[256]; // If things get weird make this number bigger!
unsigned int kstack[256]; // If things get weird make this number bigger!

void goLeft();
void goStop();
void goRight();
void goBackward();
void goForward();
int getPing(int);
void keyboardCog(void *par);
void findWall();

void scanCog(void *par); // Use a cog to fill range variables with ping distances

int main(){

  simpleterm_close();                         // Close default same-cog terminal
  term = fdserial_open(31, 30, 0, 115200);    // Set up other cog for terminal
 
  // Start the ping cog
  int scanCogPtr    = cogstart(&scanCog, NULL, sstack, sizeof(sstack));
  int keyboardCogPtr = cogstart(&keyboardCog, NULL, kstack, sizeof(kstack));

  dprint(term,"main Start\n"); 

  while ( scanCogInit == 0 || keyboardCogInit == 0 ) {

    // Wait for cogs to initialize
    pause(500);
    dprint(term,"main Waiting for Cog initialization\n"); 
  }
  dprint(term,"main Cog initialized\n"); 

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


  // Keep running
  while( isRunning ) {

/*
scanCog left=40 ahead=19 right=12
scanCog left=40 ahead=19 right=12
scanCog left=40 ahead=19 right=12
scanCog left=41 ahead=19 right=11
scanCog left=34 ahead=15 right=9
scanCog left=40 ahead=10 right=9
Command c=115
scanCog left=29 ahead=5 right=8
scanCog left=29 ahead=3 right=8
scanCog left=29 ahead=3 right=8
scanCog left=29 ahead=3 right=8
*/
    
    if ( isWallFollowing   ) {
  
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

          speedRight = speed - (right - minWallDist);
          speedLeft = speed;
          //dprint(term,"main light right\n"); 
        }
        // Otherwise turn sharp
        else {

          speedRight = speed;
          speedLeft = speed + (speed / 2);
          //dprint(term,"main sharp right\n"); 
        }
        
      }
      // If too far right turn slightly left
      else if ( right <= minWallDist ) {
                            
        speedLeft = speed - (minWallDist - right);        
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
  cogstop( scanCogPtr );
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
void scanCog(void *par) {

  dprint(term,"scanCog Start\n"); 

  while( isRunning ) {

    ahead = getPing(PINGA);
    right     = getPing(PINGR);;
    left      = getPing(PINGL);;
    scanCogInit = 1; // scan Cog initilized
    dprint(term,"scanCog left=%d ahead=%d right=%d\n", left, ahead, right); 

    pause(100);// Keep the print out from getting over run
  }
  dprint(term,"scanCog End\n"); 
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





