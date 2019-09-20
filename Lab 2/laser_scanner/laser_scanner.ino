#define step_size 1 // in degrees
#define pan_range 15 // in degrees
#define tilt_range 30 // in degrees
#define scan_time 100 // in milliseconds

#include <Servo.h>

Servo pan_servo;  // create servo object to control a servo
Servo tilt_servo;  // create servo object to control a servo

int pan_pos = 90;
int tilt_pos = 90;

int pan_pin = 9;
int tilt_pin = 10;

int IRPin = A0;
int IRValue = 0;

double IRValueIn = 0;

void setup() {
  pan_servo.attach(pan_pin);  // attaches the servo on pin 9 to the servo object
  tilt_servo.attach(tilt_pin);  // attaches the servo on pin 9 to the servo object
  Serial.begin(115200);
}

void loop() {
  pan_servo.write(pan_pos); // 0 is right, 180 is left
  tilt_servo.write(tilt_pos); // 180 is up, 0 is down
  IRValue = analogRead(IRPin);
  IRValueIn = convertToInches(IRValue);

  // in steps of 1 degree, scan object
  for (tilt_pos = 90 + tilt_range; tilt_pos >= 90 - tilt_range; tilt_pos -= step_size) { // goes from 180 to 0 degrees
    tilt_servo.write(tilt_pos);              // tell servo to go to position in variable 'pos'
    for (pan_pos = 90 + pan_range; pan_pos >= 90 - pan_range; pan_pos -= step_size) {
      pan_servo.write(pan_pos);
      delay(scan_time);                       // waits 15ms for the servo to reach the position - TODO: Remove this
      IRValue = analogRead(IRPin);
      IRValueIn = convertToInches(IRValue);

      Serial.println('p'); // Send 'p' to indicate new point is being sent
      Serial.println(convertX(pan_pos,tilt_pos,IRValueIn));
      Serial.println(convertY(pan_pos,tilt_pos,IRValueIn));
      Serial.println(convertZ(pan_pos,tilt_pos,IRValueIn));
    }

  }

}

double convertToInches(int x) {
  double output;
  output = -1.324e-11*pow(x,5) + 2.416e-08*pow(x,4) - 1.756e-05*pow(x,3) +
           0.006438*pow(x,2) - 1.243*x + 119.8;

  return output;
}

double convertX(int theta, int phi, double rho) {
  double xcoord;
  double theta_rad;
  double phi_rad;
  theta_rad = theta * 3.14 / 180;
  phi_rad = phi * 3.14 / 180;
  xcoord = rho * cos(3.14-theta_rad) * sin(phi_rad-3.14);
  return xcoord;
}

double convertY(int theta, int phi,  double rho) {
  double ycoord;
  double theta_rad;
  double phi_rad;
  theta_rad = theta * 3.14 / 180;
  phi_rad = phi * 3.14 /90;
  ycoord = rho * sin(3.14-theta_rad) * sin(phi_rad-3.14);
  return ycoord;
}

double convertZ(int theta, int phi, double rho){
  double zcoord;
  double phi_rad;
  phi_rad = phi * 3.14 / 180;
  zcoord = rho * cos(phi_rad + 1.57);
  return zcoord;
}
