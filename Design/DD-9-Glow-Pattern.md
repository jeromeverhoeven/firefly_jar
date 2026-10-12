# Design Decision: Glow Pattern and Modes

**Asset:** [6080011](./6080011.md)

## Decision
What should the fireflies actually *do*: flash rhythm, fades, interplay between LEDs, and selectable modes? [MuSCoW](../Define/MuSCoW.md) item 1 leaves the glow pattern to a design decision, and item 12 now asks for joy that *lasts*.

## Criteria
- **Item 1, firefly look (Must):** fades, not blinks; never all in unison by accident.
- **Item 12, maximum lasting joy (Should):** variety and surprises that keep the jar interesting after Christmas morning.
- **Item 14, audience 7-16 (Must):** playful for younger kids, not babyish for teens.
- **Item 16, deadline (Must):** firmware scope must be cuttable. Ship a minimum, then add.
- **Item 22, reprogrammable (Could):** patterns written so an older cousin can tweak them.
- **Battery:** average LED duty while active ([DD-3 model](./DD-3-Power-Source.md#reference-use-profile-and-battery-model) assumes ~1/6 duty per LED).

## Discussion / Research
**Real firefly behavior to borrow.**
- Common eastern firefly (*Photinus pyralis*) males flash **every ~5-7 s**, faster when warm, while flying a rising "J" path.
- Females **answer after a ~1-2 s delay** ([Wikipedia](https://en.wikipedia.org/wiki/Photinus_pyralis)).
- That gives two cheap, authentic effects: a **~6 s cadence with randomized offsets** per LED, and **call-and-response**, where one LED flashes and another answers ~1-2 s later.
- **Synchronous fireflies** (e.g., *Photinus carolinus* in the Smokies) gradually fall into unison. A "sync" mode that drifts from random to synchronized over a minute is a striking, showable trick for older kids.
- These timing figures can be tuned and visually checked against documented ranges in [6080014, the Flash Pattern Simulator](../Develop/6080014.md) before being committed to firmware constants.

**Addendum (2026-10-12): the courtship micro-event, reconciled.** [6080015, the Courtship Sequence dev note](../Develop/6080015.md) worked out how Meadow's "occasional call-and-response" (above) actually plays out with 6 fixed LEDs, and flagged a conflict with the simulator's model that needed a decision before firmware could be written. **Resolved in favor of the dev note's model**, because it's concrete, has working pseudocode, and — critically — fakes the male's J-flight as motion across physical points (a low LED handing off to the one above it) rather than a single point breathing in place, which reads more like the real "big dipper" flight the species is named for:
- A **courtship** is an occasional event inside Meadow (every 60-120 s, [`COURTSHIP_EVERY_S`](../Develop/DevNote-Courtship-Sequence.md)), not every flash — the rest of the time, all 6 LEDs just run independent Meadow cadences as before.
- The **male's J** = a ~0.5 s pulse (fast rise, slower fall) that starts dim on a lower LED and peaks on the LED one slot above it (`J_HANDOFF_MS` later) — the "hand-off" is what sells upward motion with no moving parts.
- The **female** is always LED 6 (lowest, nearest the moss), answering at ~10% brightness, `ANSWER_DELAY_MS` after the male's **peak** (not his onset — a change from this doc's original framing).
- Each successive J in one courtship starts one LED lower (the male "approaching"), ending in a smaller single-LED "landing" flash near her.
- Full detail, field citations, exact timeline, and firmware pseudocode are in [6080015](../Develop/DevNote-Courtship-Sequence.md) — this addendum just records the decision and points there rather than duplicating it.

This supersedes `FLASH_PERIOD_S` (6±1 → 5.9±0.6 s) and `FADE_MS` (→ split into `PULSE_RISE_MS`/`PULSE_FALL_MS`) in [Spec-2's Data Model](./Spec-2.md#data-model-firmware-configuration), which now carries the full constant list. The [simulator (6080014)](../Develop/6080014.md) still models the earlier single-point version — worth a future revision to match, but not a blocker for prototype firmware.

**Flash shape.** A ~0.3-0.8 s fade-in/fade-out, using a gamma-corrected PWM ramp, with a brief peak. Peak brightness randomized ±30% per flash so it never looks mechanical.

**Modes via double-tap ([DD-1](./DD-1-Trigger-Mechanism.md)).** Cycle order:
1. **Meadow** (default): independent ~6 s cadence, random offsets, occasional call-and-response.
2. **Sync:** starts random and converges to unison over ~60 s, then drifts apart.
3. **Clap** (stretch goal): mic on, and a clap or "boo!" scatters the fireflies into a burst. Auto-exits after 2 min.

Each mode change is confirmed by a short "roll call" ripple across the 6 LEDs.

**System cues (always on):**
- **Settling down:** the last 5 min of the night show ([DD-5](./DD-5-Glow-Cycle.md)) gets slower and dimmer, with fewer fireflies.
- **Sleepy fireflies (low battery):** below ~3.5 V (read via P0.31 per the [Seeed wiki](https://wiki.seeedstudio.com/XIAO_BLE/)), shows open with a few slow, dim "yawns." That tells the kid to charge it, in character.
- **Tilt drift:** when the jar is tipped, flashes favor the LEDs on the low side (using IMU orientation).

**Firmware scope by deadline risk.**
- **Must-ship:** Meadow, settling down, sleepy-fireflies cue.
- **Should-ship:** Sync, tilt drift.
- **Stretch:** Clap.
- All parameters are constants at the top of the sketch for item 22.

**Battery.**
- Meadow and Sync average ~1/6 duty per LED, matching the model at ~214 days/charge.
- Settling down lowers duty further.
- Clap mode adds ~3 mA while listening, but is capped at 2 min per use, ~0.1 mAh.

## Final Decision
**Multi-mode firmware cycled by double-tap: Meadow (default) → Sync → Clap (stretch).** All modes use firefly-like fades on a ~6 s randomized cadence with call-and-response. Always-on cues: settling-down ending, "sleepy fireflies" low-battery warning, and tilt drift. Scope is tiered so the Must-ship set alone is a complete gift.

## Explanation
- **Item 1:** smooth randomized fades on a real firefly cadence.
- **Item 12:** hidden modes, tilt drift, and in-character cues give kids things to discover in February. Sync is a "show your friends" trick.
- **Item 14:** Meadow and Clap delight younger kids; Sync and reprogrammable parameters appeal to teens.
- **Item 16:** tiered scope keeps Christmas safe.
- **Item 22:** patterns are plain, commented constants and functions.
- **Battery:** ~214 days/charge as modeled.

## Options Summary
| Forced Rank | Option | Est. Cost / Unit | Cost Implication | Battery Life Implication | Joy (1-5) | Upside | Downside |
|---|---|---|---|---|---|---|---|
| 1 | Multi-mode (Meadow → Sync → Clap) + settle, low-battery, tilt cues | $0 (firmware) | None; firmware time only | ~214 days/charge; clap use ~0.1 mAh each | 5 | Authentic and surprising; lasting discovery; in-character battery warning | Most firmware effort; needs tiered scope to protect the deadline |
| 2 | Meadow only (randomized fades + call-and-response) + low-battery cue | $0 | None | ~214 days/charge | 4 | Authentic; least firmware | Fewer things to discover; weaker lasting joy |
| 3 | Sync-only display | $0 | None | ~214 days/charge | 3 | Striking | Monotonous; synchronous unison reads less like a typical backyard |
| 4 | Simple random on/off blinking | $0 | None | Slightly better (shorter on-time) | 2 | Trivial firmware | Looks electronic, not alive (weakens item 1) |
| 5 | Ship blank/user-programmed only (for teens) | $0 | None | Unknown | 2 | Maximum hackability (item 22) | Not a gift out of the box; fails younger audience (item 14) |

## Sources
- [Wikipedia: Photinus pyralis (flash interval, female response delay, J-flight)](https://en.wikipedia.org/wiki/Photinus_pyralis)
- [Seeed Studio XIAO nRF52840 Series wiki (battery voltage pins)](https://wiki.seeedstudio.com/XIAO_BLE/)
- [National Geographic: synchronous fireflies, Great Smoky Mountains](https://www.nationalgeographic.com/travel/article/synchronous-fireflies-light-up-smoky-mountains-national-park)
- [CU Boulder Today: synchronous fireflies](https://www.colorado.edu/today/synchronous-fireflies)
