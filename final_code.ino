#include <Servo.h>

// --- Solar Tracking ---
#define LDR_LEFT  A0   // Left LDR
#define LDR_RIGHT A1   // Right LDR
#define OFFSET 30
#define ERROR 60

Servo servo;
int Spoint = 90; // Start from center position

// --- Smart Irrigation ---
int sensor_pin = A4;
int sensorValue;
int moisturePercent;
const int pumpPin = 10;

void setup() {
  // Servo setup
  servo.attach(11);
  servo.write(Spoint);

  // Pump setup
  pinMode(pumpPin, OUTPUT);

  // Serial Monitor
  Serial.begin(9600);
  Serial.println("System Initializing...");
}

void loop() {
  // --- Solar Tracking ---
  int leftValue  = analogRead(LDR_LEFT);
  int rightValue = analogRead(LDR_RIGHT);
  int diff = leftValue - rightValue - OFFSET;

  if (abs(diff) > ERROR) {
    if (diff > 0) {
      Spoint = Spoint - 1;  // move left
    } else {
      Spoint = Spoint + 1;  // move right
    }
  }

  Spoint = constrain(Spoint, 0, 180);
  servo.write(Spoint);

  // Debug info for solar tracking
  Serial.print("LDR L: ");
  Serial.print(leftValue);
  Serial.print(" | LDR R: ");
  Serial.print(rightValue);
  Serial.print(" | Diff: ");
  Serial.print(diff);
  Serial.print(" | Servo: ");
  Serial.print(Spoint);
  Serial.print(" || ");

  // --- Soil Moisture Reading ---
  sensorValue = analogRead(sensor_pin);
  moisturePercent = map(sensorValue, 1023, 0, 0, 100); // Calibrated

  Serial.print("Soil Moisture: ");
  Serial.print(moisturePercent);
  Serial.print("% | Condition: ");

  if (moisturePercent < 30) {
    Serial.println("Dry Soil");
    digitalWrite(pumpPin, LOW);  // Pump OFF
  } 
  else if (moisturePercent <= 60) {
    Serial.println("Moist Soil");
    digitalWrite(pumpPin, HIGH); // Pump ON
  } 
  else {
    Serial.println("Wet Soil");
    digitalWrite(pumpPin, HIGH); // Pump ON
  }

  delay(50); // Delay between loops
}
