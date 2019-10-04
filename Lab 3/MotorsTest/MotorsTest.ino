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

struct pid {
  float P, I, D;
  String Pstring, Istring, Dstring;
};
typedef struct pid PID;

PID pid1;
PID pid2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  shield.begin();

  resetLCD();
}

void loop() {
  if(Serial.available() > 0) {
    lcd.begin(16,2);
    resetLCD();
    pid1 = readSerial();
    pid2 = readSerial();
    updateLCD(pid1, pid2);
  }

  // lcd.setCursor(1, 0);
  // lcd.print(P1string);
  // lcd.setCursor(6, 0);
  // lcd.print(I1string);
  // lcd.setCursor(11, 0);
  // lcd.print(D1string);
  // P1string = "";
  // I1string = "";
  // D1string = "";
//    delay(0);
//    if (data == 'v'){
//      LMotor->setSpeed(50);
//      delay(100);
//    }
//    else {
//      LMotor->setSpeed(0);
//      delay(100);
//    }
//  }

//  if(Serial.available() > 0) {
//    char data = Serial.read();
//    char str[2];
//    str[0] = data;
//    str[1] = '\0';
//    Serial.print(str);
//  }
//  lcd.setCursor(2, 1);
//  // print the number of seconds since reset:
////  lcd.print(millis() / 1000);
//  lcd.print(4.1);
//
//  lcd.setCursor(7, 1);
//  lcd.print(6.3);
//
//  lcd.setCursor(12, 1);
//  lcd.print(6.5);
//
//    lcd.setCursor(0, 1);
//  lcd.print("P:");
//  lcd.setCursor(5, 1);
//  lcd.print("I:");
//  lcd.setCursor(10, 1);
//  lcd.print("D:");

  // put your main code here, to run repeatedly:
  LMotor->setSpeed(0); // Speed goes from 0 255
  LMotor->run(FORWARD);  // Forward or backwards

  RMotor->setSpeed(0); // Speed goes from 0 255
  RMotor->run(FORWARD);  // Forward or backwards
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
