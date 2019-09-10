#define step_size 1 // in degrees
#define pan_range 50 // in degrees
#define tilt_range 30 // in degrees
#define scan_time 15 // in milliseconds

#include <Servo.h>

Servo pan_servo;  // create servo object to control a servo
Servo tilt_servo;  // create servo object to control a servo

int pan_pos = 0;
int tilt_pos = 0;

int pan_pin = 9;
int tilt_pin = 10;

int IRPin = A0;
int IRValue = 0;

void setup() {
  pan_servo.attach(pan_pin);  // attaches the servo on pin 9 to the servo object
  tilt_servo.attach(tilt_pin);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
}

void loop() {
  pan_servo.write(90); // 0 is right, 180 is left
  tilt_servo.write(90); // 180 is up, 0 is down
  IRValue = analogRead(IRPin);
  Serial.println(IRValue);

  /*for (tilt_pos = 90 + tilt_range; tilt_pos >= 90 - tilt_range; tilt_pos -= step_size) { // goes from 180 to 0 degrees
    tilt_servo.write(tilt_pos);              // tell servo to go to position in variable 'pos'
    Serial.println("print");
    for (pan_pos = 90 + pan_range; pan_pos >= 90 - pan_range; pan_pos -= step_size) {
      // in steps of 1 degree
      pan_servo.write(pan_pos);
      IRValue = analogRead(IRPin);
      Serial.print(IRValue);
      Serial.print(" ");


      delay(scan_time);                       // waits 15ms for the servo to reach the position
    }
    Serial.println(" ");
  }

  // for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15ms for the servo to reach the position
*/
}
