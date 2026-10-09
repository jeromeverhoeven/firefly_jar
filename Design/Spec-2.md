# Design Specification — Firefly Jar

## Project Overview
A battery-operated decorative jar that visually reads as fireflies glowing inside glass. Several warm-toned LEDs, mounted on fine wire "stems" at varying heights inside the jar, fade in and out independently and arrhythmically to mimic real firefly flicker. The unit wakes from a low-power sleep state and runs its glow sequence when it senses being shaken and/or when the ambient room is dark, then returns to sleep to conserve battery.

This spec traces to [Define/MuSCoW.md](../Define/MuSCoW.md) and resolves which **Could** items this first design iteration adopts versus defers.

## Renderings

![Exterior concept](Spec-2-media/rendering-exterior.svg)
*Exterior concept — glass jar, screw lid with small air/sound perforations, fireflies glowing at varying heights, side window for the light sensor.*

![Internal section view](Spec-2-media/rendering-internal-section.svg)
*Internal section — a false-bottom deck hides the driver PCB, 2xAA battery holder/switch, and tilt switch. Fine wire LED stems rise from the deck into the jar volume. The LDR peeks through a small side port in the jar wall/label band.*

## Operating Principle

### Light engine
- A small low-power microcontroller (ATtiny-class, 8-pin DIP) drives 5-6 warm-white/yellow diffused LEDs, each through its own current-limiting resistor.
- Firmware fades each LED independently using PWM with randomized on-time, peak brightness, and pause duration per LED, so the group never blinks in unison — this is what sells the "firefly" read rather than a generic blinking light.
- Outside an active glow cycle, the MCU sits in a low-power sleep mode so the batteries primarily drain only during active cycles, not while idle on a shelf.

### Trigger / activation logic
Per [Define/MuSCoW.md](../Define/MuSCoW.md) items 4-10, four sensing options and three combination strategies were identified. For this iteration:

| MuSCoW item | Decision | Reason |
|---|---|---|
| 5 — Tilt/vibration switch (shake) | **Adopted** | Lowest cost, zero analog circuitry — a closed contact can drive the MCU's wake/interrupt pin directly |
| 6 — Low-light sensor (LDR) | **Adopted** | Thematically correct (fireflies appear at dusk/dark) and one of the cheapest sensor options |
| 9 — OR'd multi-trigger | **Adopted** | Either "dark enough" or "shaken" independently starts a glow cycle — most forgiving/responsive behavior for a gift object, and OR logic needs no extra parts beyond the two sensors already chosen |
| 4 — Accelerometer | Deferred | Higher part cost and firmware complexity than a tilt switch for a binary "was it shaken" signal; revisit only if directional/intensity sensing becomes a feature request |
| 7 — Microphone / clap | Deferred | Needs a mic + analog threshold/amplifier stage, the priciest and most false-trigger-prone of the sensor options; revisit as a v2 feature |
| 10 — AND'd multi-trigger | Deferred | Requiring e.g. dark **and** shaken makes the toy feel less responsive; more appropriate once/if a sensor set is added that benefits from false-trigger suppression |

Behavior: while asleep, a dark reading from the LDR **or** a closed pulse from the tilt switch wakes the MCU, which runs the flicker sequence for a fixed window (tentatively ~30-60 s), then re-checks conditions and either continues (still dark) or returns to sleep.

### Power management
- 2x AA alkaline cells in a holder with an integrated on/off switch (hard kill switch for shipping/storage).
- Sleep-current design target keeps shelf standby life in the months range; active glow cycles are the dominant draw and are self-limiting by the timed window above.

## Cost Feasibility

[Define/MuSCoW.md](../Define/MuSCoW.md) item 3 sets a **Should**-level target of under $20/unit in a batch of 10. This is a feasibility-level, category-based estimate (not a line-item BOM — that belongs to the Develop phase) using representative distributor/retail pricing at small quantity:

| Category | Representative part(s) | Est. cost/unit (qty 10) |
|---|---|---|
| MCU | ATtiny85-class DIP-8 | ~$1.60 |
| LEDs (x5-6) | Diffused warm-white/yellow 5mm | ~$0.80 |
| Shake sensor | Tilt/vibration ball switch | ~$0.40 |
| Light sensor | LDR (GL5528-class) | ~$0.40 |
| Passives | Resistors, misc | ~$0.20 |
| Battery holder | 2xAA w/ switch + leads | ~$1.50 |
| Batteries | 2x AA alkaline | ~$0.80 |
| Driver board | Small proto/custom PCB | ~$1.00 |
| Enclosure | 8 oz glass mason jar + lid | ~$1.30 |
| Misc. | Wire stems, solder, hot glue, hardware | ~$0.75 |
| **Estimated parts total** | | **~$8.75/unit** |

This is roughly **55% below** the $20 Should-target, leaving headroom to:
- absorb real-world price variance and shipping/tax on small-quantity orders,
- add a deferred sensor (accelerometer and/or mic) in a later revision while staying under budget, and
- cover simple packaging without threatening the target.

**Conclusion: the $20/unit @ qty-10 target is comfortably feasible** with the adopted sensor set (tilt switch + LDR, OR'd). Labor/assembly time is not costed here since the qty-10 pilot batch is assumed to be hand-built by the project owner; a paid-assembly cost should be added before scaling beyond the pilot batch.

## Open Design Decisions
- Final LED count/placement for the most convincing "swarm" density without cluttering the jar.
- Exact glow-cycle duration and whether a second shake while active should extend it.
- Whether to add a mechanical lid vent purely for condensation/battery heat, independent of the sound-sensor cutouts shown in the rendering (sound sensing itself is deferred per item 7 above).
