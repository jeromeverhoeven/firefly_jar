# Design Decision: Triggering Mechanism(s)

**Asset:** [6080003](./6080003.md)

**Revision B.** Re-evaluated against the expanded [MuSCoW](../Define/MuSCoW.md) (items 14-24, especially item 21, night light AND toy) and the platform chosen in [DD-2](./DD-2-Platform.md). Revision A chose tilt/vibration switch + LDR, OR'd, on an ATtiny. As Rev A's "living table" note predicted, a module that bundles an accelerometer with the MCU changes the cost side, and the XIAO Sense does exactly that.

## Decision
Which sensor(s) should trigger the firefly glow effect, and how should multiple sensors combine (single sensor, OR'd, or AND'd), per [MuSCoW](../Define/MuSCoW.md) items 4-11 and 21?

## Criteria
- **Item 21, night light AND toy (Must):** at least two wake paths. One is an ambient "it got dark" path, and the other is an on-demand "wake it up to show people" path.
- **Items 15 and 17, safety and durability (Must):** the trigger gesture should not encourage handling the enclosure can't survive.
- **Item 3, budget (Should):** incremental $/unit on top of the [DD-2](./DD-2-Platform.md) platform.
- **Battery life (item 2 Must, item 19 Should):** every always-armed sensor draws current 24 h/day. See the [DD-3 battery model](./DD-3-Power-Source.md#reference-use-profile-and-battery-model).
- **Item 12, joy including lasting joy (Should):** delightful on day 1 *and* in February.
- **False-trigger cost:** an occasional extra wake is fine (item 23 is a *Won't*, since "slightly annoying is fun"), but a jar that never sleeps drains the battery.
- **Unit-to-unit consistency (item 18 Should):** 12 hand-built units should behave the same.
- **Item 16, deadline (Must):** firmware effort must fit before Christmas.

## Discussion / Research

**Standby current is the criterion Revision A missed.** Anything armed while the jar sleeps costs battery every hour of every day.

| Sensor | Armed (standby) current | Source / note |
|---|---|---|
| Spring vibration switch (SW-18010P) | ~0 µA | Normally-open spring contact; closes only while vibrating |
| Ball tilt switch (SW-520D style) | 0 µA, *or* continuous pull-up current | Can rest **closed** depending on how the jar sits. Against a ~13 kΩ internal pull-up at 3.3 V that is ~250 µA, or ~56 days/charge, unless firmware disables the pull-up after waking |
| LDR (GL5528) + 100 kΩ divider | <1 µA average | Powered from a GPIO only while sampling (~every 10 s); must be sampled with **LEDs off**, or the jar's own glow reads as daylight |
| LSM6DS3TR-C accelerometer (on XIAO Sense) | ~9 µA low-power @ 12.5 Hz; ~160 µA high-performance | Datasheet figures quoted in the [Seeed forum](https://forum.seeedstudio.com/t/xiao-ble-sense-lsm6ds3-int1-single-tap-interrupt/264206?page=2); wake-up, tap, double-tap, 6D interrupts ([ST](https://www.st.com/content/st_com/en/products/mems-and-sensors/inemo-inertial-modules/lsm6ds3tr-c.html)); INT1 → P0.11 ([Seeed wiki](https://wiki.seeedstudio.com/XIAO_BLE/)) |
| PDM microphone (on XIAO Sense) | ~3 mA total while listening (mic + nRF52840 awake processing audio; estimate) | Always-armed would cut life to ~6 days/charge, so it can't be a 24 h wake source |
| PIR (AM312) | ~8 µA at 5 V; 3-12 V supply; ~4 s hold; 100° cone, ~3 m | [Phipps Electronics](https://www.phippselectronics.com/product/mini-pir-motion-sensor-module-am312/), [atomic14](https://www.atomic14.com/esp32/sensors/am312/). The lens must see out through the jar or lid |

**Measured-risk flag.** Forum users measured **100 µA to 1.24 mA** of XIAO Sense sleep current with the IMU double-tap demo, before low-power tuning ([Seeed forum](https://forum.seeedstudio.com/t/xiao-ble-sense-lsm6ds3-int1-single-tap-interrupt/264206?page=2)). At 100 µA the reference profile gives ~109 days/charge. At 1.24 mA it gives ~14 days.
- The firmware must disable the UART/USB serial in sleep, put the QSPI flash in deep power-down, turn the gyro off, and run the accelerometer in its 12.5 Hz low-power mode.
- **Destroy must measure sleep current on the first prototype before the batch build.** Target: ≤20 µA.

**Platform changes the cost side.** [DD-2](./DD-2-Platform.md) selects the XIAO nRF52840 Sense, which already carries the accelerometer and mic, so their incremental cost is **$0**. Compared to a spring switch, the accelerometer also gives:
- Sensitivity set in firmware, so all 12 units behave the same.
- Single and double-tap gestures, so modes can change without a button.
- Tilt direction, so fireflies can "drift" toward the low side.

**Combination logic.** Item 21 settles it in favor of OR'd.
| Option | Behavior | Notes |
|---|---|---|
| Single trigger (item 9) | One sensor only | Fails item 21: either no automatic night light or no on-demand wake |
| OR'd (item 10) | Any armed sensor fires → glow starts | Satisfies item 21; no extra hardware with an MCU |
| AND'd (item 11) | All selected sensors must agree | "Dark AND shaken" blocks waking it to show people in a lit room; fails item 21 |

**Mic as an opt-in effect, not a wake source.** Because the mic is too hungry to keep armed, it becomes a **clap mode**: double-tap into it, a clap or "boo!" makes the fireflies scatter, and it auto-exits after 2 min. Same hardware, pure firmware. It's a stretch goal that doesn't threaten the deadline.

**Safety interplay.** Shake-to-wake trains kids to shake the jar. The accelerometer can wake on a **gentle tap or tilt** with a tunable threshold, so the firmware rewards a light touch rather than vigorous shaking. That directly reduces the risk handled in [DD-10](./DD-10-Enclosure.md).

## Final Decision
**LDR (dusk) OR accelerometer (tap/tilt/shake), OR'd.** Uses the XIAO Sense's on-board LSM6DS3TR-C in low-power wake-up mode and a GL5528 LDR powered from a GPIO and sampled with the LEDs off, with hysteresis. The LDR re-arms only after the room has been lit again ([DD-5](./DD-5-Glow-Cycle.md)).
- The **microphone** is an opt-in, time-limited clap mode (stretch goal), never an always-armed wake source.
- **PIR**, **AND'd logic**, and **single-sensor** designs are rejected for this build.
- If [DD-2](./DD-2-Platform.md) falls back to the plain XIAO or an ATtiny, use rank 3 (LDR OR spring vibration switch). Avoid ball tilt switches.

## Explanation
- **Item 21 (Must):** two independent wake paths. Dusk drives the night light, and tap/shake drives the toy.
- **Item 3:** ~$0.15/unit incremental (LDR). The IMU and mic come with the DD-2 module.
- **Battery:** ~9 µA armed for the IMU gives ~214 days/charge under the reference model, *provided* the sleep-current risk above is closed out in Destroy.
- **Item 12, joy and lasting joy:** tap gestures, tilt drift, and an optional clap mode give variety that outlasts a single trigger.
- **Item 18, consistency:** firmware thresholds behave the same on all 12 units, with no orientation-dependent ball switches.
- **Item 15, safety:** gentle-tap wake discourages hard shaking.

## Options Summary
| Forced Rank | Option | Est. Cost / Unit | Cost Implication | Battery Life Implication | Joy (1-5) | Upside | Downside |
|---|---|---|---|---|---|---|---|
| 1 | LDR OR accelerometer (on-module IMU) | ~$0.15 (LDR + resistor; IMU in DD-2 module) | +$0.15; IMU covered by the $5.22 Sense premium justified in DD-2 | ~214 days/charge (15 µA standby); **risk:** 109 days at 100 µA if sleep isn't tuned | 4 | Two wake paths; tunable and consistent; tap/double-tap modes; tilt effects; rewards gentle handling | IMU register setup in firmware; sleep current must be verified |
| 2 | Rank 1 plus opt-in mic clap mode | ~$0.15 (mic in module) | +$0 over rank 1 | Same as rank 1; each 2-min clap session costs ~0.1 mAh | 5 | Highest wow factor: clap and they scatter | Extra firmware (PDM + threshold) is a deadline risk; noise false-triggers while in mode |
| 3 | LDR OR spring vibration switch (Rev A pairing) | ~$0.45 | +$0.30 vs. rank 1; works on any MCU | ~242 days/charge on plain XIAO (0 µA armed) | 3 | Cheapest standby; simple | Binary only; no gestures/modes; extra part + 2 joints per jar |
| 4 | LDR OR ball tilt switch | ~$0.45 | +$0.30 vs. rank 1 | ~242 days if it rests open; ~56 days if it rests closed against a pull-up | 3 | Very cheap | Orientation-dependent drain and behavior; inconsistent across units |
| 5 | LDR OR PIR (AM312) | ~$1.65-2.15 (module ~$1.50-2 est. + LDR) | +$1.50-2.00 plus a lens hole | ~196 days/charge (+8 µA armed) plus false-wake active time | 4 | "Greets you as you walk by" | Lens must see out of the jar; pets/drafts false-trigger; 3 V minimum supply |
| 6 | Accelerometer only (single trigger) | $0 | Cheapest | ~214 days, less night-light active time | 3 | Simplest firmware | No automatic night light; fails item 21 (Must) |
| 7 | LDR only (single trigger) | ~$0.15 | +$0.15 | Longest (no IMU armed) | 3 | Pure "comes alive at dusk" | Can't show people on demand; fails item 21 (Must) |
| 8 | LDR AND shake (AND'd) | ~$0.15 | +$0.15 | Lowest active time | 2 | Fewest false wakes | Won't wake in a lit room to show people; fails item 21 (Must) |

## Sources
- [ST LSM6DS3TR-C product page](https://www.st.com/content/st_com/en/products/mems-and-sensors/inemo-inertial-modules/lsm6ds3tr-c.html)
- [Seeed forum: XIAO BLE Sense LSM6DS3 INT1 tap interrupt (current figures, measured sleep)](https://forum.seeedstudio.com/t/xiao-ble-sense-lsm6ds3-int1-single-tap-interrupt/264206?page=2)
- [Seeed Studio XIAO nRF52840 Series wiki](https://wiki.seeedstudio.com/XIAO_BLE/)
- [Phipps Electronics: AM312 mini PIR](https://www.phippselectronics.com/product/mini-pir-motion-sensor-module-am312/)
- [atomic14: AM312 mini PIR](https://www.atomic14.com/esp32/sensors/am312/)
