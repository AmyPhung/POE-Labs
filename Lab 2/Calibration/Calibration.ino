#define step_size 1 // in degrees - must be an int equal to or greater than 1
#define pan_range 15 // in degrees
#define tilt_range 30 // in degrees
#define scan_time 100 // in milliseconds

#include <Servo.h>

Servo pan_servo;  // create servo object to control pan servo
Servo tilt_servo;  // create servo object to control tilt servo

int pan_pos = 90; // default pan positon - 90 is straight ahead
int tilt_pos = 90; // default tilt position - 90 is straight ahead

int pan_pin = 9;
int tilt_pin = 10;

int IRPin = A0;
int IRValue = 0;

double IRValueIn = 0; // IR value in inches

void setup() {
  pan_servo.attach(pan_pin);  // attaches the servo on pin 9 to the servo object
  tilt_servo.attach(tilt_pin);  // attaches the servo on pin 9 to the servo object
  Serial.begin(115200);
}
void loop() {

  pan_servo.write(90);
  tilt_servo.write(90);
  IRValue = analogRead(IRPin);

  Serial.println(IRValue);
  
}
