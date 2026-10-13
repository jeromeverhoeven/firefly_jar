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

  Battery-voltage pins/scaling: verified against a working community
  library (honvl/Seeed-Xiao-NRF52840-Battery, xiaobattery.h) rather than
  guessed — the board package itself defines VBAT_ENABLE (pin 14, drive
  LOW to enable the divider) and PIN_VBAT (the ADC pin) directly, so
  there's no need to reference raw nRF52 port/pin names at all. (An
  earlier revision of this sketch used invented macros P0_14/P0_31,
  which don't exist in the board package and failed to compile — fixed
  here.) That reference library's scaling constant assumes the default
  (10-bit) ADC resolution, so this sketch deliberately never calls
  analogReadResolution() - see the comment in setup(). Even so, treat
  the first VBAT printout as provisional: check it against a multimeter
  reading of the actual cell before trusting it for anything (like the
  low-battery cue's 3.5 V threshold later on).

  Board package note: on the (non-mbed) "Seeed nRF52 Boards" package,
  Serial silently fails to LINK (not compile - you only find out at
  upload time) unless Adafruit_TinyUSB.h is explicitly included, even
  though nothing here calls it directly. That's the "undefined reference
  to Serial / Adafruit_USBD_CDC::begin" error this line fixes. The
  alternative is switching to the "mbed-enabled" board package instead -
  worth doing later when IMU work starts, not now (it costs more flash
  for nothing this sketch needs yet).
*/

#include <Adafruit_TinyUSB.h>
#include <Arduino.h>

const uint8_t LED_PINS[6] = {D2, D3, D4, D5, D6, D7};
const char*   LED_NAMES[6] = {
  "LED1 (~6cm)", "LED2 (~9cm)", "LED3 (~12cm)",
  "LED4 (~15cm)", "LED5 (~18cm)", "LED6 (~21cm, FEMALE_LED)"
};

const uint8_t LDR_POWER_PIN = D1;
const uint8_t LDR_SENSE_PIN = D0;

// VBAT_ENABLE and PIN_VBAT are defined by Seeed's board package itself
// (see the file header) - no local pin definitions needed for these two.

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

  pinMode(VBAT_ENABLE, OUTPUT);
  digitalWrite(VBAT_ENABLE, HIGH); // disabled by default; drive LOW only to sample, per Seeed's pattern
  pinMode(PIN_VBAT, INPUT);

  // Deliberately NOT calling analogReadResolution() here: the VBAT scaling
  // constant below was lifted from a verified reference implementation
  // that reads at the Arduino core's default resolution (10-bit on this
  // core). Changing the resolution would silently invalidate that constant
  // (12-bit counts read ~4x higher for the same voltage) - easy to miss
  // and exactly the kind of bug that bit the first pin-macro attempt here.
  // LDR_raw below is therefore also 0-1023, not 0-4095.

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
  digitalWrite(VBAT_ENABLE, LOW); // enable the divider
  delay(2);
  uint16_t raw = analogRead(PIN_VBAT);
  digitalWrite(VBAT_ENABLE, HIGH); // disable again, don't leave it drawing current
  // Scaling verified against honvl/Seeed-Xiao-NRF52840-Battery (xiaobattery.h):
  // mV-per-count constant below, then correct for the board's own divider ratio.
  const float VBAT_MV_PER_LSB = 0.003395996f;
  float adcVoltage = raw * VBAT_MV_PER_LSB;
  return adcVoltage * (1510.0f / 510.0f);
}

void loop() {
  Serial.println(F("--- LED step test ---"));
  stepLeds();

  Serial.println(F("--- LDR sample (cover it before the next loop to check response) ---"));
  uint16_t ldr = sampleLdr();
  Serial.print(F("LDR raw (0-1023): "));
  Serial.println(ldr);

  Serial.println(F("--- Battery ---"));
  float vbat = readBatteryVoltage();
  Serial.print(F("VBAT: "));
  Serial.print(vbat, 2);
  Serial.println(F(" V"));

  Serial.println();
  delay(1500);
}
