#include <Servo.h>

// ===== PIN CONFIGURATION =====
#define TRIG_PIN 6
#define ECHO_PIN 5

#define SERVO_BADAN 3
#define SERVO_RIGHT 2
#define SERVO_LEFT 4

#define LED_MATA_KIRI 8
#define LED_MATA_KANAN 9
#define LED_MERAH 10   // LED merah untuk mode angry
#define BUZZER_PIN 11  //CHANGE IT TO DF PLAYER

// ===== OBJEK SERVO =====
Servo servoBadan;
Servo servoRight;
Servo servoLeft;

// ===== VARIABEL SENSOR =====
long duration;
int distance;
int distancePrev = 999;  // I THINK ITS JUST AS AN INITIAL VALUE IF ITS SO FAR
                         // THEN IN THE VOID LOOP, WILL MAKE THIS DISTANCEPREV = DISTANCE
                         // SO WE CAN USE THIS VARB AS TO STORE THE ULTRASONIC VALUE IN THE FUNCTION ?? (PROLLY)

// ===== VARIABEL GERAKAN =====
int sudutBadan = 90;
bool arah = true;        // kanan
int kecepatanGerak = 2;  //turn 2 angle at a time

// ===== VARIABEL MODE & MOOD =====
enum Mood { IDLE,
            CURIOUS,
            ALERT,
            SCARE,
            ANGRY,
            TIRED };
Mood currentMood = IDLE;  // WERE GONNA SET IT ON THIS
int scareCounter = 0;     // TO COUNT HOW MANY TIMES THE FUNCTION HAS BEEN ACTIVATED (LIKE UDH BRP KALI AT THAT DISTANCE SO WE CAN RESET IT)

unsigned long lastScareTime = 0;   // IT STORES THE NUMBER OF TIME (IN MILLIS) OF WHEN THE LAST SCARE WAS ---- STORES LAST SCARE*TIME* MILLIS.
unsigned long moodChangeTime = 0;  // STORES (TIMES IN MILLIS ARDUINO) OF WHEN WAS THE LAST TIME MOOD CHANGES.

// ===== VARIABEL TIMING ===== // THESE ALL ARE TO STORE THE LAST TIME THE VARB WAS ACTIVATED
unsigned long lastBlinkTime = 0;
unsigned long lastSoundTime = 0;
unsigned long lastRandomMove = 0;
bool ledState = false;  // TO DETERMINE or *TO REMEMBER IF LED IS ON OR OFF*

void setup() {
  // PIN MODE
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_MATA_KIRI, OUTPUT);
  pinMode(LED_MATA_KANAN, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // SERVO ATTACH
  servoBadan.attach(SERVO_BADAN);
  servoRight.attach(SERVO_RIGHT);
  servoLeft.attach(SERVO_LEFT);

  // POSISI AWAL
  servoBadan.write(90);
  servoRight.write(0);
  servoLeft.write(0);

  Serial.begin(9600);

  // ANIMASI STARTUP
  startupAnimation();  // IT MEANS THAT THIS FUNCTION WILL RUN AUTOMATICALLY WHEN ARDUINO STARTS
}

void loop() {
  distance = bacaUltrasonik();  // SO.. DISTANCE WILL HAVE THE VALUE FROM bacaUltrasonik function (distance value)

  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.print(" cm | Mood: ");
  Serial.println(getMoodName());  // WILL PRINT THAT getMoodName function

  // UPDATE MOOD BERDASARKAN JARAK
  updateMood();  // THIS WILL RUN THE FUCNTION REGULARLY RIGHT AFTER THE ARDUINO STARTS

  // EKSEKUSI BEHAVIOR SESUAI MOOD
  executeMoodBehavior();  // THIS WILL RUN THE FUCNTION REGULARLY RIGHT AFTER THE ARDUINO STARTS

  // EFEK LED & SUARA ---- SO LED AND DF PLAYER RUNS TOGETHER *MODIFY THIS LATER*
  handleLEDEffects();  // THIS WILL RUN THE FUCNTION REGULARLY RIGHT AFTER THE ARDUINO STARTS

  distancePrev = distance;  // SO HERE, DISTANCEPREV WILL HAVE THE VALUE FROM THE DISTANCE FUNCTION RECEIVED (ULTRASNONIC DISTANCE MEASSUREMENT)
  delay(30);                //WHY HARUS PAKE DELAY?
}  // void loop closed

// ===== FUNGSI ULTRASONIK ===== - FUNCTIONS DOES NOT GO INTO VOID LOOP
int bacaUltrasonik() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 999;

  return duration * 0.034 / 2;
}

