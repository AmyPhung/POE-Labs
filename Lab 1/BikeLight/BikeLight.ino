/*
  BikeLight
  Turns on an LED on for one second, then off for one second, repeatedly.

  
 */

// Pin 13 has an LED connected on most Arduino boards.
// Pin 11 has the LED on Teensy 2.0
// Pin 6  has the LED on Teensy++ 2.0
// Pin 13 has the LED on Teensy 3.0
// give it a name:
int green_pin  = 13;
int yellow_pin = 12;
int red_pin    = 11;
int button_pin = 7;

int button_val = 0;
int prev_button_val = 0;

int state     = 0;
int sub_state = 0;

// Timing Variables
unsigned long oldLoopTime = 0;    //create a name for past loop time in milliseconds
unsigned long newLoopTime = 0;    //create a name for new loop time in milliseconds
unsigned long cycleTime = 0;      //create a name for elapsed loop cycle time
const long controlLoopInterval = 200; //create a name for control loop cycle time in milliseconds


// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  // initialize the digital pin as an output.
  pinMode(green_pin, OUTPUT);
  pinMode(yellow_pin, OUTPUT);
  pinMode(red_pin, OUTPUT);
  pinMode(button_pin, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  button_val = digitalRead(button_pin);
  Serial.println(button_val);
  // Serial.println(checkSwitch);
  if (checkSwitch(button_val, prev_button_val)) {
    state++;
    if (state >= 7) {
      state = 0;
    }
  }

  prev_button_val = button_val;

  switch (state) {
    case 0:
      resetLights();
      Serial.println("Case 0");
      digitalWrite(green_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
      break;
    case 1:
      resetLights();
      Serial.println("Case 1");
      digitalWrite(yellow_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
      break;
    case 2:
      resetLights();
      Serial.println("Case 2");
      digitalWrite(red_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
      break;
    case 3:
      resetLights();
      Serial.println("Case 3");
      digitalWrite(green_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(yellow_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(red_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
      break;
    case 4:
      resetLights();
      Serial.println("Case 4");
      newLoopTime = millis();               // get current Arduino time (50 days till wrap)
      if (newLoopTime - oldLoopTime >= controlLoopInterval) { // if true run flight code
        oldLoopTime = newLoopTime;          // reset time stamp
        if (sub_state == 0) {
          sub_state = 1;
        } else {
          sub_state = 0;
        }
      }
      if (sub_state == 0) {
        resetLights();
      } else {
        digitalWrite(green_pin, HIGH);    // turn the LED off by making the voltage HIGH
        digitalWrite(yellow_pin, HIGH);    // turn the LED off by making the voltage HIGH
        digitalWrite(red_pin, HIGH);    // turn the LED off by making the voltage HIGH
      }
      break;
    case 5:
      resetLights();
      Serial.println("Case 5");
      newLoopTime = millis();               // get current Arduino time (50 days till wrap)
      if (newLoopTime - oldLoopTime >= controlLoopInterval) { // if true run flight code
        oldLoopTime = newLoopTime;          // reset time stamp
        Serial.println(sub_state);
        sub_state++;
        if (sub_state >= 3) {
          sub_state = 0;
        }
      }

      resetLights();
      switch(sub_state) {
        case 0:
          digitalWrite(green_pin, HIGH);    // turn the LED off by making the voltage HIGH
          break;
        case 1:
          digitalWrite(yellow_pin, HIGH);    // turn the LED off by making the voltage HIGH
          break;
        case 2:
          digitalWrite(red_pin, HIGH);    // turn the LED off by making the voltage HIGH
          break;
        default:
          break;
      }
      break;
    case 6:
      resetLights();
      Serial.println("Case 6");
      int ir_val = analogRead(0);
      if (ir_val > 200 && ir_val <= 300) {
        digitalWrite(red_pin, HIGH); // Go away, you're too close
      } else if (ir_val > 300 && ir_val <= 400) {
        digitalWrite(yellow_pin, HIGH); // Ehhh, you're a tad close
      } else if (ir_val > 400 && ir_val <= 500) {
        digitalWrite(green_pin, HIGH); // All good!
      } // else, I have no idea where you are
      break;
    default:
      Serial.println("Default case >:("); // <- Emma
      resetLights();
      break;
  }
}

bool checkSwitch(int button_val, int prev_button_val) {
  if (button_val == 1 && prev_button_val == 0) {
    return true;
  } else {
    return false;
  }
}

void resetLights() {
  digitalWrite(green_pin, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(yellow_pin, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(red_pin, LOW);   // turn the LED on (HIGH is the voltage level)
}
