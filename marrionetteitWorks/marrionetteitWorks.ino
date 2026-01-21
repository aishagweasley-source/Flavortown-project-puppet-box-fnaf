#include <Servo.h>

// ===== PIN ===== // we define the things we need and also the pins we use
#define TRIG_PIN 13
#define ECHO_PIN 12

#define SERVO_BADAN 9
#define SERVO_RIGHT 10
#define SERVO_LEFT 11

// ===== OBJEK SERVO ===== // we basically tells how many and what servos we using and name it
Servo servoBadan;
Servo servoRight;
Servo servoLeft;

// ===== VARIABEL ===== // this is ulstrasonic variables
long duration;
int distance;

int sudutBadan = 90; // variable for sudut badan awal
bool arah = true; // telling to move arah right

// how we want the things to work at first
void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

// telling that these are servos we use(attach) and the names we use to define them
  servoBadan.attach(SERVO_BADAN);
  servoRight.attach(SERVO_RIGHT);
  servoLeft.attach(SERVO_LEFT);

  servoBadan.write(90);
  servoRight.write(0);
  servoLeft.write(0);

  Serial.begin(9600); // how fast we want things to work
}

void loop() {
  distance = bacaUltrasonik(); // remember what ( = ) means

  Serial.print("Jarak: ");
  Serial.println(distance); // we want the baca ultrasonic to be printed in the output 

  if (distance > 0 && distance < 5) { // if distace is in between 0-5, do that
    // MODE NGEJUTIN
    servoBadan.write(90);
    servoRight.write(90);
    servoLeft.write(90);
  } 
  else {
    // MODE IDLE // 
    servoRight.write(0);
    servoLeft.write(0);
    gerakBadan();
  }

  delay(30);
}

// ===== FUNGSI ULTRASONIK =====
int bacaUltrasonik() {    // basically hpw to count distance
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 999;

  return duration * 0.034 / 2;
}

// ===== GERAK BADAN KIRI-KANAN =====
void gerakBadan() {
  if (arah) {
    sudutBadan += 2;
    if (sudutBadan >= 120) arah = false;
    // making it go right with anglesnya 2' at a time, till its on angle 120 AND then it will turn left
  } 
  else {
    sudutBadan -= 2;
    if (sudutBadan <= 60) arah = true;
    // it iwl go 2 angle at a time until its on angle 60 THEN it will turn right
  }

  servoBadan.write(sudutBadan); // yeah.. that. 
}