// ===== UPDATE MOOD SYSTEM =====
void updateMood() {
  unsigned long currentTime = millis();  // SO THERES A NEW VARB WHICH IS "currentTime" AND THE OUTPUT IS "millis"
                                         // THIS IS TO MEASURE THE TIME OF WHEN WAS THE LAST TIME MOOD WAS UPDATED (in millis)
  // ZONE DETECTION
  if (distance > 0 && distance < 5) {  // if distance 0 - 5
    // ZONA SANGAT DEKAT - SCARE MODE!
    if (currentMood != SCARE && currentMood != ANGRY) {  // IF CURRENTMOOD IS NOT SCARE OR ANGRY THEN DO...    "!" MEANS NOT/FALSE
      currentMood = SCARE;                               // MAKE/CHANGE CURRENTMOOD TO SCARE
      scareCounter++;                                    // TO STORE VALUE, "++" INCREASE VALUE BY 1
      lastScareTime = currentTime;                       // TO SAVE AT WHAT TIME WAS THE LAST SCARE -- BC IT USE currentTime = millis    , LAST TIME IT SCARES = THAT CURRENT TIME
      moodChangeTime = currentTime;                      // TO SAVE THE TIME , WHAT WAS THE LAST TIME MOOD CHANGES -- BC currentTime = millis  , LAST TIME MOOD CHANGED = CURRENT TIME

      // Kalau sudah 3x ketemu, jadi ANGRY
      if (scareCounter >= 3) {  // IF SCARECOUNTER UDH LEBIH DARI 3x
        currentMood = ANGRY;    // MAKE MOOD ANGRY
        scareCounter = 0;       // RESET SCARECOUNTER TO 0
        // BASICALLY.. IF SCARECOUNTER IS OVER or EQUAL TO 3 , CHANGE MOOD TO ANGRY , AND THEN RESET THE SCARECOUNTER BACK TO 0
      }
    }
  } else if (distance >= 5 && distance < 15) {  //IF THOSE UPPER FUNCTIONS ARE (IF) ARE FALSE, THEN RUN ELSE IF - // else if = if condition A, B, if's above is false/doesnt run
    // ZONA DEKAT - ALERT
    if (currentMood == IDLE || currentMood == CURIOUS) {  //IF IDLE OR CURIOUS IS TRUE = STILL RUN - // "||" YOU CAN STILL TUN THIS FUNCTION EVEN IF ONLY VARB A OR B IS TRUE
      currentMood = ALERT;                                // IF MOOD IS IDLE OR CURIOUS, CHANGE TO ALERT
      moodChangeTime = currentTime;
    }
  } else if (distance >= 15 && distance < 40) {  // IF FUNCTIONS ABOVE ARE FALSE, THEN RUN THIS IF DISTANCE 15-40  ,  ACTUALLY JUST SAYING THAT IF DISTANCE IS NOT LIKE THE FUNCTIONS ABOVE BUT LIKE THIS ONE THEN RUN THIS
    // ZONA MENENGAH - CURIOUS
    if (currentMood == IDLE) {       // KLO DISTANCE SGEITU DAN IDLE THEN RUN THAT
      currentMood = CURIOUS;         // CHANGE TO CURIOUS
      moodChangeTime = currentTime;  // SAVE THAT LAST MOOD CHANGE TIME IN THAT VARB
    }
  } else {  // IF NONE OF THOSE FUNCTIONS RUN, THEN DO THIS
    // ZONA JAUH - kembali IDLE setelah beberapa saat
    if (currentTime - lastScareTime > 5000) {  // IF WAKTU SEKARANG - LAST SCARETIME IS BIGGER THAN 5s THEN DO THAT
      scareCounter = 0;                        // BASICALLY "HOW MUCH TIME HAS PASSED SINCE THE LAST SCARE?" IF ITS LONGER THAN 5s THEN RESET SCARECOUNTER
    }

    if (currentMood != IDLE && currentTime - moodChangeTime > 3000) {  // IF MOOD IS NOT IDLE AND MOOD TIME IS ALSO LONGGER THAN 3s
      currentMood = IDLE;                                              // CHANGE IT TO IDLE
      moodChangeTime = currentTime;                                    // RESET MOODCHANGETIME
    }
  }

  // TIRED mode setelah banyak aktivitas
  if (currentMood == ANGRY && currentTime - moodChangeTime > 8000) {  // IF MOOD IS ANGRY AND ITS BEEN MORE THAN 8s
    currentMood = TIRED;                                              // THEN CHANGE MOODE TO TIRED
    moodChangeTime = currentTime;                                     // RESET THE MOODCHANEGTIME
  }

  if (currentMood == TIRED && currentTime - moodChangeTime > 5000) {  // IF MOOD HAS BEEN TIRED FOR MORE THAN 5 SECOND
    currentMood = IDLE;                                               // THEN CHANGE MOOD TO IDLE
    scareCounter = 0;                                                 // AND SCARECOUNT TO 0
  }
}

