// Pin Definitions
#define DATA_PIN 4   // DS - Serial Data Input
#define LATCH_PIN 6  // ST_CP - Latch Pin
#define CLOCK_PIN 5  // SH_CP - Clock Pin

// Display Control Pins
#define DS1 7  // First Display - Tens Digit
#define DS2 8  // First Display - Ones Digit
#define DS3 9  // Second Display - Tens Digit
#define DS4 10 // Second Display - Ones Digit

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

// Countdown Values
int firstDisplay = 20;  // First 2-digit display countdown
int secondDisplay = 10; // Second 2-digit display countdown

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
  while (true) {
    countdown(20, 10); // Phase 1
    countdown(3, 3);   // Phase 2 (both count 3s together)
    countdown(10, 20); // Phase 3
  }
}

// Function to handle countdown
void countdown(int first, int second) {
  for (int i = first, j = second; i >= 0 || j >= 0; i--, j--) {
    unsigned long startTime = millis();
    
    while (millis() - startTime < 1000) {  // Keep updating for 1 second
      displayTwoDigit(i >= 0 ? i : 0, j >= 0 ? j : 0);
    }
  }
}

// Function to display a two-digit number on both displays
void displayTwoDigit(int num1, int num2) {
  int tens1 = num1 / 10;
  int ones1 = num1 % 10;
  int tens2 = num2 / 10;
  int ones2 = num2 % 10;

  // Display First Display Tens
  digitalWrite(DS1, LOW);
  digitalWrite(DS2, HIGH);
  digitalWrite(DS3, HIGH);
  digitalWrite(DS4, HIGH);
  updateShiftRegisters(digitMap[tens1]);
  delay(5);
  digitalWrite(DS1, HIGH);

  // Display First Display Ones
  digitalWrite(DS2, LOW);
  digitalWrite(DS1, HIGH);
  digitalWrite(DS3, HIGH);
  digitalWrite(DS4, HIGH);
  updateShiftRegisters(digitMap[ones1]);
  delay(5);
  digitalWrite(DS2, HIGH);

  // Display Second Display Tens
  digitalWrite(DS3, LOW);
  digitalWrite(DS1, HIGH);
  digitalWrite(DS2, HIGH);
  digitalWrite(DS4, HIGH);
  updateShiftRegisters(digitMap[tens2]);
  delay(5);
  digitalWrite(DS3, HIGH);

  // Display Second Display Ones
  digitalWrite(DS4, LOW);
  digitalWrite(DS1, HIGH);
  digitalWrite(DS2, HIGH);
  digitalWrite(DS3, HIGH);
  updateShiftRegisters(digitMap[ones2]);
  delay(5);
  digitalWrite(DS4, HIGH);
}

// Function to update shift registers
void updateShiftRegisters(byte value) {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, value);
  digitalWrite(LATCH_PIN, HIGH);
}
