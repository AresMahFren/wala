// Pin Definitions
#define DATA_PIN 2   // DS - Serial Data Input
#define LATCH_PIN 3  // ST_CP - Latch Pin
#define CLOCK_PIN 4  // SH_CP - Clock Pin

// Display Control Pins
#define DS1 5  // First Display - Tens Digit
#define DS2 6  // First Display - Ones Digit
#define DS3 7  // Second Display - Tens Digit
#define DS4 8 // Second Display - Ones Digit

// Traffic Light Shift Register Outputs (Q0 - Q5)
#define GREEN_1  0
#define YELLOW_1 1
#define RED_1    2
#define GREEN_2  3
#define YELLOW_2 4
#define RED_2    5

// Binary Calculator Pin Definitions
#define BUTTON A0  // Push button for operation selection
#define LED_BLUE 12  // Blue - Addition
#define LED_YELLOW 11  // Yellow - Subtraction
#define LED_GREEN 10  // Green - Multiplication
#define LED_RED 9  // Red - Division

// DIP Switch Inputs (3-bit numbers)
#define B1 13
#define B2 A1
#define B3 A2
#define B4 A3
#define B5 A4
#define B6 A5

// Inverted 7-segment digit mappings (Common Cathode → HIGH means ON)
const byte digitMap[10] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

// Modes
enum Mode { TRAFFIC_LIGHT, BINARY_CALCULATOR };
Mode currentMode = BINARY_CALCULATOR;

// Binary Calculator Variables

// Independent timers for each display
int time1 = 30;  // Countdown for Display 1
int time2 = 27;  // Countdown for Display 2
unsigned long prevTime1, prevTime2;
bool buttonPressed = false;

// Active traffic light states
int state1 = RED_1;
int state2 = GREEN_2;

int operation = 0; // either +,-,*,/
unsigned long buttonPressTime = 0;  // For detecting long press
bool buttonState = false; // To store the time the button is pressed
bool lastButtonState = false; // Previous state of the button (pressed or not)
bool mode = false; //False for mode 1, True for mode 2
int lastResult = -1;  // Stores the last computed result
bool resultAvailable = false; // Flag to check if result should be displayed

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DS1, OUTPUT);
  pinMode(DS2, OUTPUT);
  pinMode(DS3, OUTPUT);
  pinMode(DS4, OUTPUT);

  // Turn off all digits initially
  digitalWrite(DS1, HIGH);
  digitalWrite(DS2, HIGH);
  digitalWrite(DS3, HIGH);
  digitalWrite(DS4, HIGH);

   // DIP Switch Setup
  pinMode(B1, INPUT);
  pinMode(B2, INPUT);
  pinMode(B3, INPUT);
  pinMode(B4, INPUT);
  pinMode(B5, INPUT);
  pinMode(B6, INPUT);

  // Set button and LED pins
  pinMode(BUTTON, INPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  prevTime1 = millis();
  prevTime2 = millis();



}

void loop() {
  if (resultAvailable && currentMode == BINARY_CALCULATOR) {
    displayResult(lastResult); // ✅ Continuously show last result
  }
   bool currentButtonState = digitalRead(BUTTON) == HIGH;  // Read the button state (HIGH for pressed)


  
    if (currentButtonState != lastButtonState) {
    if (currentButtonState == true) {
      // Button just pressed
      buttonPressTime = millis();  // Record the time the button was pressed
      switch(operation){
          case 0: //addition
            digitalWrite(LED_BLUE, HIGH);
            digitalWrite(LED_YELLOW, LOW);
            digitalWrite(LED_GREEN, LOW);
            digitalWrite(LED_RED, LOW);
            binaryCalculatorLoop();
            break;
          case 1://subtraction
            digitalWrite(LED_YELLOW, HIGH);
            digitalWrite(LED_BLUE, LOW);
            digitalWrite(LED_GREEN, LOW);
            digitalWrite(LED_RED, LOW);
            binaryCalculatorLoop();
            break;
          case 2://multiplication
            digitalWrite(LED_GREEN, HIGH);
            digitalWrite(LED_BLUE, LOW);
            digitalWrite(LED_YELLOW, LOW);
            digitalWrite(LED_RED, LOW);
            binaryCalculatorLoop();
            break;
          case 3://division
            digitalWrite(LED_RED, HIGH);
            digitalWrite(LED_BLUE, LOW);
            digitalWrite(LED_GREEN, LOW);
            digitalWrite(LED_YELLOW, LOW);
            binaryCalculatorLoop();
            break;
          default://clear
          operation = -1;
            digitalWrite(LED_RED, LOW);
            digitalWrite(LED_BLUE, LOW);
            digitalWrite(LED_GREEN, LOW);
            digitalWrite(LED_YELLOW, LOW);

        }
    } else {
      // Button just released
      if (millis() - buttonPressTime < 5000) {
        // If the button was pressed for less than 5 seconds, increment the operation
        operation++;
        Serial.print("Operation: ");
        Serial.println(operation);
        
      }
      
    }
  }

  // Check if the button is held for 5 seconds
  if (currentButtonState == true && millis() - buttonPressTime >= 4000) {
    // Switch mode after 5 seconds of holding the button
    if (!mode) {
      mode = true;  // Switch to mode 2
      Serial.println("Switched to Mode 2");
      currentMode = TRAFFIC_LIGHT;
            digitalWrite(LED_BLUE, LOW);
            digitalWrite(LED_YELLOW, LOW);
            digitalWrite(LED_GREEN, LOW);
            digitalWrite(LED_RED, LOW);
            trafficLightLoop();
      
    }
  }
  // digitalWrite(LED_BLUE, HIGH);
  // Update the last button state for the next loop iteration
  lastButtonState = currentButtonState;
}

