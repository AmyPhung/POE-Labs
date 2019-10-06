#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include <LiquidCrystal.h>

Adafruit_MotorShield shield = Adafruit_MotorShield();
Adafruit_DCMotor *LMotor = shield.getMotor(1);
Adafruit_DCMotor *RMotor = shield.getMotor(2);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

String P1string, I1string, D1string;
String P2string, I2string, D2string;

int prevSpeedL = 0;
int prevSpeedR = 0;

struct pid {
  float P, I, D;
  String Pstring, Istring, Dstring;
};
typedef struct pid PID;

PID pid1;
PID pid2;
bool isEStopped = true;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  Serial.begin(115200);
  shield.begin();

  resetLCD();
}

void loop() {
//  if(Serial.available() > 0) {
//    if (Serial.read() == 'e') {
//      isEStopped = !isEStopped;
//    } else {
//      resetLCD();
//      pid1 = readSerial();
//      pid2 = readSerial();
//      updateLCD(pid1, pid2);
//    }
//  }
//
////  if (isEStopped) {
////    LMotor->setSpeed(0); // Speed goes from 0 255
////    LMotor->run(FORWARD);  // Forward or backwards
////
////    RMotor->setSpeed(0); // Speed goes from 0 255
////    RMotor->run(FORWARD);  // Forward or backwards
////    return;
////  }

  //TODO: make this an update function
  float rightSensor = getSensorVoltage(A1);
  float leftSensor = getSensorVoltage(A0);
  Serial.print(rightSensor); Serial.print(" : "); Serial.println(leftSensor);
  float normRightSensor = normalizeReading(0.8, 1.2, rightSensor );
  float normLeftSensor = normalizeReading(0.6, 1.4, leftSensor );
  Serial.print(normRightSensor); Serial.print(" : "); Serial.println(normLeftSensor);
  int speedL;
  int speedR;


  int LForwards = 30;
  int RForwards = 20;

  int LStop = 0;
  int RStop = 0;
  
  if (normRightSensor==1 && normLeftSensor==1) {
    speedL = LForwards;
    speedR = RForwards;
  } else if (normRightSensor==1 && normLeftSensor==0) {
    speedL = LForwards;
    speedR = RStop;
  } else if (normRightSensor==0 && normLeftSensor==1) {
    speedL = LStop;
    speedR = RForwards;
  } else {
    speedL = prevSpeedL;
    speedR = prevSpeedR;
  }

  
//
//  int speedL = normLeftSensor * pid1.P;
//  int speedR = normRightSensor * pid2.P; // TODO: add in other constants here
  Serial.println("SPEEEEEED");
  Serial.print(speedL); Serial.print(" : "); Serial.println(speedR);

  LMotor->setSpeed(0); // Speed goes from 0 255
  LMotor->run(FORWARD);  // Forward or backwards

  RMotor->setSpeed(0); // Speed goes from 0 255
  RMotor->run(FORWARD);  // Forward or backwards

  prevSpeedR = speedR;
  prevSpeedL = speedL;
}

void resetLCD() {
  lcd.setCursor(0, 0);
  lcd.print("P");
  lcd.setCursor(5, 0);
  lcd.print("I");
  lcd.setCursor(10, 0);
  lcd.print("D");
  lcd.setCursor(16, 0);
  lcd.print("L");
  lcd.setCursor(0, 1);
  lcd.print("P");
  lcd.setCursor(5, 1);
  lcd.print("I");
  lcd.setCursor(10, 1);
  lcd.print("D");
  lcd.setCursor(16, 1);
  lcd.print("R");
}

PID readSerial() {
  PID pid_out;

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

void updateLCD(PID pid1, PID pid2) {
  lcd.setCursor(1, 0);
  lcd.print(pid1.Pstring);
  lcd.setCursor(6, 0);
  lcd.print(pid1.Istring);
  lcd.setCursor(11, 0);
  lcd.print(pid1.Dstring);
  lcd.setCursor(1, 1);
  lcd.print(pid2.Pstring);
  lcd.setCursor(6, 1);
  lcd.print(pid2.Istring);
  lcd.setCursor(11, 1);
  lcd.print(pid2.Dstring);
}

float getSensorVoltage(int pin) {
   float sensorVal = analogRead(pin);
   float voltage = sensorVal * (5.0 / 1023.0);
   return voltage;
}

float normalizeReading(float sensor_min, float sensor_max, float sensor_value) {
  // take in a sensor max and min
  // make that a value from 0 to 1
  // TODO: Add error checking here
  float output = 0.0 + (sensor_value - sensor_min) * (1.0 - 0.0) / (sensor_max - sensor_min);
//  if (output > 1) {
//    output = 1;
//  } else if (output < 0){
//    output = 0;
//  }
//  return output;
  if (output > 1) {
     Serial.println(output);
      return 1;
    } else {
       Serial.println(output);
      return 0;
    }
   
}
