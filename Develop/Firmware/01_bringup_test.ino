/*
  01_bringup_test.ino — Firefly Jar, hardware bring-up test

  Run this FIRST on every freshly soldered unit, before trusting any
  joint — flash it over USB before assembly even starts (Assembly.md
  step 2), and again after each stage that adds new wiring.

  What it does, on loop, printed to the Serial Monitor at 115200 baud:
    1. Lights each of the 6 LEDs individually (others off) so you can
       confirm D2..D7 actually drive the LED you think they do, matching
       Develop/Assembly-media/wiring-diagram.svg.
    2. Samples the LDR divider (D1 high -> settle -> read D0 -> D1 low)
       and prints the raw ADC value. Cover the LDR with a finger between
       loops; the number should drop noticeably. No DARK/LIGHT thresholds
       yet - those are explicitly "set in Destroy" per Spec-2's Data
       Model, once this prototype gives real numbers to calibrate from.
    3. Reads and prints battery voltage.

  Pin map: Design/Spec-2.md #pin-map-xiao-nrf52840-sense.

  Battery-voltage caveat: the P0.14 (enable) / P0.31 (ADC) pattern and
  the scale factor below follow Seeed's documented XIAO nRF52840 Sense
  battery-read circuit, but the exact pin macro names and divider ratio
  have varied slightly across board-package versions. If VBAT prints a
  wildly wrong number (e.g. 0.00 V or something far from ~3.7-4.2 V),
  check Seeed's wiki for the macro names in your installed board
  package version before assuming the circuit itself is at fault.
*/

#include <Arduino.h>

const uint8_t LED_PINS[6] = {D2, D3, D4, D5, D6, D7};
const char*   LED_NAMES[6] = {
  "LED1 (~6cm)", "LED2 (~9cm)", "LED3 (~12cm)",
  "LED4 (~15cm)", "LED5 (~18cm)", "LED6 (~21cm, FEMALE_LED)"
};

const uint8_t LDR_POWER_PIN = D1;
const uint8_t LDR_SENSE_PIN = D0;

// Seeed XIAO nRF52840 Sense battery-read pins (see caveat above).
const uint8_t VBAT_ENABLE_PIN = P0_14;
const uint8_t VBAT_SENSE_PIN  = P0_31;

void setup() {
  Serial.begin(115200);
  uint32_t waitStart = millis();
  while (!Serial && millis() - waitStart < 3000) { /* give a monitor up to 3s to attach, then proceed anyway */ }

  for (uint8_t i = 0; i < 6; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }
  pinMode(LDR_POWER_PIN, OUTPUT);
  digitalWrite(LDR_POWER_PIN, LOW);

  pinMode(VBAT_ENABLE_PIN, OUTPUT);
  digitalWrite(VBAT_ENABLE_PIN, HIGH); // disabled by default; drive LOW only to sample, per Seeed's pattern

  analogReadResolution(12);

  Serial.println();
  Serial.println(F("=== Firefly Jar bring-up test ==="));
  Serial.println(F("Wiring reference: Develop/Assembly-media/wiring-diagram.svg"));
  Serial.println();
}

void stepLeds() {
  for (uint8_t i = 0; i < 6; i++) {
    for (uint8_t j = 0; j < 6; j++) digitalWrite(LED_PINS[j], LOW);
    digitalWrite(LED_PINS[i], HIGH);
    Serial.print(F("LED on: "));
    Serial.print(LED_NAMES[i]);
    Serial.print(F("  (pin D"));
    Serial.print(i + 2);
    Serial.println(F(")"));
    delay(500);
  }
  for (uint8_t j = 0; j < 6; j++) digitalWrite(LED_PINS[j], LOW);
}

uint16_t sampleLdr() {
  digitalWrite(LDR_POWER_PIN, HIGH);
  delay(5); // let the divider settle
  uint16_t raw = analogRead(LDR_SENSE_PIN);
  digitalWrite(LDR_POWER_PIN, LOW);
  return raw;
}

float readBatteryVoltage() {
  digitalWrite(VBAT_ENABLE_PIN, LOW); // enable the divider
  delay(2);
  uint16_t raw = analogRead(VBAT_SENSE_PIN);
  digitalWrite(VBAT_ENABLE_PIN, HIGH); // disable again, don't leave it drawing current
  // 12-bit ADC, 3.0V reference, x2 divider is Seeed's commonly documented
  // scaling for this circuit - verify against your board package if the
  // result looks wrong (see the file header caveat).
  return (raw / 4095.0f) * 3.0f * 2.0f;
}

void loop() {
  Serial.println(F("--- LED step test ---"));
  stepLeds();

  Serial.println(F("--- LDR sample (cover it before the next loop to check response) ---"));
  uint16_t ldr = sampleLdr();
  Serial.print(F("LDR raw (0-4095): "));
  Serial.println(ldr);

  Serial.println(F("--- Battery ---"));
  float vbat = readBatteryVoltage();
  Serial.print(F("VBAT: "));
  Serial.print(vbat, 2);
  Serial.println(F(" V"));

  Serial.println();
  delay(1500);
}
