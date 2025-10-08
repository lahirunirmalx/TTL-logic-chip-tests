/*
  TMS4116 / 4116 DRAM Tester
  Board: Arduino UNO (5V)
  Rails: VDD +12V, VCC +5V, VBB -5V, VSS GND (externally supplied!)
  See README for wiring and safety.

  License: MIT
*/

#include <Arduino.h>

// -------- Pin map (see README) --------
const uint8_t PIN_RAS = A3;   // 4116 pin 4
const uint8_t PIN_CAS = 9;    // 4116 pin 15
const uint8_t PIN_WE  = A2;   // 4116 pin 3

const uint8_t PIN_DIN = A1;   // 4116 pin 2  (Arduino -> DRAM)
const uint8_t PIN_Q   = 8;    // 4116 pin 14 (DRAM -> Arduino)

// A0..A6 (multiplexed address bus)
const uint8_t PIN_A0 = A4;    // 4116 pin 5
const uint8_t PIN_A1 = 2;     // 4116 pin 7
const uint8_t PIN_A2 = A5;    // 4116 pin 6
const uint8_t PIN_A3 = 6;     // 4116 pin 10
const uint8_t PIN_A4 = 5;     // 4116 pin 11
const uint8_t PIN_A5 = 4;     // 4116 pin 12
const uint8_t PIN_A6 = 7;     // 4116 pin 13

const uint8_t ADDR_PINS[7] = { PIN_A0, PIN_A1, PIN_A2, PIN_A3, PIN_A4, PIN_A5, PIN_A6 };

// -------- Timing (ultra conservative; Arduino is slow) --------
const unsigned t_settle_us = 1;  // settle after setting address/data
const unsigned t_rcd_us    = 1;  // RAS->CAS delay
const unsigned t_cas_us    = 1;  // CAS low time
const unsigned t_ras_us    = 1;  // RAS low min

// ---------- Helpers ----------
inline void setAddr7(uint8_t v) {
  for (uint8_t i = 0; i < 7; i++) {
    digitalWrite(ADDR_PINS[i], (v >> i) & 1);
  }
}

inline void rasLow()  { digitalWrite(PIN_RAS, LOW);  }
inline void rasHigh() { digitalWrite(PIN_RAS, HIGH); }
inline void casLow()  { digitalWrite(PIN_CAS, LOW);  }
inline void casHigh() { digitalWrite(PIN_CAS, HIGH); }
inline void weLow()   { digitalWrite(PIN_WE,  LOW);  }
inline void weHigh()  { digitalWrite(PIN_WE,  HIGH); }

// Row/column are 0..127
uint8_t dram_read(uint8_t row, uint8_t col) {
  // Place row, open RAS
  setAddr7(row); delayMicroseconds(t_settle_us);
  weHigh();              // read
  rasLow();  delayMicroseconds(t_ras_us);

  // Place column, strobe CAS
  setAddr7(col); delayMicroseconds(t_rcd_us);
  casLow();  delayMicroseconds(t_cas_us);
  uint8_t bit = digitalRead(PIN_Q);
  casHigh();
  rasHigh();
  return bit & 1;
}

void dram_write(uint8_t row, uint8_t col, uint8_t bit) {
  // Place row, open RAS
  setAddr7(row); delayMicroseconds(t_settle_us);
  rasLow();  delayMicroseconds(t_ras_us);

  // Place column and data, assert WE then CAS (early write)
  setAddr7(col);
  digitalWrite(PIN_DIN, bit ? HIGH : LOW);
  delayMicroseconds(t_rcd_us);
  weLow();
  casLow();  delayMicroseconds(t_cas_us);
  casHigh();
  weHigh();
  rasHigh();
}

void refresh_all_rows() {
  // RAS-only refresh: open/close each row (no CAS)
  for (uint8_t row = 0; row < 128; row++) {
    setAddr7(row); delayMicroseconds(t_settle_us);
    rasLow();  delayMicroseconds(t_ras_us);
    rasHigh();
  }
}

bool pass_pattern(uint8_t (*pattern)(uint8_t row, uint8_t col), const char* name) {
  Serial.print(F("Writing ")); Serial.println(name);
  for (uint8_t r = 0; r < 128; r++) {
    for (uint8_t c = 0; c < 128; c++) {
      dram_write(r, c, pattern(r,c));
    }
  }

  Serial.print(F("Verifying ")); Serial.println(name);
  for (uint8_t r = 0; r < 128; r++) {
    for (uint8_t c = 0; c < 128; c++) {
      uint8_t expect = pattern(r,c);
      uint8_t got = dram_read(r, c);
      if (got != expect) {
        Serial.print(F("FAIL @ row="));  Serial.print(r);
        Serial.print(F(" col="));        Serial.print(c);
        Serial.print(F(" exp="));        Serial.print(expect);
        Serial.print(F(" got="));        Serial.println(got);
        return false;
      }
    }
  }
  Serial.println(F("OK"));
  return true;
}

// ---- Patterns ----
uint8_t pat_zero (uint8_t, uint8_t) { return 0; }
uint8_t pat_one  (uint8_t, uint8_t) { return 1; }
uint8_t pat_cb   (uint8_t r, uint8_t c) { return (r ^ c) & 1; }        // checkerboard
uint8_t pat_cbi  (uint8_t r, uint8_t c) { return ((r ^ c) & 1) ^ 1; }  // inverse
uint8_t pat_addr (uint8_t r, uint8_t c) { return ((r << 1) ^ c) & 1; } // pseudo-address

void setup() {
  Serial.begin(115200);
  Serial.println(F("\n4116 DRAM Tester"));

  // Configure pins
  pinMode(PIN_RAS, OUTPUT);
  pinMode(PIN_CAS, OUTPUT);
  pinMode(PIN_WE,  OUTPUT);
  pinMode(PIN_DIN, OUTPUT);
  pinMode(PIN_Q,   INPUT);

  for (uint8_t i = 0; i < 7; i++) pinMode(ADDR_PINS[i], OUTPUT);

  // Idle levels
  rasHigh(); casHigh(); weHigh();
  digitalWrite(PIN_DIN, LOW);
  for (uint8_t i = 0; i < 7; i++) digitalWrite(ADDR_PINS[i], LOW);

  Serial.println(F("Ensure VBB(-5V), VDD(+12V), VCC(+5V) are present (and sequenced) before continuing."));
  delay(1000);

  // Initial refresh sweep
  refresh_all_rows();

  bool ok = true;
  ok &= pass_pattern(pat_zero, "ALL-0");
  ok &= pass_pattern(pat_one,  "ALL-1");
  ok &= pass_pattern(pat_cb,   "CHECKER");
  ok &= pass_pattern(pat_cbi,  "CHECKER_INV");
  ok &= pass_pattern(pat_addr, "ADDR_LIKE");

  Serial.println(ok ? F("\nRESULT: PASS") : F("\nRESULT: FAIL (see first error above)"));
}

void loop() {
  // Idle; periodically refresh to keep contents alive if you want to re-read
  refresh_all_rows();
  delay(50);
}