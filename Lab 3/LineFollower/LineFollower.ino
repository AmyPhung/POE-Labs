/*
LineFollower.ino

Written by: Amy Phung and Everardo Gonzalez

Contains code to run a line-following robot equipped with two IR sensors,
an LCD screen, and two DC motors connected to an Adafruit motor driver shield
*/

#include <PID_v1.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <LiquidCrystal.h>
#include <stdio.h> // for function sprintf

// Constants
const int PYTHONMODE = 2; // 0 - no python, free to spam serial monitor
                              // 1 - use python to get PID values, no serial spamming
                              // 2 - use python for data collection

const int RMIN = 180; // Average right IR reading when off line
const int RMAX = 340; // Average right IR reading when on line
const int LMIN = 235; // Average left IR reading when off line
const int LMAX = 380; // Average left IR reading when on line

const int VELOCITY = 30;  // Speed to drive forwards at
const int MAXOFFSET = 30; // Amount to add/subtract from wheels to make turns

// E-stop
bool isEStopped = true; // Start in a safe e-stopped state

// LCD Screen variables
Adafruit_MotorShield shield = Adafruit_MotorShield();
Adafruit_DCMotor *LMotor = shield.getMotor(1);
Adafruit_DCMotor *RMotor = shield.getMotor(2);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// variables for recording data
char data_out[300];

// PID variables
String Pstring, Istring, Dstring;
double Setpoint, Input, Output;
double Kp=0.1, Ki=0, Kd=10;  // Goes from ~200 to ~20
PID linePID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

struct pidData {
  // Data structure to save PID values in
  float P, I, D;
  String Pstring, Istring, Dstring;
};
typedef struct pidData PIDData;
PIDData pidValues;

// Motor Speed Variables
int LSpeed = 0;
int RSpeed = 0;

//------------------- Arduino Setup Function ----------------------------------
void setup() {
  lcd.begin(16,2);
  Serial.begin(115200);
  linePID.SetMode(AUTOMATIC);
  shield.begin();
  resetLCD();

  if (PYTHONMODE == 0) {
    pidValues = readValues(); // Use default PID values
    updateLCD(pidValues);
  }
} // setup()

//---------------- Arduino Main Loop Function ----------------------------------
void loop() {
  if (PYTHONMODE == 1) { // Only read from serial if connected to python code
    if(Serial.available() > 0) {
      if (Serial.read() == 'e') { // e will be sent for an e-stop
        isEStopped = !isEStopped;
        lcd.setCursor(6,0);
        lcd.print(isEStopped);
      } else {
        resetLCD();
        pidValues = readSerial();
        updateLCD(pidValues);
      }
    }
    if (isEStopped) {
      LMotor->setSpeed(0); // Speed goes from 0 255
      LMotor->run(FORWARD);  // Forward or backwards

      RMotor->setSpeed(0); // Speed goes from 0 255
      RMotor->run(FORWARD);  // Forward or backwards
      return;
    }
  } else { // Without serial, don't use an e-stop
    isEStopped = false;
    lcd.setCursor(6,0);
    lcd.print(isEStopped);
  }

  int rightSensor = getSensorVoltage(A1);
  int leftSensor = getSensorVoltage(A0);

  int normRightSensor = normalizeReading(RMIN, RMAX, rightSensor);
  int normLeftSensor = normalizeReading(LMIN, LMAX, leftSensor);

  int offset = computeOffset(normLeftSensor, normRightSensor);
  computeMotorSpeeds(offset);

  LMotor->setSpeed(LSpeed); // Speed goes from 0 255
  LMotor->run(FORWARD);  // Forward or backwards

  RMotor->setSpeed(RSpeed); // Speed goes from 0 255
  RMotor->run(FORWARD);  // Forward or backwards

  if (PYTHONMODE == 0) {
    Serial.println("Raw Outputs:");
    Serial.print(leftSensor); Serial.print(" : "); Serial.println(rightSensor);
    Serial.println("Normalized Outputs:");
    Serial.print(normLeftSensor); Serial.print(" : "); Serial.println(normRightSensor);
    Serial.println("Speeds:");
    Serial.print(LSpeed); Serial.print(" : "); Serial.println(RSpeed);
  }
  else if (PYTHONMODE == 2) {
    char s[30];
    snprintf(s, sizeof(s), "%i, %i, %i, %i, %i, %i, ",
                            leftSensor, rightSensor, normLeftSensor, normRightSensor, LSpeed, RSpeed);
    Serial.println(s);
  }

} // loop()

