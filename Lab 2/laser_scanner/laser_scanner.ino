#define step_size 1 // in degrees
#define pan_range 50 // in degrees
#define tilt_range 30 // in degrees
#define scan_time 15 // in milliseconds

#include <Servo.h>

Servo pan_servo;  // create servo object to control a servo
Servo tilt_servo;  // create servo object to control a servo

int pan_pos = 45;
int tilt_pos = 45;

int pan_pin = 9;
int tilt_pin = 10;

int IRPin = A0;
int IRValue = 0;

double IRValueIn = 0;

void setup() {
  pan_servo.attach(pan_pin);  // attaches the servo on pin 9 to the servo object
  tilt_servo.attach(tilt_pin);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
}

void loop() {
  pan_servo.write(pan_pos); // 0 is right, 180 is left
  tilt_servo.write(tilt_pos); // 180 is up, 0 is down
  IRValue = analogRead(IRPin);
  //Serial.println(IRValue);
  double IRValueIn = convertToInches(IRValue);
  Serial.println(IRValueIn);
  double xcoord = convertX(pan_pos,tilt_pos,IRValueIn);
  double ycoord = convertY(pan_pos,tilt_pos,IRValueIn);
  double zcoord = convertZ(pan_pos,tilt_pos,IRValueIn);
  Serial.print(xcoord);
  Serial.print(',');
  Serial.print(ycoord);
  Serial.print(',');
  Serial.print(zcoord);
  Serial.println('.');



/*
  /for (tilt_pos = 90 + tilt_range; tilt_pos >= 90 - tilt_range; tilt_pos -= step_size) { // goes from 180 to 0 degrees
    tilt_servo.write(tilt_pos);              // tell servo to go to position in variable 'pos'
    Serial.println("print");
    for (pan_pos = 90 + pan_range; pan_pos >= 90 - pan_range; pan_pos -= step_size) {
      // in steps of 1 degree
      pan_servo.write(pan_pos);
      IRValue = analogRead(IRPin);
      // Serial.print(IRValue);



      delay(scan_time);                       // waits 15ms for the servo to reach the position
    }

  }

  // for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15ms for the servo to reach the position
*/
}

double convertToInches(int x) {
  double output;
  // output = 0.001986*pow(x,5) - 0.09726*pow(x,4) + 2.442*pow(x,3) -
  //          32.04*pow(x,2) + 177.6*x + 212.6;
  // 0.0001649 x - 0.01736 x + 0.6846 x - 11.82 x + 63.62 x + 456.2
  output = -1.324e-11*pow(x,5) + 2.416e-08*pow(x,4) - 1.756e-05*pow(x,3) +
           0.006438*pow(x,2) - 1.243*x + 119.8;

  return output;
}


double convertX(int theta, int phi, double rho) {
  double xcoord;
  double theta_rad;
  double phi_rad;
  Serial.print(theta);Serial.print(',');Serial.print(phi);Serial.print(',');Serial.print(rho);
  Serial.println('.');
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
  zcoord = rho * cos(phi_rad-3.14);
  return zcoord;
}
