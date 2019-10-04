/*
  ReadAnalogVoltage

  Reads an analog input on pin 0, converts it to voltage, and prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/ReadAnalogVoltage
*/

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // get the sensor voltage
  int sensorVoltage0 = getSensorVoltage(A0);
  int sensorVoltage1 = getSensorVoltage(A1);
  
  // normalize readings taking into account the sensor attributes
  float sensor_norm0 = normalizeReading( 2.88, 3.9, sensorVoltage0 );
  float sensor_norm1 = normalizeReading( 2.35, 3.45, sensorVoltage1 );
  // print those values!
  Serial.print(sensor_norm0); Serial.print(" : "); Serial.print(sensor_norm1);

  // determine whether to turn left or right
  if (shouldTurnLeft(sensor_norm0, sensor_norm1) ) Serial.println(" Turn Left");
  else Serial.println(" Turn Right");

  delay(100);
}

float normalizeReading(float sensor_min, float sensor_max, float sensor_value) {
  // take in a sensor max and min
  // make that a value from 0 to 1

  return 0.0 + (sensor_value - sensor_min) * (1.0 - 0.0) / (sensor_max - sensor_min);

}

bool shouldTurnLeft( float left_value, float right_value) {
  // this function assumes that left and right have been normalized from 0.0 to 1.0
  if (right_value < left_value) {
    return true;
  }
  else {
    return false;
  }
}

float getSensorVoltage( int pin) {
   float sensorVal = analogRead(pin);
   float voltage = sensorVal * (5.0 / 1023.0);
   return voltage;  
}
