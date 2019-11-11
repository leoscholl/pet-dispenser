// pet-dispenser
// by leoscholl
// Moves a servo to dispense food

#include <ServoTimer2.h> 
#include <Bounce2.h>
#include <Volume.h>
#include <EEPROM.h>

// define pins
#define SERVO_PIN 10
#define BUTTON_0_PIN 9
#define BUTTON_1_PIN 8
#define BUTTON_2_PIN 7
#define BUTTON_3_PIN 6
// Default speaker pin is 5
#define LED_PIN 4
#define PHOTODIODE_PIN A0

Bounce btn0 = Bounce();
Bounce btn1 = Bounce();
Bounce btn2 = Bounce();
Bounce btn3 = Bounce();
ServoTimer2 servo; 
int srvFwd = 1300;
int srvRev = 1530;
int unjamDur = 250;
int srvTimeout = 1500;
int thr = 300;
int blanks = 0;
int maxBlanks = 5;
int penalty = 15000;
Volume spkr;

// Mode selection
#define MODE_ADDR 0
#define MODE_ANY_BTN 0
#define MODE_TWO_ALT 1
#define MODE_UNBALANCED 2
#define MODE_TWO_PRESSES 3
#define NUM_MODES 4
unsigned int mode = 0;
int btn = -1;
unsigned long btn_t = 0;

void dispense(int quantity) {
  servo.attach(SERVO_PIN);
  digitalWrite(LED_PIN,HIGH);
  int baseline = analogRead(PHOTODIODE_PIN);
  servo.write(srvFwd);
  for (int i=0; i<quantity; i++) {
    bool trig = false;
    unsigned long t = millis();
    while (true) {
      if (baseline - analogRead(PHOTODIODE_PIN) > thr) {
        trig = true;
      } else if (millis() - t > srvTimeout) {
        blanks++;
        servo.write(srvRev);
        delay(unjamDur); 
        servo.write(srvFwd);
        break;
      } else if (trig && baseline - analogRead(PHOTODIODE_PIN > thr/3)) { 
        blanks = 0;
        break; // allow sensor to settle
      }
    }
  }
  digitalWrite(LED_PIN,LOW);
  servo.detach();
  if (blanks > maxBlanks) {
    delay(penalty);
    blanks = 0;
  }
}

//void ping(int repeat) {
//  spkr.begin();
//  int v = 127;
//  int f = 1000;
//  for (int i=0; i<repeat; i++) {
//    while(v < 250){
//      spkr.tone(f,v);
//      spkr.delay(1);
//      v++;
//    }
//    while(v > 0){
//      spkr.tone(f,v);
//      spkr.delay(1);
//      v--;
//    }
//    spkr.noTone();
//    spkr.delay(50);
//    v = 127;
//  }
//  spkr.end();
//}

//void buzz(int repeat) {
//  spkr.begin();
//  int v = 224;
//  int f = 350;
//  for (int i=0; i<repeat; i++) {
//    for (int j=0; j<5; j++) {
//      while(v < 255){
//        spkr.tone(f,v);
//        spkr.delay(1);
//        v++;
//      }
//      v = 224;
//    }
//    spkr.noTone();
//    spkr.delay(50);
//  }
//  spkr.end();
//}

void ping(int repeat) {
  for (int i=0; i<repeat; i++) {
    servo.attach(SERVO_PIN);
    servo.write(srvRev);
    delay(50); 
    servo.detach();
    delay(190);
  }
}

void buzz(int repeat) {
  // speaker is annoying. just remove for now
}

void setup() 
{ 
  pinMode(LED_PIN, OUTPUT);
  btn0.attach(BUTTON_0_PIN,INPUT_PULLUP);
  btn1.attach(BUTTON_1_PIN,INPUT_PULLUP);
  btn2.attach(BUTTON_2_PIN,INPUT_PULLUP);
  btn3.attach(BUTTON_3_PIN,INPUT_PULLUP);
  btn0.interval(25);
  btn1.interval(25);
  btn2.interval(25);
  btn3.interval(25);
  mode = EEPROM.read(MODE_ADDR);
  if (mode > NUM_MODES) {
    mode = 0;
    EEPROM.write(MODE_ADDR, mode);
  }
  ping(mode+1);
} 

 
void loop() 
{ 
  btn0.update();
  btn1.update();
  btn2.update();
  btn3.update();

  // Teacher button
  if (btn0.fell()) {
    unsigned long t = millis();
    while (true) {
      btn0.update();
      delay(1);
      if (millis() - t > 1000) { // long press
        mode++;
        mode %= NUM_MODES;
        btn = -1;
        EEPROM.write(MODE_ADDR, mode);
        ping(mode+1);
        break;
      } else if (btn0.rose()) { // short press
        dispense(1);
        break;
      }
    }
  }

  switch (mode) {
    case MODE_ANY_BTN:
      if (btn1.fell() || btn2.fell() || btn3.fell()) {
        dispense(1);
      }
      break;
    case MODE_TWO_ALT:
      if (btn1.fell() && btn != 2) {
        dispense(1);
        btn = 2;
      } else if (btn2.fell() && btn != 1) {
        dispense(1);
        btn = 1;
      } else if ((btn1.fell() && btn == 2) ||
        (btn2.fell() && btn == 1)) {
        buzz(2);
      }
      break;
    case MODE_UNBALANCED:
      if (btn1.fell()) {
        dispense(3);
      } else if (btn2.fell() || btn3.fell()) {
        dispense(1);
      }
      break;
    case MODE_TWO_PRESSES:
      if (btn1.fell() || btn2.fell() || btn3.fell()) {
        btn++;
        btn_t = millis();
      } else if (btn >= 0 && millis() - btn_t > 1500) {
        if (btn == 1) dispense(1);
        btn = -1;
      }
      break;
  }
} 
