# Prototype Assembly Drawings

**Asset:** [6080016](./6080016.md) · Build reference for unit #1 (the prototype), alongside [Spec-2's text assembly procedure](../Design/Spec-2.md#assembly-procedure-per-jar) and the existing [lid disc layout](../Design/Spec-2-media/lid-disc-layout.svg). This doc adds the one drawing that was missing — an actual wiring reference — and a prototype-specific build checklist that accounts for what actually got purchased (see the [2026-10-10 meeting](./Meetings/2026-10-10-Prototype-Parts-Order.md)).

## Wiring Diagram

![Prototype wiring diagram](Assembly-media/wiring-diagram.svg)

This is the diagram Spec-2's pin-map *table* and architecture *flowchart* don't give you: what actually gets soldered to what. Reference designators match [Spec-2's Components table](../Design/Spec-2.md#components) (U1, BT1, LDR1, R7, D1-D6).

**One deliberate deviation from the original BOM, reflected in this drawing:** the LEDs actually purchased ([meeting note](./Meetings/2026-10-10-Prototype-Parts-Order.md#6-owners-and-dates), Amazon-sourced) have a resistor **built into the lead assembly**, rated for 3 V operation — unlike [DD-4](../Design/DD-4-LED-Type-and-Count.md)'s selected part (bare pre-wired 0603, no resistor, relying on the external 1 kΩ R1-R6). A pre-wired LED explicitly built and labeled for 3 V operation is, by construction, meant to be driven directly from a 3.3 V logic pin — that's the entire purpose of building the resistor in. **So this diagram wires D2-D7 straight to each LED, no external R1-R6.** Verify this is right before committing to all six:

1. Wire **one** LED (LED 1, on D2) and run [`01_bringup_test.ino`](./Firmware/01_bringup_test.ino).
2. Measure current in series, or estimate from the voltage drop across the LED. Should land well under the GPIO's 4 mA standard-drive ceiling (see the resistor discussion in the [2026-10-10 meeting](./Meetings/2026-10-10-Prototype-Parts-Order.md#resistor-value-review-r1-r6-r7) — the built-in resistor almost certainly already limits current to a sane value for a 3 V-rated part).
3. If it reads unexpectedly bright, hot, or you can't get a current reading that makes sense, **stop and add an external 1 kΩ** (the dashed symbol in the diagram) before wiring the rest. R1-R6 stay in the [BOM](./BOM.md) as a fallback either way.

The **battery** is wired straight to BAT+/BAT− for the prototype (on-hand hookup wire substitutes for the BOM's J1 JST-PH pigtail) — simplest for one unit. J1 remains worthwhile for the batch build, where tool-free cell replacement years from now matters more than one extra joint today (ties to [meeting action item 4](./Meetings/2026-10-10-Prototype-Parts-Order.md#action-items)).

## Lid Disc Layout

Unchanged from Design — see [Spec-2-media/lid-disc-layout.svg](../Design/Spec-2-media/lid-disc-layout.svg) for hole positions (USB-C slot, LDR window, 6 LED anchor-hole pairs, 6 air holes) and the anchor ring dimensions. **Measure the actual jar opening and band inner diameter with calipers before cutting** — this drawing is explicitly "not to manufacturing tolerance" per its own footer, and [DD-10](../Design/DD-10-Enclosure.md)/[Spec-2's Open Items](../Design/Spec-2.md#open-items) still flag the PET jar's band fit as unverified.

## Prototype Build Checklist

Spec-2's 7-step procedure is written for the batch (12 units, ≤60 min each). For the prototype, work through it in this order, with the verification gates a one-off build actually needs:

| # | Step | Note for the prototype |
|---|---|---|
| 1 | Cut the lid disc per the [layout](../Design/Spec-2-media/lid-disc-layout.svg) | Use whichever jar (PET or glass) is on hand for a test fit — the final age-split decision ([meeting item 1](./Meetings/2026-10-10-Prototype-Parts-Order.md#1-cousin-age-split-pet-vs-glass)) doesn't block this |
| 2 | Flash [`01_bringup_test.ino`](./Firmware/01_bringup_test.ino) over USB **before any soldering** | Confirms the board, the Arduino IDE board-support setup, and USB-C flashing all work first — cheapest point to catch a dead board |
| 3 | Solder LED 1 only (D2) per the wiring diagram above | Do the current check above before going further |
| 4 | Solder the LDR + R7 divider to D0/D1/GND | Re-run `01_bringup_test.ino`; confirm the Serial Monitor shows a sensible analog reading that changes when you cover the LDR |
| 5 | Solder LEDs 2-6 and the battery leads | Thread each lead through its anchor pair at the hang length called out on the wiring diagram (6/9/12/15/18/21 cm) before soldering, so you're not re-threading a finished joint |
| 6 | Mount the XIAO on edge with USB-C through the slot; glue the LiPo flat; connect the battery | |
| 7 | Flash [`02_courtship_demo.ino`](./Firmware/02_courtship_demo.ino) and function-check | See [Test Code](#test-code) below |
| 8 | Fit the liner and moss, insert the lid assembly, tighten the band | |

## Test Code

Two sketches in [Develop/Firmware](./Firmware/), both for the XIAO nRF52840 Sense (Arduino IDE, Seeed's nRF52 board support — see [Spec-2 Implementation](../Design/Spec-2.md#technologies)):

| Sketch | Purpose | Status |
|---|---|---|
| [`01_bringup_test.ino`](./Firmware/01_bringup_test.ino) | Hardware verification: steps each LED individually, prints raw LDR readings, prints battery voltage over Serial. Run this first, before trusting any soldered joint. | Ready to flash |
| [`02_courtship_demo.ino`](./Firmware/02_courtship_demo.ino) | Implements the Meadow background cadence plus the courtship micro-event from [DevNote 6080015](./DevNote-Courtship-Sequence.md) (now the project's reconciled model — see the [DD-9 addendum](../Design/DD-9-Glow-Pattern.md)). | Ready to flash — see the file's own header comment for what's deliberately **not** included yet |

Neither sketch implements IMU wake/sleep power management, the night-light/toy state machine (FR-2 through FR-4), mode-cycling via double-tap, or the low-battery cue — those need the Seeed IMU library integrated and tuned against real sleep-current measurements (TC-FJ-01), which is Destroy-phase work, not something to guess at from here. `02_courtship_demo.ino` runs Meadow continuously over USB power so you can watch the courtship sequence and tune `PULSE_RISE_MS` / `J_HANDOFF_MS` / `FEMALE_GAIN` against the real LEDs before those constants get locked into the full state machine.