// ===== EKSEKUSI BEHAVIOR SESUAI MOOD =====
void executeMoodBehavior() {
  switch (currentMood) {  // SWITCH BASICALLY TELLS TO LOOK AT THE "CURRENTMOOD" VARB VALUE  *LIKE SWITCH ON AND LOOK AT THE CURRENTMOOD VARB*
    case IDLE:            // IF ITS IDLE THEN DO RUN THE BEHAVIOR IDLE FUNCTION , AND THEN AFTER ITS RUNS, MAKE IT STOP
      behaviorIdle();
      break;
    case CURIOUS:  // IF CURRENTMOOD IS CURIOUS, THEN RUN BEHAVIOURCURIOUS FUNCTION, THEN TURN IT OFF AFTER IT RUNS
      behaviorCurious();
      break;
    case ALERT:
      behaviorAlert();
      break;
    case SCARE:
      behaviorScare();
      break;
    case ANGRY:
      behaviorAngry();
      break;
    case TIRED:
      behaviorTired();
      break;
  }
}

// ===== BEHAVIOR FUNCTIONS =====v
void behaviorIdle() {
  // Gerakan santai kiri-kanan
  servoRight.write(0);
  servoLeft.write(0);
  kecepatanGerak = 1;
  gerakBadanSmooth();
}

void behaviorCurious() {
  // Gerakan lebih aktif, kepala mengikuti
  servoRight.write(30);
  servoLeft.write(30);
  kecepatanGerak = 2;

  // Gerakan "mengintip"
  if (millis() - lastRandomMove > 1500) {  // IF LAST RANDOMMOVE IS MORE THAN 1,5s AGO THEN RUN THAT
    sudutBadan = 90 + random(-20, 20);     // BASICALLY SAYING, START FORM 90* AND THEN WIGGLE AROUND BETWEEN -20 TO 20
    servoBadan.write(sudutBadan);          // RUN SUDUTBADAN
    lastRandomMove = millis();             // SAVE LASTRANDOMOVE
  }

  // Suara penasaran
  if (millis() - lastSoundTime > 2000) {
    soundCurious();
    lastSoundTime = millis();
  }
}

void behaviorAlert() {
  // Siaga, tangan naik sedikit
  servoRight.write(60);
  servoLeft.write(60);
  kecepatanGerak = 3;

  // Fokus ke arah target
  servoBadan.write(90);

  // Suara alert
  if (millis() - lastSoundTime > 1500) {
    soundAlert();
    lastSoundTime = millis();
  }
}

void behaviorScare() {
  // GERAKAN MENAKUTKAN!
  unsigned long currentTime = millis();

  if (currentTime - moodChangeTime < 500) {  // HAS IT BEEN 0.5s SINCE THE LAST MOOD CHANGE?
    // Ledakan gerakan di awal
    servoBadan.write(90 + random(-30, 30));
    servoRight.write(180);
    servoLeft.write(180);
    soundScream();
  } else {
    // Gerakan agresif berkelanjutan
    servoBadan.write(60 + random(0, 60));
    servoRight.write(120 + random(-20, 20));
    servoLeft.write(120 + random(-20, 20));
  }
}

void behaviorAngry() {
  // Gerakan sangat agresif & cepat
  unsigned long currentTime = millis();

  if (currentTime % 200 < 100) {
    servoBadan.write(60);
    servoRight.write(180);
    servoLeft.write(0);
  } else {
    servoBadan.write(120);
    servoRight.write(0);
    servoLeft.write(180);
  }

  // Suara marah continuous
  if (millis() - lastSoundTime > 500) {
    soundAngry();
    lastSoundTime = millis();
  }
}

void behaviorTired() {
  // Gerakan lemah & lambat
  servoRight.write(20);
  servoLeft.write(20);
  kecepatanGerak = 1;

  // Turun perlahan
  if (sudutBadan > 70) {
    sudutBadan--;
    servoBadan.write(sudutBadan);
  }
}

// ===== GERAKAN SMOOTH =====
void gerakBadanSmooth() {
  if (arah) {
    sudutBadan += kecepatanGerak;
    if (sudutBadan >= 120) arah = false;
  } else {
    sudutBadan -= kecepatanGerak;
    if (sudutBadan <= 60) arah = true;
  }
  servoBadan.write(sudutBadan);
}

