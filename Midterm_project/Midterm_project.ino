#define DATA_PIN 4   // DS - Serial Data Input
#define LATCH_PIN 6  // ST_CP - Latch Pin
#define CLOCK_PIN 5  // SH_CP - Clock Pin
#define DS1 7        // First display, first digit
#define DS2 8        // First display, second digit
#define DS3 9        // Second display, first digit
#define DS4 10       // Second display, second digit

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
}

void loop() {
  for (int i = 99; i >= 0; i--) {  // Countdown from 99
    displayNumbers(i, i - 10);  // Display different numbers on each display
    delay(1000);
  }
}

void displayNumbers(int num1, int num2) {
  int tens1 = num1 / 10;
  int ones1 = num1 % 10;
  int tens2 = num2 / 10;
  int ones2 = num2 % 10;

  // Show first digit of first display
  digitalWrite(DS1, HIGH);
  digitalWrite(DS2, LOW);
  digitalWrite(DS3, LOW);
  digitalWrite(DS4, LOW);
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, digitMap[tens1]);
  digitalWrite(LATCH_PIN, HIGH);
  delay(5);  

  // Show second digit of first display
  digitalWrite(DS1, LOW);
  digitalWrite(DS2, HIGH);
  digitalWrite(DS3, LOW);
  digitalWrite(DS4, LOW);
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, digitMap[ones1]);
  digitalWrite(LATCH_PIN, HIGH);
  delay(5);

  // Show first digit of second display
  digitalWrite(DS1, LOW);
  digitalWrite(DS2, LOW);
  digitalWrite(DS3, HIGH);
  digitalWrite(DS4, LOW);
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, digitMap[tens2]);
  digitalWrite(LATCH_PIN, HIGH);
  delay(5);

  // Show second digit of second display
  digitalWrite(DS1, LOW);
  digitalWrite(DS2, LOW);
  digitalWrite(DS3, LOW);
  digitalWrite(DS4, HIGH);
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, digitMap[ones2]);
  digitalWrite(LATCH_PIN, HIGH);
  delay(5);
}