void disableTrafficLights() {
  // Turn off all traffic light shift register outputs
  updateShiftRegisters(0b00000000, 0b00000000);

  // Clear display
  digitalWrite(DS1, LOW);
  digitalWrite(DS2, LOW);
  digitalWrite(DS3, HIGH);
  digitalWrite(DS4, HIGH);
}

void trafficLightLoop() {
  unsigned long prevTime1 = millis();
  unsigned long prevTime2 = millis();

  while (currentMode == TRAFFIC_LIGHT) {  // ✅ Check if we are still in Traffic Light mode
    unsigned long currentTime = millis();

    // Check if the button is held for 5 seconds → Switch to Binary Calculator Mode
    if (digitalRead(BUTTON) == HIGH && millis() - buttonPressTime >= 5000) {
      currentMode = BINARY_CALCULATOR;
      buttonPressTime=0;
      mode = false;
      Serial.println("Switched to Binary Calculator Mode");
      restoreTrafficLights(); 
      disableTrafficLights();
       // Disable traffic light operation
      return; // Exit loop and switch mode
    }

    // Countdown for Display 1
    if (currentTime - prevTime1 >= 1000) {
      prevTime1 = currentTime;

      if (time1 > 1) {
        time1--;
      } else {
        switchLight1();
      }
    }

    // Countdown for Display 2
    if (currentTime - prevTime2 >= 1000) {
      prevTime2 = currentTime;

      if (time2 > 1) {
        time2--;
      } else {
        switchLight2();
      }
    }

    // Update display every loop
    displayTwoDigit(time1, time2, state1, state2);
  }
}

void restoreTrafficLights() {
  // Reset the traffic light timer values
  time1 = 30;
  time2 = 27;
  prevTime1 = millis();
  prevTime2 = millis();

  // Restore traffic light system
  displayTwoDigit(time1, time2, state1, state2);
}

void binaryCalculatorLoop() {
  if (currentMode == BINARY_CALCULATOR) {
    disableTrafficLights();  // ✅ Ensure traffic lights are OFF

    int num1 = (digitalRead(B1) << 2) | (digitalRead(B2) << 1) | digitalRead(B3);
    int num2 = (digitalRead(B4) << 2) | (digitalRead(B5) << 1) | digitalRead(B6);

    lastResult = calculate(num1, num2); // Store result
    resultAvailable = true;  // ✅ Set flag to keep displaying
  }
}


void displayResult(int result) {
  result = constrain(result, 0, 99); // Ensure within range

  int tens = result / 10;
  int ones = result % 10;

  for (int i = 0; i < 5; i++) {  // Refresh multiple times for persistence
    // Display Tens Digit (DS1)
    digitalWrite(DS1, LOW);
    updateShiftRegisters(digitMap[tens], 0b00000000);
    delay(10);  
    digitalWrite(DS1, HIGH);

    // Display Ones Digit (DS2)
    digitalWrite(DS2, LOW);
    updateShiftRegisters(digitMap[ones], 0b00000000);
    delay(10);
    digitalWrite(DS2, HIGH);
  }
}


int calculate(int num1, int num2) {
  switch (operation) {
    case 0: return num1 + num2;
    case 1: return num1 - num2;
    case 2: return num1 * num2;
    case 3: return (num2 == 0) ? 0 : num1 / num2;
    default: operation = -1;
  }
  return 0;
}

// Function to switch lights for Display 1
void switchLight1() {
  if (state1 == RED_1) {
    state1 = GREEN_1;
    time1 = 27;
  } else if (state1 == GREEN_1) {
    state1 = YELLOW_1;
    time1 = 3;
  } else if (state1 == YELLOW_1) {
    state1 = RED_1;
    time1 = 30;
  }
}

// Function to switch lights for Display 2
void switchLight2() {
  if (state2 == GREEN_2) {
    state2 = YELLOW_2;
    time2 = 3;
  } else if (state2 == YELLOW_2) {
    state2 = RED_2;
    time2 = 30;
  } else if (state2 == RED_2) {
    state2 = GREEN_2;
    time2 = 27;
  }
}

// Function to display numbers & control traffic lights
void displayTwoDigit(int num1, int num2, int activeLight1, int activeLight2) {
  int tens1 = num1 / 10;
  int ones1 = num1 % 10;
  int tens2 = num2 / 10;
  int ones2 = num2 % 10;

  byte ledState = (1 << activeLight1) | (1 << activeLight2);

  digitalWrite(DS1, LOW);
  updateShiftRegisters(digitMap[tens1], ledState);
  delay(5);
  digitalWrite(DS1, HIGH);

  digitalWrite(DS2, LOW);
  updateShiftRegisters(digitMap[ones1], ledState);
  delay(5);
  digitalWrite(DS2, HIGH);

  digitalWrite(DS3, LOW);
  updateShiftRegisters(digitMap[tens2], ledState);
  delay(5);
  digitalWrite(DS3, HIGH);

  digitalWrite(DS4, LOW);
  updateShiftRegisters(digitMap[ones2], ledState);
  delay(5);
  digitalWrite(DS4, HIGH);
}

// Function to update shift registers
void updateShiftRegisters(byte displayData, byte ledData) {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, ledData);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, displayData);
  digitalWrite(LATCH_PIN, HIGH);
}