// ===== EFEK LED =====
void handleLEDEffects() {
  unsigned long currentTime = millis();

  switch (currentMood) {
    case IDLE:
      // LED berkedip pelan
      if (currentTime - lastBlinkTime > 1000) {
        ledState = !ledState;
        digitalWrite(LED_MATA_KIRI, ledState);
        digitalWrite(LED_MATA_KANAN, ledState);
        digitalWrite(LED_MERAH, LOW);
        lastBlinkTime = currentTime;
      }
      break;

    case CURIOUS:
      // LED nyala terus
      digitalWrite(LED_MATA_KIRI, HIGH);
      digitalWrite(LED_MATA_KANAN, HIGH);
      digitalWrite(LED_MERAH, LOW);
      break;

    case ALERT:
      // LED berkedip cepat
      if (currentTime - lastBlinkTime > 300) {
        ledState = !ledState;
        digitalWrite(LED_MATA_KIRI, ledState);
        digitalWrite(LED_MATA_KANAN, ledState);
        digitalWrite(LED_MERAH, LOW);
        lastBlinkTime = currentTime;
      }
      break;

    case SCARE:
      // LED menyala penuh
      digitalWrite(LED_MATA_KIRI, HIGH);
      digitalWrite(LED_MATA_KANAN, HIGH);
      digitalWrite(LED_MERAH, HIGH);
      break;

    case ANGRY:
      // LED merah berkedip sangat cepat
      if (currentTime - lastBlinkTime > 100) {
        ledState = !ledState;
        digitalWrite(LED_MATA_KIRI, ledState);
        digitalWrite(LED_MATA_KANAN, ledState);
        digitalWrite(LED_MERAH, HIGH);
        lastBlinkTime = currentTime;
      }
      break;

    case TIRED:
      // LED redup (nyala sebentar mati lama)
      if (currentTime - lastBlinkTime > 2000) {
        digitalWrite(LED_MATA_KIRI, HIGH);
        digitalWrite(LED_MATA_KANAN, HIGH);
        delay(100);
        digitalWrite(LED_MATA_KIRI, LOW);
        digitalWrite(LED_MATA_KANAN, LOW);
        digitalWrite(LED_MERAH, LOW);
        lastBlinkTime = currentTime;
      }
      break;
  }
}

// ===== SOUND EFFECTS =====
void soundCurious() {
  tone(BUZZER_PIN, 800, 100);
  delay(120);
  tone(BUZZER_PIN, 1000, 100);
}

void soundAlert() {
  tone(BUZZER_PIN, 1200, 150);
  delay(100);
  tone(BUZZER_PIN, 1200, 150);
}

void soundScream() {
  // Suara "jeritan" naik turun
  for (int i = 400; i < 1500; i += 100) {
    tone(BUZZER_PIN, i, 50);
    delay(50);
  }
  for (int i = 1500; i > 400; i -= 100) {
    tone(BUZZER_PIN, i, 50);
    delay(50);
  }
}

void soundAngry() {
  tone(BUZZER_PIN, 200, 100);
  delay(50);
  tone(BUZZER_PIN, 150, 100);
}

// ===== STARTUP ANIMATION =====
void startupAnimation() {
  // Test semua komponen
  Serial.println("=== HORROR ROBOT STARTING ===");

  // Test LED
  digitalWrite(LED_MATA_KIRI, HIGH);
  digitalWrite(LED_MATA_KANAN, HIGH);
  digitalWrite(LED_MERAH, HIGH);
  delay(500);
  digitalWrite(LED_MATA_KIRI, LOW);
  digitalWrite(LED_MATA_KANAN, LOW);
  digitalWrite(LED_MERAH, LOW);

  // Test Buzzer
  tone(BUZZER_PIN, 1000, 200);
  delay(300);
  tone(BUZZER_PIN, 1500, 200);
  delay(300);

  // Test Servo
  servoBadan.write(60);
  delay(300);
  servoBadan.write(120);
  delay(300);
  servoBadan.write(90);

  servoRight.write(90);
  servoLeft.write(90);
  delay(300);
  servoRight.write(0);
  servoLeft.write(0);

  Serial.println("=== READY TO SCARE! ===");
  delay(1000);
}

// ===== HELPER FUNCTION =====
String getMoodName() {
  switch (currentMood) {
    case IDLE: return "IDLE";
    case CURIOUS: return "CURIOUS";
    case ALERT: return "ALERT";
    case SCARE: return "SCARE";
    case ANGRY: return "ANGRY";
    case TIRED: return "TIRED";
    default: return "UNKNOWN";
  }
}