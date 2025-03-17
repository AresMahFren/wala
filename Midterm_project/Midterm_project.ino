// Pin Definitions
#define DATA_PIN 2   // DS - Serial Data Input
#define LATCH_PIN 4  // ST_CP - Latch Pin
#define CLOCK_PIN 3  // SH_CP - Clock Pin

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

// Independent timers for each display
int time1 = 30;  // Countdown for Display 1
int time2 = 27;  // Countdown for Display 2

// Active traffic light states
int state1 = RED_1;
int state2 = GREEN_2;

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
}

void loop() {
  unsigned long prevTime1 = millis();
  unsigned long prevTime2 = millis();

  while (true) {
    unsigned long currentTime = millis();

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
