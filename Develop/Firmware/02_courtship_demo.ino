/*
  02_courtship_demo.ino — Firefly Jar, Meadow + courtship demo

  Implements:
    - Meadow background: all 6 LEDs flash independently on their own
      jittered ~5.9s cadence (FLASH_PERIOD_S), smooth rise/fall fades.
    - The "courtship" micro-event from Develop/DevNote-Courtship-Sequence.md
      (asset 6080015) - the model Design/DD-9-Glow-Pattern.md's
      2026-10-12 addendum reconciled on, after it conflicted with the
      earlier flash-pattern simulator (6080014). An occasional male J
      (dip on a lower LED, handoff to the one above it), a dim answer
      from LED6 (always the "female"), repeated with the male's pair
      starting one LED lower each time, ending in a single-LED "landing"
      flash.

  Deliberately NOT included (see Develop/Assembly.md #test-code):
    - IMU wake/sleep power management, IMU libraries of any kind.
    - The night-light/toy state machine (FR-2..FR-4) and mode-cycling.
    - The low-battery cue.
  This sketch just runs Meadow + courtship continuously on USB power,
  so the shape and timing of the pattern can be watched and tuned on
  real hardware before those constants get locked into the full
  state-machine firmware.

  One known simplification vs. the DevNote's own pseudocode: its
  landing-trigger formula ("male+1 == FEMALE_LED") doesn't actually
  match its own worked example table (which lands only on the 3rd
  pass). This sketch lands on pass 3 unconditionally, matching the
  table - see Develop/Assembly.md for the full note.

  Pin map: Design/Spec-2.md #pin-map-xiao-nrf52840-sense.
*/

#include <Arduino.h>

// ---------------------------------------------------------------------
// Data Model constants - Design/Spec-2.md + Develop/DevNote-Courtship-Sequence.md
// LED numbers below are 1-indexed (LED1..LED6) to match the DevNote
// exactly; ledPin()/ledIdx() convert to the 0-indexed LED_PINS array.
// ---------------------------------------------------------------------
const uint8_t LED_PINS[6] = {D2, D3, D4, D5, D6, D7};

const float    FLASH_PERIOD_S_NOM   = 5.9f;
const float    FLASH_PERIOD_S_JIT   = 0.6f;
const uint16_t PULSE_RISE_MS        = 150;
const uint16_t PULSE_FALL_MS        = 350;
const float    J_DIP_LEVEL          = 0.30f;
const uint16_t J_HANDOFF_MS         = 100;
const float    LANDING_LEVEL        = 0.60f;
const uint8_t  FEMALE_LED           = 6;     // 1-indexed: lowest-hanging LED
const float    FEMALE_GAIN          = 0.10f;
const uint16_t ANSWER_DELAY_MS_NOM  = 2100;
const uint16_t ANSWER_DELAY_MS_JIT  = 200;
const uint16_t COURTSHIP_EVERY_S_MIN = 60;
const uint16_t COURTSHIP_EVERY_S_MAX = 120;
const uint16_t PAIR_REST_S_MIN      = 15;
const uint16_t PAIR_REST_S_MAX      = 30;

inline uint8_t ledIdx(uint8_t led1indexed) { return led1indexed - 1; }
inline uint8_t ledPin(uint8_t led1indexed) { return LED_PINS[ledIdx(led1indexed)]; }

// ---------------------------------------------------------------------
// Per-LED non-blocking fade envelope. Every LED - whether running its
// own independent Meadow cadence or taking part in a courtship - is
// just "a pulse with a peak level and a rise/fall shape" to this layer.
// ---------------------------------------------------------------------
struct LedEnvelope {
  bool     active = false;
  uint32_t startMs = 0;
  uint16_t riseMs = PULSE_RISE_MS;
  uint16_t fallMs = PULSE_FALL_MS;
  float    peak = 1.0f;
};
LedEnvelope env[6];

void startPulse(uint8_t led1indexed, float peak,
                 uint16_t riseMs = PULSE_RISE_MS, uint16_t fallMs = PULSE_FALL_MS) {
  uint8_t i = ledIdx(led1indexed);
  env[i].active = true;
  env[i].startMs = millis();
  env[i].riseMs = riseMs;
  env[i].fallMs = fallMs;
  env[i].peak = peak;
}

float smoothstep(float x) {
  if (x < 0) x = 0; if (x > 1) x = 1;
  return x * x * (3 - 2 * x);
}

// advances one LED's envelope and writes its PWM; returns true while still lit
bool updateLedEnvelope(uint8_t i) {
  if (!env[i].active) return false;
  uint32_t t = millis() - env[i].startMs;
  float level;
  if (t < env[i].riseMs) {
    level = smoothstep((float)t / env[i].riseMs);
  } else if (t < (uint32_t)env[i].riseMs + env[i].fallMs) {
    level = 1.0f - smoothstep((float)(t - env[i].riseMs) / env[i].fallMs);
  } else {
    env[i].active = false;
    analogWrite(LED_PINS[i], 0);
    return false;
  }
  analogWrite(LED_PINS[i], (uint8_t)(255.0f * env[i].peak * level));
  return true;
}

// ---------------------------------------------------------------------
// Independent Meadow timers - one per LED. A LED claimed by the
// courtship state machine (courtshipOwns[]) is skipped here so the two
// systems never fight over the same pin; every other LED keeps its own
// cadence the whole time, so the jar never goes fully dark.
// ---------------------------------------------------------------------
uint32_t nextMeadowMs[6];
bool     courtshipOwns[6] = {false, false, false, false, false, false};

