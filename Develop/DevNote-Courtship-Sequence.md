# Development Note: Courtship Sequence (Male J-Flash and Female Answer)

**Asset:** [6080015](./6080015.md)
**Date:** 2026-10-10
**Author:** Jerome Verhoeven
**Related:** [DD-9 Glow Pattern and Modes](../Design/DD-9-Glow-Pattern.md) · [Spec-2 Data Model](../Design/Spec-2.md#data-model-firmware-configuration) · [Flash Pattern Simulator, 6080014](./6080014.md)

## Purpose
Define a "Courtship" event for the firmware's Meadow mode: one male *Photinus pyralis* (common eastern firefly) flashing his J-shaped signal, and a female answering from the grass. The jar's LEDs can't move, so this note records:
- the field data the sequence is based on,
- how the J-flight is faked with fixed LEDs,
- an exact timeline,
- proposed firmware constants.

## Field Data (*Photinus pyralis*)
| Behavior | Value | Source |
|---|---|---|
| Male flash duration | ~0.5 s single pulse, fast rise, slower fade (some observers: ~¾ s) | [Jordahl, citing Case 2004](https://kjordahl.codeberg.page/ffbehavior.html); [Silent Sparks](https://silentsparks.com/2018/06/02/a-guide-to-lightningbug-linguistics/) |
| Male flash interval | 5.9 ± 0.6 s (5-7 s); shorter on warmer nights | [Jordahl, citing Lloyd 1966](https://kjordahl.codeberg.page/ffbehavior.html); [Wikipedia](https://en.wikipedia.org/wiki/Photinus_pyralis) |
| Male flight | Dips, then flashes on the upswing, drawing a "J" ("big dipper firefly") | [The Biofiles](https://thebiofiles.com/info/4177) |
| Female answer delay | ~1-2 s; one measurement 2.1 ± 0.2 s | [Wikipedia](https://en.wikipedia.org/wiki/Photinus_pyralis); [Jordahl, citing Case 2004](https://kjordahl.codeberg.page/ffbehavior.html) |
| Female behavior | Stationary in vegetation; much dimmer than the male (one jar model uses 10x) | [Jordahl, citing Buck 1988](https://kjordahl.codeberg.page/ffbehavior.html) |
| Male approach | Flies toward the female's last flash and keeps signaling | [The Biofiles](https://thebiofiles.com/info/4177) |
| Rival males | A male flashes right after another only if ≥3.5 s since his own last flash; females tend to stop answering when a second male appears | [Jordahl, citing Buck 1988 and Case 2004](https://kjordahl.codeberg.page/ffbehavior.html) |
| Synchrony | Rare in *P. pyralis* (common in *P. carolinus*) | [Jordahl, citing Buck 1988](https://kjordahl.codeberg.page/ffbehavior.html) |
| Color | Yellow | [Wikipedia](https://en.wikipedia.org/wiki/Photinus_pyralis) |

## Illusion Approach (design choices, not biology)
- **J-stroke = LED hand-off.** The male's pulse starts dim on a lower LED and peaks on the LED one step higher. The eye reads a short upward streak.
- **Female = lowest LED.** LED 6 (~21 cm hang) sits nearest the moss, "in the grass." She is dim and never moves.
- **Approach = start lower each time.** Each successive J begins one LED lower, toward the female, and ends with a small "landing" flash near her.
- Hang lengths follow [Spec-2's pin map](../Design/Spec-2.md#pin-map-xiao-nrf52840-sense): LED 1 = 6 cm (highest) through LED 6 = 21 cm (lowest).

## Pulse Envelope (every flash)
- Rise ~120-150 ms, fall ~350 ms, about 0.5 s total.
- Shape: fast attack, exponential-like tail. The Jordahl model uses y = x·e^(−x²).
- Apply gamma correction to the PWM so the fade looks smooth to the eye.

## Sequence: One Courtship Exchange (~14 s)
| Time (s) | LED (hang) | Event | Peak |
|---|---|---|---|
| 0.00 | LED 4 (15 cm) | **Male J #1 begins**: faint ramp (the dip) | 30% |
| 0.10 | LED 4 → LED 3 (12 cm) | **Upswing hand-off**: LED 4 fades as LED 3 rises | — |
| 0.20 | LED 3 | **J peak** | 100% |
| 0.20-0.55 | LED 3 | Slow fade to dark | → 0 |
| 2.10 | LED 6 (21 cm) | **Female answers** (2.1 s after male peak) | ~10% |
| 3.00 | LED 1 (6 cm) | *Rival male* on his own cadence; female ignores him | 100% |
| 5.90 | LED 5 (18 cm) → LED 4 | **Male J #2**, starting one LED lower (approaching) | 30% → 100% |
| 8.00 | LED 6 | **Female answers again** | ~10% |
| 9.10 | LED 1 | Rival flashes again; no answer | 100% |
| 11.80 | LED 5 (18 cm) only | **Male "lands"** near her: smaller, single-LED flash | 60% |
| 13.90 | LED 6 | **Final answer**; pair rests 15-30 s | ~10% |

### Timeline (each column = 0.25 s)
```
time (s)   0       2       4       6       8       10      12      14
LED1  6 cm ············█·······················█·······················   rival male
LED3 12 cm ·█··························································   J #1 peak
LED4 15 cm ▁·······················█···································   J #1 dip / J #2 peak
LED5 18 cm ························▁······················▆············   J #2 dip / landing
LED6 21 cm ········▂·······················▂·······················▂···   female (dim)
```
Key: `▁` dip (~30%) · `█` peak (100%) · `▆` landing (~60%) · `▂` female (~10%)

## Rules
1. **Male cadence:** 5.9 ± 0.6 s, re-randomized every cycle, never metronomic.
2. **Female delay:** 2.1 ± 0.2 s after the courting male's **peak**. She answers only him.
3. **Spacing:** no male flashes within 3.5 s of his own previous flash.
4. **Approach:** each J starts one LED lower than the last. The landing flash ends the exchange.
5. **Rest:** after the final answer, the pair rests 15-30 s. The next courtship uses a different male LED.
6. **Background:** LEDs not in the courtship keep flashing as independent males (Meadow mode), so the jar never goes fully dark.
7. **Frequency:** courtship runs as an occasional event inside Meadow, about every 60-120 s, so kids catch it now and then.

## Proposed Firmware Constants
Additions to the [Spec-2 data model](../Design/Spec-2.md#data-model-firmware-configuration). Values are starting points to tune in the [simulator](./6080014.md) and on the prototype.

| Constant | Default | Unit | Notes |
|---|---|---|---|
| `PULSE_RISE_MS` | 150 | ms | Male and female |
| `PULSE_FALL_MS` | 350 | ms | Male and female |
| `J_DIP_LEVEL` | 0.30 | fraction | Lower LED's peak during the dip |
| `J_HANDOFF_MS` | 100 | ms | When the upper LED starts rising |
| `LANDING_LEVEL` | 0.60 | fraction | Final single-LED flash |
| `FEMALE_LED` | 6 | LED index | Lowest hanging LED |
| `FEMALE_GAIN` | 0.10 | fraction | Female peak relative to male |
| `ANSWER_DELAY_MS` | 2100 ± 200 | ms | From male **peak** (see open items) |
| `MALE_SPACING_MIN_S` | 3.5 | s | Per-male minimum between flashes |
| `COURTSHIP_EVERY_S` | 60-120 | s | Random interval between courtship events |
| `PAIR_REST_S` | 15-30 | s | Rest after an exchange |

Existing constants affected: `FLASH_PERIOD_S` (6 ± 1 → 5.9 ± 0.6), `FADE_MS` (symmetric 300-800 → `PULSE_RISE_MS` / `PULSE_FALL_MS`).

## Pseudocode
```c
void courtship(void) {
  int male = pickMaleAbove(FEMALE_LED, /*levelsAbove=*/2);   // e.g., LED 4
  for (int pass = 0; pass < 3; pass++) {
    bool landing = (male + 1 == FEMALE_LED) || pass == 2;
    if (landing) {
      pulse(male, LANDING_LEVEL);                            // single small flash
    } else {
      jFlash(/*low=*/male, /*high=*/male - 1);               // dip on lower LED, peak on upper
    }
    waitFromPeak(jitter(ANSWER_DELAY_MS, 200));
    pulse(FEMALE_LED, FEMALE_GAIN);                          // dim answer
    if (landing) break;
    male = male + 1;                                         // next J starts one LED lower
    waitUntilNextMaleSlot(jitter(5900, 600));                // respects MALE_SPACING_MIN_S
  }
  rest(randomBetween(15, 30));
}
```

## Open Items
- **Reconcile with the [simulator (6080014)](./6080014.md).** The simulator models the male as a longer multi-segment glow (1-2 s per segment) standing in for flight. It measures the female delay from the male's **onset** (default 2.0 s). This note uses a ~0.5 s pulse with an LED hand-off, measured from the **peak** (2.1 s). The two differ by ~0.2 s in delay reference and noticeably in male display length. Decide which model the firmware follows, then update DD-9 and Spec-2.
- Confirm on the prototype that the hand-off reads as upward motion at a 3 cm LED spacing. If not, try 2-step hand-offs (three LEDs) or a longer `J_HANDOFF_MS`.
- Check `FEMALE_GAIN` = 0.10 against the 1 kΩ resistor. At ~1.3 mA peak, 10% may be too dim to see in a lit room. Fine at night.
