/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/



#include <Servo.h>
// 
Servo bottom180;  // create Servo object to control a servo
Servo leftarm;
Servo rightarm;
int pos = 0;  //initial servo posisiton

//iltrasonic
long duration;
int distance;
const int trigPin = 13;
const int echoPin = 12;


void setup() {
//servos
  leftarm.attach(11);  // attaches the servo on pin 9 to the Servo object
  rightarm.attach(10);
  bottom180.attach(9);

  }

//ultrasonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); //so this part tell when it feels something echo and then it will give output as trigger that so then it will give out the output as it is
  Serial.begin(9600); // so serial.begin actually worls as how fast it transfers data per second , 9600 max
  pinMode(13, OUTPUT);


void loop() {


//ultrasonic calculating distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

//bottom servo 180 rotate
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    bottom180.write(pos);              // tell servo to go to position in variable 'pos'
    delay(12);                       // waits 15 ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    bottom180.write(pos);              // tell servo to go to position in variable 'pos'
    delay(12);                       // waits 15 ms for the servo to reach the position

  if(distance < 10) {
    break;
  }
  else {
  ser.write(pos);
  delay(3);
  servo2.write(140);
  }
}

// if else








}