uint32_t jitteredMs(float nomS, float jitS) {
  float s = nomS + (random(-1000, 1001) / 1000.0f) * jitS;
  if (s < 0.5f) s = 0.5f;
  return (uint32_t)(s * 1000.0f);
}

void scheduleMeadow(uint8_t i /*0-indexed*/) {
  nextMeadowMs[i] = millis() + jitteredMs(FLASH_PERIOD_S_NOM, FLASH_PERIOD_S_JIT);
}

void updateMeadow() {
  uint32_t now = millis();
  for (uint8_t i = 0; i < 6; i++) {
    if (courtshipOwns[i]) continue;
    if (now >= nextMeadowMs[i] && !env[i].active) {
      startPulse(i + 1, 1.0f); // full-brightness independent flash
      scheduleMeadow(i);
    }
  }
}

// ---------------------------------------------------------------------
// Courtship state machine - Develop/DevNote-Courtship-Sequence.md
// ---------------------------------------------------------------------
enum CourtshipStage { CS_IDLE, CS_DIP, CS_WAIT_FOR_ANSWER, CS_WAIT_NEXT_PASS, CS_RESTING };
CourtshipStage csStage = CS_IDLE;
uint32_t csEventAtMs = 0;    // when IDLE: next courtship start time
uint32_t csDeadlineMs = 0;   // generic "do the next thing at this time" for the active stages
uint8_t  csMaleLow = 0;      // 1-indexed low LED of the current J (dips first)
uint8_t  csPass = 0;         // 0,1,2 - pass 2 always lands
bool     csLanding = false;

void scheduleCourtship() {
  uint32_t s = random(COURTSHIP_EVERY_S_MIN, COURTSHIP_EVERY_S_MAX + 1);
  csEventAtMs = millis() + s * 1000UL;
}

void claimPairForCourtship(uint8_t low, uint8_t high) {
  courtshipOwns[ledIdx(low)] = true;
  if (high >= 1 && high <= 6) courtshipOwns[ledIdx(high)] = true;
  courtshipOwns[ledIdx(FEMALE_LED)] = true;
}

void releaseAllFromCourtship() {
  for (uint8_t i = 0; i < 6; i++) courtshipOwns[i] = false;
  for (uint8_t i = 0; i < 6; i++) scheduleMeadow(i); // rejoin Meadow on a fresh timer
}

void beginCourtshipPass() {
  uint8_t high = csMaleLow - 1; // one LED higher = one step toward the lid
  csLanding = (csPass == 2);
  claimPairForCourtship(csMaleLow, csLanding ? 0 : high);

  startPulse(csMaleLow, J_DIP_LEVEL); // the dip, always on the low LED
  csStage = CS_DIP;
  csDeadlineMs = millis() + J_HANDOFF_MS;
}

void updateCourtship() {
  uint32_t now = millis();
  switch (csStage) {

    case CS_IDLE:
      if (now >= csEventAtMs) {
        csMaleLow = FEMALE_LED - 2; // start 2 LEDs above the female, e.g. LED4
        csPass = 0;
        beginCourtshipPass();
      }
      break;

    case CS_DIP:
      if (now >= csDeadlineMs) {
        if (csLanding) {
          startPulse(csMaleLow, LANDING_LEVEL); // single small flash, no handoff
        } else {
          startPulse(csMaleLow - 1, 1.0f);      // peak on the LED one step up
        }
        // peak lands ~PULSE_RISE_MS after this pulse starts; the female's
        // ANSWER_DELAY_MS is measured from that peak, not from pulse onset.
        csDeadlineMs = now + PULSE_RISE_MS
                     + jitteredMs(ANSWER_DELAY_MS_NOM / 1000.0f, ANSWER_DELAY_MS_JIT / 1000.0f);
        csStage = CS_WAIT_FOR_ANSWER;
      }
      break;

    case CS_WAIT_FOR_ANSWER:
      if (now >= csDeadlineMs) {
        startPulse(FEMALE_LED, FEMALE_GAIN); // her dim answer
        if (csLanding) {
          csStage = CS_RESTING;
          csDeadlineMs = now + random(PAIR_REST_S_MIN, PAIR_REST_S_MAX + 1) * 1000UL;
        } else {
          csMaleLow += 1; // next J starts one LED lower - the approach
          if (csMaleLow > FEMALE_LED - 1) csMaleLow = FEMALE_LED - 1; // never advance onto her own LED
          csPass += 1;
          csStage = CS_WAIT_NEXT_PASS;
          // respects MALE_SPACING_MIN_S trivially: FLASH_PERIOD_S_NOM (5.9s)
          // already exceeds the 3.5s minimum between one LED's own flashes.
          csDeadlineMs = now + jitteredMs(FLASH_PERIOD_S_NOM, FLASH_PERIOD_S_JIT);
        }
      }
      break;

    case CS_WAIT_NEXT_PASS:
      if (now >= csDeadlineMs) beginCourtshipPass();
      break;

    case CS_RESTING:
      if (now >= csDeadlineMs) {
        releaseAllFromCourtship();
        scheduleCourtship();
        csStage = CS_IDLE;
      }
      break;
  }
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(A0) ^ micros()); // A0 floats on D0; fine for jitter, not crypto

  for (uint8_t i = 0; i < 6; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    analogWrite(LED_PINS[i], 0);
    scheduleMeadow(i);
  }
  scheduleCourtship();

  Serial.println(F("Firefly Jar: Meadow + courtship demo running."));
  Serial.println(F("See Develop/DevNote-Courtship-Sequence.md for the sequence this implements."));
}

void loop() {
  updateMeadow();
  updateCourtship();
  for (uint8_t i = 0; i < 6; i++) updateLedEnvelope(i);
}
