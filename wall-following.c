/*
  Blank Simple Project.c
  http://learn.parallax.com/propeller-c-tutorials 
*/
#include "simpletools.h"                      // Include simple tools
#include "ping.h"                      // Include simple tools


int TUR = 17,       // Turet pin
    PING = 16;      // Ping pin





int main(){

  int ping;

  //calcPing();
  ping = getPing();
/*
  int cmDist = 0;

  // Add your code here
  servo_angle(TUR, 0);                         // P16 servo to 0 degrees

  getPing();

  pause(300);                                // ...for 3 seconds
  servo_angle(TUR, 450);                         // P16 servo to 0 degrees

  getPing();

  pause(300);                                // ...for 3 seconds
  servo_angle(TUR, 900);                         // P16 servo to 0 degrees

  getPing();

                              // ...for 3 seconds
  servo_stop();                               // Stop servo process
*/
  return 0;
}
 
int getPing() {

  int size = 5;
  //int data[5] = {27, 30, 26, 2, 60};

  int sum = 0;     // stores sum of elements
  //int sumsq = 0; // stores sum of squares
  for ( int i = 0; i < size; i++ ) {

    int ping = ping_cm( PING );
    sum += ping;
    printf("Ping %d = %d\n",i,ping);
    //sumsq+= data[i] * data[i];
  }
  int mean = sum / size; 
  //int varience = sumsq / size - mean * mean;
  //printf("The mean is: %d\n", mean);
  //printf("Variance is: %d\n", varience);
    printf("Returned mean= %d\n", mean);

  return mean;
}





