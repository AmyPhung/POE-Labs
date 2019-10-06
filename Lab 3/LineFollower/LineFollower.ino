#include <PID_v1.h>
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <LiquidCrystal.h>

// Constants
const bool PYTHONMODE = false; // When true, use python code to set PID values

const int RMIN = 180;
const int RMAX = 340;
const int LMIN = 235;
const int LMAX = 380;

const int VELOCITY = 30;
const int MAXOFFSET = 30;

// E-stop
bool isEStopped = true;

// LCD Screen variables
Adafruit_MotorShield shield = Adafruit_MotorShield();
Adafruit_DCMotor *LMotor = shield.getMotor(1);
Adafruit_DCMotor *RMotor = shield.getMotor(2);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

String Pstring, Istring, Dstring;

// PID variables
double Setpoint, Input, Output;
double Kp=0.1, Ki=0, Kd=0;  // Goes from ~200 to ~20
PID linePID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

struct pidData {
  float P, I, D;
  String Pstring, Istring, Dstring;
};
typedef struct pidData PIDData;
PIDData pidValues;

// Motor Speed Variables
int LSpeed = 0;
int RSpeed = 0;

void setup() {
  lcd.begin(16,2);
  Serial.begin(115200);
  linePID.SetMode(AUTOMATIC);
  shield.begin();
  resetLCD();

  if (!PYTHONMODE) {
    pidValues = readValues();
    updateLCD(pidValues);
  }
}

void loop() {
  if (PYTHONMODE) { 
    if(Serial.available() > 0) {
      if (Serial.read() == 'e') {
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
  } else {
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

  if (!PYTHONMODE) {
    Serial.println("Raw Outputs:");
    Serial.print(leftSensor); Serial.print(" : "); Serial.println(rightSensor);
    Serial.println("Normalized Outputs:");
    Serial.print(normLeftSensor); Serial.print(" : "); Serial.println(normRightSensor);
    Serial.println("Speeds:");
    Serial.print(LSpeed); Serial.print(" : "); Serial.println(RSpeed);
  }

  
} // loop() //////////////////////////////////////////////////////////

// Other functions

int getSensorVoltage(int pin) {
   int sensorVal = analogRead(pin);
   return sensorVal;
}

int normalizeReading(int sensor_min, int sensor_max, int sensor_value) {
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
  Serial.println("STARTING HERE");
  // PID cannot handle both cases, so we'll manually handle left/right
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
  LSpeed = VELOCITY + turnOffset;
  RSpeed = VELOCITY - turnOffset;
}

// LCD Functions
void resetLCD() {
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
  lcd.setCursor(1, 1);
  lcd.print(pid.Pstring);
  lcd.setCursor(6, 1);
  lcd.print(pid.Istring);
  lcd.setCursor(11, 1);
  lcd.print(pid.Dstring);
}

// Serial Functions
PIDData readSerial() {
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
  PIDData pid_out;
  pid_out.P = Kp;
  pid_out.I = Ki;
  pid_out.D = Kd;
  pid_out.Pstring = String(pid_out.P);
  pid_out.Istring = String(pid_out.I);
  pid_out.Dstring = String(pid_out.D);
  return pid_out;
}
