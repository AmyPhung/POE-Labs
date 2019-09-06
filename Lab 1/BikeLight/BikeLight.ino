/*
  BikeLight

  Written by Amy Phung and Emma Mack

  Contains code for a bike light that switches modes on button press. Modes
  include:
    Mode 1 (state Default): All lights off
    Mode 2 (state 0): Green light on
    Mode 3 (state 1): Yellow light on
    Mode 4 (state 2): Red light on
    Mode 5 (state 3): All lights on
    Mode 6 (state 4): All lights flashing
    Mode 7 (state 5): Wave pattern
    Mode 8 (state 6): Stop lights - red when close to IR, yellow when medium,
                      green when far

 */

// Define pins
int green_pin  = 13;
int yellow_pin = 12;
int red_pin    = 11;
int button_pin = 7;

// Setup state variables
int button_val = 0;      // 0 if button isn't currently pressed, 1 if button is pressed
int prev_button_val = 0; // Save previous value in order to tell if button has changed states

int state     = 0; // Current bike light mode to run
int sub_state = 0; // Current state within mode - used for flashing and wave patterns

// Timing Variables
unsigned long oldLoopTime = 0;    // Past loop time in milliseconds
unsigned long newLoopTime = 0;    // New loop time in milliseconds
unsigned long cycleTime = 0;      // Elapsed loop cycle time
const long controlLoopInterval = 200; // Control loop cycle time in milliseconds


// Startup/Reset Procedure
void setup() {
  Serial.begin(9600);
  // Initialize the digital pins for the LEDS as outputs
  pinMode(green_pin, OUTPUT);
  pinMode(yellow_pin, OUTPUT);
  pinMode(red_pin, OUTPUT);
  // Initialize the digitl pin for the button as an input
  pinMode(button_pin, INPUT);
}

// Main Loop
void loop() {
  // Check to see if button is pressed
  button_val = digitalRead(button_pin);

  // Switch state on button press
  if (checkSwitch(button_val, prev_button_val)) {
    state++;
    if (state >= 7) {
      state = 0;
    }
  }

  prev_button_val = button_val;

  // Run code for current state
  switch (state) {
    case 0: // Green light on
      resetLights();
      Serial.println("Case 0");
      digitalWrite(green_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
      break;
    case 1: // Yellow light on
      resetLights();
      Serial.println("Case 1");
      digitalWrite(yellow_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
      break;
    case 2: // Red light on
      resetLights();
      Serial.println("Case 2");
      digitalWrite(red_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
      break;
    case 3: // All lights on
      resetLights();
      Serial.println("Case 3");
      digitalWrite(green_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(yellow_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
      digitalWrite(red_pin, HIGH);   // turn the LED on (HIGH is the voltage level)
      break;
    case 4:  // Flashing lights
      resetLights();
      Serial.println("Case 4");
      newLoopTime = millis(); // get current Arduino time (50 days till wrap)
      // After specified loop time has passed, switch light sub-state
      if (newLoopTime - oldLoopTime >= controlLoopInterval) {
        oldLoopTime = newLoopTime; // reset time stamp
        // Switch between all on and all off
        if (sub_state == 0) {
          sub_state = 1;
        } else {
          sub_state = 0;
        }
      }
      // Change lights depending on sub-state
      if (sub_state == 0) {
        resetLights();
      } else {
        digitalWrite(green_pin, HIGH);    // turn the LED on by making the voltage HIGH
        digitalWrite(yellow_pin, HIGH);    // turn the LED on by making the voltage HIGH
        digitalWrite(red_pin, HIGH);    // turn the LED on by making the voltage HIGH
      }
      break;
    case 5: // Light wave
      resetLights();
      Serial.println("Case 5");
      newLoopTime = millis(); // get current Arduino time (50 days till wrap)
      // After specified loop time has passed, switch light sub-state
      if (newLoopTime - oldLoopTime >= controlLoopInterval) {
        oldLoopTime = newLoopTime; // reset time stamp
        sub_state++;
        if (sub_state >= 3) {
          sub_state = 0;
        }
      }
      resetLights();
      switch(sub_state) {
        case 0:
          digitalWrite(green_pin, HIGH); // turn the LED on by making the voltage HIGH
          break;
        case 1:
          digitalWrite(yellow_pin, HIGH); // turn the LED on by making the voltage HIGH
          break;
        case 2:
          digitalWrite(red_pin, HIGH); // turn the LED on by making the voltage HIGH
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
  // Turn all the LEDs off
  digitalWrite(green_pin, LOW);
  digitalWrite(yellow_pin, LOW);
  digitalWrite(red_pin, LOW);
}
