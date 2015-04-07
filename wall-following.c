/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"                      // Include simple tools
#include "ping.h"                      // Include simple tools
#include "servo.h"                      // Include simple tools
#include "abdrive.h"

terminal *term;                               // For full duplex serial terminal
char c = 0;                                   // Stores character input


int TUR = 17,       // Turet pin
    PING = 16;      // Ping pin

int scanDirection = 1;  // Direction of scan
int scanPtr = 1;        // Nth angle being scanned

int scanAngle[] = {0 ,900, 1800}; // Angles for the turet to point
int scanPing[] = {0 ,0, 0}; // Holds the distance for each direction

int leftPtr = 0;      // Angle for left
int aheadPtr = 1; // Angle for straight ahead
int rightPtr = 2;  // Angle for right

int isTurningLeft = 0;
int isTuretOn = 0;

int speed = 16;
int minWallDist = 30;

unsigned int tstack[256]; // If things get weird make this number bigger!
unsigned int kstack[256]; // If things get weird make this number bigger!

void goLeft();
void goStop();
void goRight();
void goBackward();
void goForward();
int getPing();
void scanStep();
void keyboardCog(void *par);

void turetCog(void *par); // Use a cog to fill range variables with ping distances

int main(){

 
  // Start the ping cog
  cogstart(&turetCog, NULL, tstack, sizeof(tstack));
  cogstart(&keyboardCog, NULL, kstack, sizeof(kstack));

  simpleterm_close();                         // Close default same-cog terminal

  drive_setRampStep(10);                      // 10 ticks/sec / 20 ms

  //drive_ramp(128, 128);                       // Forward 2 RPS

  int correction = 0;

  // Keep running
  while(1) {

    int ahead = scanPing[ aheadPtr ];
    int left = scanPing[ leftPtr ];
    int right = scanPing[ rightPtr ];



    // Stop if something staight in front
    //
    if (  ahead <= 15 ) {

      goLeft();
      isTurningLeft = 1;
    }

    else if ( isTurningLeft == 1 && scanPing[ rightPtr ] > 15 ) {

      goForward();
      isTurningLeft = 0;
    }

    //printf("Angle=%d Distance=%d\n", scanAngle[scanPtr], scanPing[scanPtr]);


  }
  servo_stop();                               // Stop servo process

  return 0;
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

  while(1) {

    if ( isTuretOn == 1 ) {
      scanStep();
      pause(100);
    }
    else {

      servo_angle(TUR, 900);  // Turn the servo

    }
  }
}

void keyboardCog(void *par) {

  term = fdserial_open(31, 30, 0, 115200);    // Set up other cog for terminal

  while(1) {
    // Handle any keystroke navigation
    c = fdserial_rxTime(term, 50);            // Get character from terminal
    if(c == 'f') goForward();         // If 'f' then forward
    if(c == 'b') goBackward();       // If 'b' then backward
    if(c == 'l') goLeft();        // If 'l' then left
    if(c == 'r') goRight();        // If 'r' then right
    if(c == 's') goStop();           // If 's' then stop
    if(c == 't' ) {

      if ( isTuretOn == 0 ) {
          isTuretOn = 1;
      }
      else {

        isTuretOn = 0;
      }
    }
  }
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
* Scan an angle
*/
void scanStep() {

  int numAngles = 3;

  
  // Set next angle
	// Change the position of the turet
  int angle = scanAngle[ scanPtr ];

  servo_angle(TUR, angle);  // Turn the servo
  scanPing[ scanPtr ] = getPing();  // Save the ping


  // If the ptr is greater than the number of positions, go the other way
  if ( scanPtr + scanDirection == numAngles) {

    high(26);                   
    low(27);
    scanDirection = -1;
  }
  // If the ptr is less than 0, go the other way
  else if ( scanPtr + scanDirection < 0 ) {

    high(27);                   
    low(26);
    scanDirection = 1;
  }   
  else {

    high(27);                   
    high(26); 
  }                              
  scanPtr += scanDirection;

}




