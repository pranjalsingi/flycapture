/*  Blue Dragon Zophie Firmware Development
    Build 4 March 23 2017
    Pranjal Singi, Robert Schmalzigaug
    Developed for Adafruit Feather M0 and Feather MotorShieldV2
*/


/* PRANJAL TO DO LIST
 * Add servo to sketch. Look at Examples, Servo, Knop for an example. Servo is attached to PWM Pin 11 and BAT pin.
 * DC turret motor is on M4 on the Motorwing
 * Focus Stepper motor is on Steper 1 on the Motorwing, needs code (we do not have lower limit stopp at this time. 
 * LCD, on startup, choose case 2 from list below. Cirle radius 30px.
*/


// Included Libraries
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <TFT_ILI9163C.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Defining Pins
#define Enc1Pin A3             // Encoder(1) on analog Pin 3, Turret positions
#define Enc2Pin A4             // Encoder(2) on analog Pin 4, Turret initialization
#define Enc3Pin A5              // Encoder(3) on analog Pin 5, Focus position 
#define ledPin 11               // Illumination LED on Pin 10
#define __CS 10                // LCD CS on Pin 10
#define __DC 5                 // LCD A0 on Pin 5
//#define RST 6                  // Reset to GND
// the reset pin should be set on HIGH

#define BLACK   0x0000
#define WHITE   0xFFFF

// Defining Variables
int curPos = 0;
int distance = 0;               // Between curPos and newPos
int counter = 0;                // Counting turret steps turret
int image = 0;                  // tft displayed image
int y = 0;                      // half screen width 
int oriy = 0;                   // half screen origin coordinate
String val = "";                // Storing input from serial communication
bool flag = false;              // for Stepo Motor
bool stepoSwitch = true;        // switch for rotating             

// Establishing DC Motor Object
Adafruit_MotorShield AFMS = Adafruit_MotorShield();       // Establish motor object
Adafruit_DCMotor *Motor = AFMS.getMotor(2);               // Turret DC motor on M4 port on Motorwing 
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2); // Focus Stepper Motor on Stepper 1 port on Motorwing
Servo myservo;                                            // create servo object to control a servo

TFT_ILI9163C tft = TFT_ILI9163C(__CS, __DC);            // Establish tft display object


void setup() {

  Serial.begin(9600);             // initialize serial communication
  tft.begin();                    // initialize tft display
  AFMS.begin();                   // initialize I2C commnunication with motor
  myMotor->setSpeed(30);
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, 100);      // seting led to 50% intensity on startup

  // Servo Initialization
 /* myservo.attach(9);
  myservo.write(30);
  myservo.detach();*/
  

  // Obejctive turret initilization
  if(digitalRead(Enc3Pin)){
    while(digitalRead(Enc3Pin)){
      myMotor->step(1, 2, 1);
    }
  }

  while (digitalRead(Enc2Pin) == HIGH) {
    Motor->run(FORWARD);
    Motor->setSpeed(60);
    if (digitalRead(Enc2Pin) == LOW) {
      Motor->run(RELEASE);
      curPos = 0;
    }
  }
}