//---------------- IR and Controls Functions -----------------------------------
int getSensorVoltage(int pin) {
   // Returns the current measurement on the IR sensor
   int sensorVal = analogRead(pin);
   return sensorVal;
}

int normalizeReading(int sensor_min, int sensor_max, int sensor_value) {
  // Maps a sensor measurement between 0 and 1000. If sensor measurement maps
  // to something lower or higher than 0 or 1000, caps the mapping to a number
  // within that range
  int output;
  output = map(sensor_value, sensor_min, sensor_max, 0, 1000);
  if (output < 0) {
    return 0;
  }
  if (output > 1000) {
    return 1000;
  }
  return output;
}

int computeOffset(int L, int R) {
  // PID library can't handle both cases, so we'll manually handle left/right
  // Computes offset to add/subtract from motor values to cause turns based on
  // difference between left and right sensor measurements
  if (L - R < 0) {
    Input = L - R;
    linePID.Compute();
    if (Output > MAXOFFSET) {
      return MAXOFFSET;
    }
    return Output;
  } else {
    Input = R - L;
    linePID.Compute();
    if (Output > MAXOFFSET) {
      return -MAXOFFSET;
    }
    return -Output;
  }
}

void computeMotorSpeeds(int turnOffset) {
  // Adds computed offset to left wheel and subtracts offset from right wheel
  // to cause turns
  LSpeed = VELOCITY + turnOffset;
  RSpeed = VELOCITY - turnOffset;
}

//---------------------------- LCD Functions -----------------------------------
void resetLCD() {
  // Sets LCD to an initial state with info on whether or not the robot is
  // e-stopped, whether or not it's using python, and the current PID values
  // it's using
  lcd.setCursor(0, 0);
  lcd.print("ESTOP:");
  lcd.setCursor(6,0);
  lcd.print(isEStopped);
  lcd.setCursor(8,0);
  lcd.print("PYTHON:");
  lcd.print(PYTHONMODE);
  lcd.setCursor(0, 1);
  lcd.print("P");
  lcd.setCursor(5, 1);
  lcd.print("I");
  lcd.setCursor(10, 1);
  lcd.print("D");
}

void updateLCD(PIDData pid) {
  // Update the LCD with the new PID values
  lcd.setCursor(1, 1);
  lcd.print(pid.Pstring);
  lcd.setCursor(6, 1);
  lcd.print(pid.Istring);
  lcd.setCursor(11, 1);
  lcd.print(pid.Dstring);
}

//---------------------------- Serial Functions --------------------------------
PIDData readSerial() {
  // Parses the serial input from the computer and returns the PID input in the
  // PIDData data structure.
  PIDData pid_out;

  char data = Serial.read();
  if (data == 'p') {
    while (true) {
      data = Serial.read();
      if (data == 'i') {
        break;
      }
      pid_out.Pstring += data;
    }
    while (true) {
      data = Serial.read();
      if (data == 'd') {
        break;
      }
      pid_out.Istring += data;
    }
    while (true) {
      data = Serial.read();
      if (data == 'e') {
        break;
      }
      pid_out.Dstring += data;
    }
  }

  pid_out.P = pid_out.Pstring.toFloat();
  pid_out.I = pid_out.Istring.toFloat();
  pid_out.D = pid_out.Dstring.toFloat();
  return pid_out;
}

PIDData readValues() {
  // Writes hard-coded PID values into a PIDData struct.
  PIDData pid_out;
  pid_out.P = Kp;
  pid_out.I = Ki;
  pid_out.D = Kd;
  pid_out.Pstring = String(pid_out.P);
  pid_out.Istring = String(pid_out.I);
  pid_out.Dstring = String(pid_out.D);
  return pid_out;
}
