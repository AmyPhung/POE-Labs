/*
laser_scanner.ino

By: Amy Phung and Sophie Wu

Contains code for a pan/tilt 3D IR scanner

Scan range is set by pan_range and tilt_range, step size is set by step_size.
scan_time sets amount of time to wait for motor and IR response before moving
to next scan position. Sends measured point (x,y,z) in inches over serial
*/

#define step_size 1 // in degrees - must be an int equal to or greater than 1
#define pan_range 20 // in degrees
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
  pan_servo.write(pan_pos); // 0 is right, 180 is left
  tilt_servo.write(tilt_pos); // 180 is up, 0 is down
  IRValue = analogRead(IRPin); // Get analog measurement from IR sensor
  IRValueIn = convertToInches(IRValue);

  // in steps of 1 degree, scan object
  for (tilt_pos = 90 + tilt_range; tilt_pos >=
       90 - tilt_range; tilt_pos -= step_size) { // goes from top down
    tilt_servo.write(tilt_pos); // move tilt servo
    for (pan_pos = 90 + pan_range; pan_pos >=
         90 - pan_range; pan_pos -= step_size) { // goes from left to right
      pan_servo.write(pan_pos);
      // wait for the servo to reach the position and give IR time to scan
      delay(scan_time);
      IRValue = analogRead(IRPin);
      IRValueIn = convertToInches(IRValue);

      // Send output via serial
      Serial.println('p'); // Send 'p' to indicate new point is being sent
      Serial.println(convertX(pan_pos,tilt_pos,IRValueIn));
      Serial.println(convertY(pan_pos,tilt_pos,IRValueIn));
      Serial.println(convertZ(pan_pos,tilt_pos,IRValueIn));
    }

  }

}

double convertToInches(int x) {
  /*
    Takes in a measured IR value and computes corresponding value in inches.
    The equation is a best-fit line computed from empirically measured points.

    Args:
      x (int): Output of analogRead() from the IR sensor

    Returns:
      output (double): Distance in inches from the IR sensor
  */
  double output;
  output = -4.044e-12*pow(x,5) + 1.507e-08*pow(x,4) - 1.681e-05*pow(x,3) +
           0.008214*pow(x,2) - 1.896*x + 187.2;

  return output;
}

double convertX(int theta, int phi, double rho) {
  /*
    Converts current spherical coordinate to cartesian x coordinate in inches

    Args:
      theta (int): Current pan servo angle in degrees
      phi (int): Current tilt servo angle in degrees
      rho (double): IR measurement value in inches

    Returns:
      xcoord (double): Cartesian x coordinate
  */
  double xcoord;
  double theta_rad;
  double phi_rad;
  theta_rad = theta * 3.14 / 180;
  phi_rad = phi * 3.14 / 180;
  xcoord = rho * cos(3.14-theta_rad) * sin(phi_rad-3.14);
  return xcoord;
}

double convertY(int theta, int phi,  double rho) {
  /*
    Converts current spherical coordinate to cartesian y coordinate in inches

    Args:
      theta (int): Current pan servo angle in degrees
      phi (int): Current tilt servo angle in degrees
      rho (double): IR measurement value in inches

    Returns:
      xcoord (double): Cartesian y coordinate
  */
  double ycoord;
  double theta_rad;
  double phi_rad;
  theta_rad = theta * 3.14 / 180;
  phi_rad = phi * 3.14 / 180;
  ycoord = rho * sin(3.14-theta_rad) * sin(phi_rad-1.57);
  return ycoord;
}

double convertZ(int theta, int phi, double rho){
  /*
    Converts current spherical coordinate to cartesian z coordinate in inches

    Args:
      theta (int): Current pan servo angle in degrees
      phi (int): Current tilt servo angle in degrees
      rho (double): IR measurement value in inches

    Returns:
      xcoord (double): Cartesian z coordinate
  */
  double zcoord;
  double phi_rad;
  phi_rad = phi * 3.14 / 180;
  zcoord = rho * cos(phi_rad + 1.57);
  return zcoord;
}