void loop() {
  // Recieve data from Node and write it to a String
//  if(digitalRead(Enc3Pin)){
//    Serial.println("HIGH");
//  }
//  else{
//    Serial.println("Low");
//  }
  
  if (Serial.available()) {
    char inChar = (char)Serial.read();
    
    // FOCUS MOTOR CODE
    if(inChar == 'Q'){  //goes forward on single
      Serial.println("coming here");
      flag = true;
      int temp = val.toInt();
      val = "";
      if(temp == 1)       moveStepper(1, 1, 1);
      else if(temp == 2)  moveStepper(1, 2, 1);
      else if(temp == 3)  moveStepper(1, 1, 4);
      else if(temp == 4)  moveStepper(1, 2, 4);
    }
    
    // SET ILLUMINATION INTENSITY ------------------------------------
    else if (inChar == 'S') {
      analogWrite(ledPin, val.toInt());
      Serial.print("LED value: ");
      Serial.print(val);
      val = "";
    }
    
    // CHANGE MAGNIFICATION ------------------------------------------
    else if (inChar == 'M') {
      int newPos = val.toInt();
      distance = curPos - newPos;
      curPos = newPos;
      val = "";
      // SET DISTANCE AND DIRECTION OF TURRET MOVEMENT -------
      if (distance > 0) {
        if (distance > 2) {
          distance = 5 - distance;
          rotate('F', distance);
        }
        else {
          rotate('B', distance);
        }
      }
      else {
        distance = abs(distance);
        if (distance > 2) {
          distance = 5 - distance;
          rotate('B', distance);
        }
        else {
          rotate('F', distance);
        }
      }
    }
    // CHANGE LCD DISPLAY -------------------------------------------
    else if (inChar == 'A') {
      image = val.toInt();
      val = "";
      switch (image) {
        case 1: {
            tft.fillScreen();
            FilledCircles(10, WHITE);
            Serial.println("Circls radius 10");
            }
            break;
        case 2: {
            tft.fillScreen();
            FilledCircles(30, WHITE);
            Serial.println("Circle radius 30");
            }
            break;
        case 3: {
            tft.fillScreen();
            FilledCircles(50, WHITE);
            Serial.println("Circle radius 50");
            }
            break;
        case 4: {
            tft.fillScreen();
            FilledRects(0, (tft.height() / 2), WHITE);
            Serial.println("Left Half");
            }
            break;
        case 5: {
            tft.fillScreen();
            FilledRects((tft.height() / 2), (tft.height() / 2), WHITE);
            Serial.println("Right Half");
            }
            break;
        case 6: {
            tft.fillScreen(WHITE);
            FilledCircles(10, BLACK);
            Serial.println("Dark Field 30");
            }
            break;
      }
    }
    /*else if(inChar == 'E'){
      int temp = val.toInt();
      myservo.attach(9);
      if(temp == 1)   myservo.write(30);
      else            myservo.write(100);
      delay(300);
      myservo.detach();          
    }*/
    // creates number from input for led and newPos
    else {
      val += inChar;
    }
  }
}

// FUNCTION DEFINING TURRET ROTATION SPEED AND COUNTING TURRET MARKS FOR POSITIONING
int rotate(char directn, int steps) {

  // GET FOCUS TO LOW
  while(digitalRead(Enc3Pin)){
    myMotor->step(1, 2, 1);
  }

  if (directn == 'F') {
    Motor->run(FORWARD);
    Motor->setSpeed(60);
    while (!digitalRead(Enc1Pin));
//    delay(400);
    while (counter < steps) {
      while (digitalRead(Enc1Pin));
      counter++;
      if (counter == steps) {
        break;
      }
      while (!digitalRead(Enc1Pin));
    }
    Motor->run(RELEASE);
    counter = 0;
  }
  else {
    Motor->run(BACKWARD);
    Motor->setSpeed(60);
    while (!digitalRead(Enc1Pin));
//    delay(400);
    while (counter < steps) {
      while (digitalRead(Enc1Pin));
      counter++;
      if (counter == steps) {
        break;
      }
      while (!digitalRead(Enc1Pin));
    }
    Motor->run(RELEASE);
    counter = 0;
  }
}

// DEFINE SHAPES DISPLAYED ON TFT IMAGES FOR BRIGHT / OBLIQUE / DARK FIELD IMAGING
unsigned long FilledCircles(uint8_t radius, uint16_t color) {
  tft.fillCircle(64, 64, radius, color);
}

unsigned long FilledRects(uint8_t oriy, uint8_t y, uint16_t color1) {
  int x = 128, orix = 0;
  tft.fillRect(orix, oriy, x, y, color1);
}

// FOCUS MOTOR
int moveStepper(int steps, int direct, int steptype){
  // CHECKS FOR FOCUS SENSOR HIGH, THEN ONLY FOCUS
  while(flag){
    if(direct == 2 && !digitalRead(Enc3Pin)){
      flag = false;
      break;
    }
    myMotor->step(steps, direct, steptype);
    if(Serial.available()){
      if(Serial.read() == 'W')  flag = false;
    }
  }
}

