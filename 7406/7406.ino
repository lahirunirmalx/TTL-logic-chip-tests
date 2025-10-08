
// This code:

// * Toggles each input HIGH/LOW.
// * Reads the inverted output.
// * Checks if the inversion is correct.
 
// 7406 Hex Inverter (Open Collector) Test


int inputs[4]  = {4, 5, 6, 7};      // Arduino pins connected to 7406 inputs
int outputs[4] = {8, 9, 10, 11};    // Arduino pins connected to 7406 outputs (with pull-ups)

void setup() {
  Serial.begin(9600);
  Serial.println("Starting 7406 Test...");

  // Setup input pins (Arduino driving them)
  for (int i = 0; i < 4; i++) {
    pinMode(inputs[i], OUTPUT);
  }

  // Setup output pins (reading from 7406)
  for (int i = 0; i < 4; i++) {
    pinMode(outputs[i], INPUT);
  }
}

void loop() {
  bool allPass = true;
  Serial.println("Testing inversion...");

  for (int i = 0; i < 4; i++) {
    // Test LOW input -> expect HIGH output
    digitalWrite(inputs[i], LOW);
    delay(5);
    int readHigh = digitalRead(outputs[i]);
    if (readHigh != HIGH) {
      Serial.print("FAIL channel "); Serial.print(i+1);
      Serial.println(" (Expected HIGH when input LOW)");
      allPass = false;
    }

    // Test HIGH input -> expect LOW output
    digitalWrite(inputs[i], HIGH);
    delay(5);
    int readLow = digitalRead(outputs[i]);
    if (readLow != LOW) {
      Serial.print("FAIL channel "); Serial.print(i+1);
      Serial.println(" (Expected LOW when input HIGH)");
      allPass = false;
    }
  }

  if (allPass) {
    Serial.println("✅ All inverter channels OK!");
  } else {
    Serial.println("❌ Some channels FAILED!");
  }

  Serial.println("-------------------------");
  delay(2000);
}