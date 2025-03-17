// Pin Definitions
#define DATA_PIN 4   // DS - Serial Data Input
#define LATCH_PIN 6  // ST_CP - Latch Pin
#define CLOCK_PIN 5  // SH_CP - Clock Pin

// Display Enable Pins
#define DS1 7  // First display (tens)
#define DS2 8  // First display (ones)
#define DS3 9  // Second display (tens)
#define DS4 10 // Second display (ones)

// 7-segment display digit mappings (Common Cathode)
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

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(DS1, OUTPUT);
  pinMode(DS2, OUTPUT);
  pinMode(DS3, OUTPUT);
  pinMode(DS4, OUTPUT);
  
  // Ensure all displays are off initially
  digitalWrite(DS1, LOW);
  digitalWrite(DS2, LOW);
  digitalWrite(DS3, LOW);
  digitalWrite(DS4, LOW);
}

void loop() {
  countdown(30, DS1, DS2); // First display: Countdown from 30
  countdown(40, DS3, DS4); // Second display: Countdown from 40
}

void countdown(int time, int tensPin, int onesPin) {
  for (int t = time; t >= 0; t--) {
    displayNumber(t, tensPin, onesPin);
    delay(1000); // 1-second delay per countdown step
  }
}

void displayNumber(int num, int tensPin, int onesPin) {
  int tens = num / 10;
  int ones = num % 10;

  digitalWrite(LATCH_PIN, LOW);

  // Send both digits at the same time
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, digitMap[tens]); // Send tens first
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, digitMap[ones]); // Send ones second

  digitalWrite(LATCH_PIN, HIGH);

  // Enable both digits at the same time
  digitalWrite(tensPin, HIGH);
  digitalWrite(onesPin, HIGH);
  delay(5);
  digitalWrite(tensPin, LOW);
  digitalWrite(onesPin, LOW);
}
