# Design Decision: Power Source

**Asset:** [6080005](./6080005.md)

## Decision
What battery should power the jar, and how does the recipient restore power? This decision has to satisfy [MuSCoW](../Define/MuSCoW.md) item 2 (battery operated, *Must*), item 15 (safe for the audience, *Must*), and item 19 (kid can restore power without tools or adult help, *Should*). It also has to work with the platform chosen in [DD-2](./DD-2-Platform.md).

## Criteria
- **Item 15, safety (Must):** no swallowable cells (no coin cells), no exposed leads, and a protected chemistry with gentle charging if rechargeable.
- **Item 19, kid can restore power (Should):** no screwdriver, and no opening the jar.
- **Item 2, battery operated (Must)**, plus battery life under the reference profile below.
- **Item 3, budget (Should).**
- **Fit:** must fit inside the lid "puck" with the XIAO (see [DD-7](./DD-7-LED-Stem-Mounting.md)). A regular-mouth jar opening is roughly 60 mm across.
- **Item 16, schedule (Must):** domestic stock.

## Discussion / Research

### Reference use profile and battery model
Every design decision uses this profile for its "Battery Life Implication" column. These are **engineering estimates to be verified in Destroy** with a µA-capable meter.

| Parameter | Value | Basis |
|---|---|---|
| Night-light run | 30 min after the room goes dark, once per night | [DD-5](./DD-5-Glow-Cycle.md) decision |
| Toy wakes | 5 per day x 60 s | Assumed typical play |
| Active time per day | 35 min (0.583 h) | Sum of the above |
| LED average current while active | 0.33 mA per LED (≈2 mA peak, ≈1/6 duty incl. fades) x 6 LEDs = 1.98 mA | [DD-4](./DD-4-LED-Type-and-Count.md), [DD-9](./DD-9-Glow-Pattern.md) |
| MCU active current | nRF52840 ~1.0 mA (PWM running, CPU mostly sleeping); ATtiny85 ~0.5 mA | Estimate; ATtiny 300 µA @ 1 MHz/1.8 V per datasheet |
| Standby current | XIAO ~5 µA + IMU wake-up ~10 µA = 15 µA (Sense); XIAO + tilt switch 5 µA; ATtiny ~5 µA | [Seeed](https://wiki.seeedstudio.com/XIAO_BLE/) (<5 µA), [insidegadgets](https://www.insidegadgets.com/?p=1156) (~4.5 µA); IMU figure is an estimate |
| Usable capacity | 500 mAh LiPo → 450 mAh; 1000 mAh LiPo → 900 mAh; 2xAA alkaline → ~2,000 mAh (derated from ~2,500-2,850 mAh); 3xAAA → ~1,000 mAh; CR2032 → ~200 mAh | Derated nameplate |

**Daily consumption, selected design (XIAO Sense, 6 LEDs):** (1.0 + 1.98 mA) x 0.583 h + 0.015 mA x 24 h = 1.74 + 0.36 = **2.10 mAh/day**, which gives **~214 days per charge on a 500 mAh LiPo.**

**Sensitivity to standby current (500 mAh, same active profile):**

| Standby current | mAh/day | Days per charge | Note |
|---|---|---|---|
| 15 µA | 2.10 | ~214 | Model value (XIAO <5 µA + IMU ~9 µA low-power) |
| 23 µA | 2.29 | ~196 | Adding an AM312 PIR (~8 µA) |
| 100 µA | 4.14 | ~109 | Low end of forum-measured untuned XIAO Sense sleep |
| 1,240 µA | 31.50 | ~14 | High end of forum-measured untuned sleep ([Seeed forum](https://forum.seeedstudio.com/t/xiao-ble-sense-lsm6ds3-int1-single-tap-interrupt/264206?page=2)) |

Standby current matters more than LED count. Verifying it is the first Destroy test.

### Options
- **500 mAh protected LiPo (Adafruit #1578):** 29 x 36 x 4.75 mm, 10.5 g, pre-attached JST-PH. Protection covers overcharge, over-discharge (3.0 V cutoff), and output short. $7.16 each at 10+ ([Adafruit](https://www.adafruit.com/product/1578)). It charges on the XIAO's BQ25101 at 100 mA (≈0.2C, gentle), so a full charge takes roughly 5-6 h from any USB phone charger.
- **1000 mAh LiPo:** doubles runtime but doubles charge time (~10 h at 100 mA). A typical 1000 mAh pouch is ~50 x 34 x 6 mm, which is tight next to the XIAO inside a ~60 mm lid opening. Cost estimated at ~$9-10.
- **2xAA in a screw-cover holder with switch:** $1.66 each at 25+, screw-secured sliding cover ([SparkFun](https://www.sparkfun.com/battery-holder-2xaa-with-cover-and-switch.html)). Alkaline AA shelf life is ~10 years ([Energizer E91](https://cdn.connectec.uk/Files/Products/Batteries/energizer_batteries/e91_aa_battery_datasheet.pdf)).
  - The screw cover is the child-safe choice, but it needs a screwdriver (fails item 19).
  - The holder is too large for the lid, which forces bottom mounting.
  - The XIAO can technically run from 2xAA on its 3V3 pin, but that gives up charging entirely.
- **3xAAA:** 4.5 V suits an ATtiny, but it exceeds the XIAO's 3V3 pin rating. Same tool and size issues as AA.
- **CR2032 coin cell:** smallest and cheapest, but coin cells are a serious swallowing hazard for young children. Fails item 15 (Must).

### LiPo safety measures (item 15)
- Protected cell only.
- Charge at 100 mA max.
- Cell mounted inside the lid puck, glued to the disc and covered, with no exposed leads.
- A JST connector so the cell can be replaced.
- A short note for parents: "charge with any phone charger; don't charge if the jar is cracked or the lid is damaged."

## Final Decision
**500 mAh protected LiPo (Adafruit #1578), charged through the XIAO Sense's built-in USB-C charger**, with the USB-C port exposed on top of the lid ([DD-7](./DD-7-LED-Stem-Mounting.md)). The firmware reads battery voltage (P0.31 via P0.14) to drive a "sleepy fireflies" low-battery cue ([DD-9](./DD-9-Glow-Pattern.md)).

## Explanation
- **Item 19:** a kid plugs in a phone charger. No tools, no opening the jar.
- **Item 15:** protected cell, gentle charge rate, enclosed in the lid, no coin cell.
- **Battery life:** ~7 months per charge under the reference profile. Charging becomes a rare event and doubles as a small ritual.
- **Fit and charge time** favor 500 mAh over 1000 mAh. 1000 mAh is the drop-in upgrade if a prototype shows heavier use.
- **Item 3:** $7.16 + ~$0.35 pigtail per unit, already included in the [DD-2](./DD-2-Platform.md) platform cost.

## Options Summary
| Forced Rank | Option | Est. Cost / Unit | Cost Implication | Battery Life Implication | Joy (1-5) | Upside | Downside |
|---|---|---|---|---|---|---|---|
| 1 | 500 mAh protected LiPo via XIAO USB-C charger | ~$7.51 (cell + JST pigtail) | Included in DD-2 platform cost; +$5.05 vs. 2xAA + holder | ~214 days/charge; ~5-6 h to recharge | 4 | Kid recharges with any phone charger; no tools; fits in lid; protected | Lithium cell in a kid's room needs the safety measures above; charge circuit is the XIAO's |
| 2 | 1000 mAh protected LiPo via XIAO | ~$9-10 (est.) | +~$2 vs. rank 1 | ~429 days/charge; ~10 h to recharge | 4 | Over a year between charges | Tight fit beside XIAO in a ~60 mm lid; overnight-plus charging |
| 3 | 2xAA alkaline, screw-cover holder with switch | ~$2.46 (holder $1.66 + cells ~$0.80) | Cheapest rechargeable alternative; −$5.05 vs. rank 1 | ~953 days on XIAO 3V3 pin; ~3.5 years on ATtiny (shelf life dominates) | 3 | Very long life; cells sold everywhere | Screwdriver needed (fails item 19); too big for the lid, forcing bottom mount; no charging |
| 4 | 3xAAA alkaline holder | ~$2.50 (est.) | Similar to rank 3 | ~638 days on ATtiny; not compatible with XIAO 3V3 pin | 3 | Slimmer than AA | Over-voltage for XIAO; same tool and size issues |
| 5 | CR2032 coin cell | ~$0.50 (est.) | Cheapest | ~128 days on ATtiny; poor pulse-current behavior | 2 | Tiny | Swallowing hazard; fails item 15 (Must), rejected |

## Sources
- [Adafruit Lithium Ion Polymer Battery 3.7 V 500 mAh #1578](https://www.adafruit.com/product/1578)
- [Seeed Studio XIAO nRF52840 Series wiki](https://wiki.seeedstudio.com/XIAO_BLE/)
- [SparkFun Battery Holder 2xAA with Cover and Switch](https://www.sparkfun.com/battery-holder-2xaa-with-cover-and-switch.html)
- [Energizer E91 AA datasheet](https://cdn.connectec.uk/Files/Products/Batteries/energizer_batteries/e91_aa_battery_datasheet.pdf)
- [insidegadgets ATtiny85 sleep](https://www.insidegadgets.com/?p=1156)
- [Nordic DevZone: nRF52840 sleep current](https://devzone.nordicsemi.com/f/nordic-q-a/106874/unexpected-consumption-using-system-off-sample-nrf52840dk/464634)
