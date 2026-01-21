/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/


// servo variables
#include <Servo.h>
Servo servo1;
Servo servo2;  
// create Servo object to control a servo
// twelve Servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {


//servo 
  servo1.attach(9);  // attaches the servo on pin 9 to the Servo object
  servo2.attach(10);
}

void loop() {

if(distance < 10 ) {
  servo2
}


  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo1.write(pos);              // tell servo to go to position in variable 'pos'
    servo2.write(pos);
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo1.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  // all this basically to make it rotate smoothly with controlling the speed
  }

if(distance <10 ) {
  

  servo2.write(180);
  delay(3000);
  servo2.write(-180);
  delay(3000);
}




}



