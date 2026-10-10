# Design Decision: Electronics Platform (MCU / Board)

**Asset:** [6080004](./6080004.md)

## Decision
Which microcontroller platform should drive the firefly jar: a discrete ATtiny-based build (as assumed in [Spec-2](./Spec-2.md) Rev A), or an integrated development module that bundles the MCU with charging, sensors, and USB? This decision also resolves how [MuSCoW](../Define/MuSCoW.md) item 22 (user-reprogrammable, *Could*) is met.

## Criteria
- **Item 3, budget control (Should):** $20/unit or less is an easy yes. Above that, the option must name what the extra spend buys: more joy (item 12), less build time (item 18), or another benefit.
- **Item 12, maximum lasting joy (Should):** does the platform enable richer interaction (tap/tilt/clap, multiple modes) that keeps the jar interesting after Christmas?
- **Item 13, flexible platform (Should):** spare PWM/GPIO outputs for other items in the jar.
- **Item 16, Christmas deadline (Must):** parts in stock domestically now; firmware effort fits the schedule.
- **Item 18, build time of 1 hour or less (Should):** fewer hand-wired parts and solder joints.
- **Item 19, kid can restore power (Should):** built-in USB-C charging versus a separate charger board or battery holder.
- **Item 21, night light AND toy (Must):** supports at least two wake paths (dark + user-initiated).
- **Item 22, reprogrammable (Could):** recipient can reflash over USB without a dedicated programmer.
- **Battery life:** standby current dominates shelf life; see the [DD-3 battery model](./DD-3-Power-Source.md#reference-use-profile-and-battery-model).

## Discussion / Research
Five platforms were evaluated. Costs are at qty 12 and cover only the platform-dependent parts (MCU/board, charger, battery, switch/sensor parts that the board does not already include). Common parts (LEDs, jar, lid, dressing) are the same for every option and are costed in [Spec-2](./Spec-2.md#cost-feasibility).

| Option | Platform-dependent parts | Est. cost / unit |
|---|---|---|
| A. ATtiny85 + 2xAA (Spec-2 Rev A baseline) | ATtiny85V-10PU $1.89, DIP socket ~$0.15, perfboard ~$0.40, 2xAA holder w/ switch & screw cover $1.66, 2x AA ~$0.80, tilt switch ~$0.30, LDR + resistor ~$0.15, ISP programmer ~$9 shared (~$0.75) | **~$6.10** |
| B. ATtiny85 + TP4056 USB-C charger + 500 mAh LiPo | ATtiny85 $1.89, socket ~$0.15, perfboard ~$0.40, TP4056 w/ protection ~$0.60, LiPo 500 mAh $7.16, JST pigtail ~$0.35, tilt switch ~$0.30, LDR ~$0.15, programmer share ~$0.75 | **~$11.75** |
| C. Adafruit QT Py SAMD21 + LiPoly Charger BFF + 500 mAh LiPo | QT Py $7.50, Charger BFF $5.95, LiPo $7.16, tilt switch ~$0.30, LDR ~$0.15 | **~$21.06** |
| D. Seeed XIAO nRF52840 + 500 mAh LiPo + tilt switch | XIAO nRF52840 $10.38, LiPo $7.16, JST pigtail ~$0.35, tilt switch ~$0.30, LDR ~$0.15 | **~$18.34** |
| E. Seeed XIAO nRF52840 Sense + 500 mAh LiPo | XIAO Sense $15.90 (includes LSM6DS3TR-C 6-axis IMU + PDM mic + LiPo charger), LiPo $7.16, JST pigtail ~$0.35, LDR ~$0.15 | **~$23.56** |

**What the integrated modules buy.**
- Both XIAO boards include a BQ25101 LiPo charger (50/100 mA selectable), USB-C, and a standby current under 5 µA ([Seeed wiki](https://wiki.seeedstudio.com/XIAO_BLE/)). That removes the charger board, socket, perfboard, and programmer from the build. Measured nRF52840 System ON sleep is around 3.6 µA once peripherals are disabled ([Nordic DevZone](https://devzone.nordicsemi.com/f/nordic-q-a/106874/unexpected-consumption-using-system-off-sample-nrf52840dk/464634)).
- The XIAO Sense adds an LSM6DS3TR-C IMU with hardware wake-up, tap, double-tap, free-fall, and 6D orientation interrupts ([ST](https://www.st.com/content/st_com/en/products/mems-and-sensors/inemo-inertial-modules/lsm6ds3tr-c.html)). Its INT1 is wired to P0.11, and IMU power is switched on P1.08 ([Seeed wiki](https://wiki.seeedstudio.com/XIAO_BLE/)). Double-tap gives a mode button with no button. Tilt enables "fireflies drift to the low side." The PDM mic enables a clap mode.
- XIAO pins: 11 GPIO, all PWM-capable. With 6 LEDs + 1 LDR analog input + 1 LDR power pin (so the divider draws nothing between samples), **3 pins remain spare** for item 13.
- Item 22: the XIAO boards flash over USB-C from Arduino IDE or CircuitPython. ATtiny85 needs an ISP programmer, so a recipient realistically cannot reprogram it.

**What the discrete ATtiny path buys.**
- Lowest cost and the longest battery life on 2xAA (ATtiny85 power-down is ~0.1 µA at 1.8 V per the [datasheet](https://ece-classes.usc.edu/ee459/library/documents/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf), and ~4.5 µA was reported with the watchdog wake running, per [insidegadgets](https://www.insidegadgets.com/?p=1156)).
- Downsides:
  - Only 5 usable I/O pins, so 6 LEDs require charlieplexing and leave nothing spare for item 13.
  - Needs a socket, perfboard, and separate charger.
  - An estimated 50+ hand solder joints per unit versus ~25 for the XIAO builds (see [House of Quality](./House-of-Quality.md)).

**QT Py + BFF** is easy to assemble (stacking headers), but its SAMD21 and on-board regulator idle far higher than the nRF52840. The battery model uses an estimated ~150 µA standby, which gives roughly 2 months per charge. It also costs more than the XIAO without adding sensors.

**Build-time estimate (unverified, to be measured on the first prototype in Destroy):**
- ATtiny + perfboard: ~75-90 min per jar.
- XIAO + pre-wired LEDs: ~40-50 min per jar.
- Savings: ~30-40 min per jar, or ~6-8 hours across 12 units.

**Applying the item 3 rule to the selected option (E).** E costs ~$17.46/unit more than the A baseline (~$210 across 12). It buys:
1. Joy (item 12): double-tap modes, tilt effects, an optional clap mode, and the "low battery, sleepy fireflies" cue (see [DD-9](./DD-9-Glow-Pattern.md)).
2. Build time (item 18): an estimated ~30-40 min saved per jar.
3. Kid-restorable power (item 19): USB-C charging, no tools.
4. Reprogramming (item 22) and 3 spare PWM pins (item 13).

That clears the item 3 bar on joy *and* time. Option D is ~$5.22 cheaper but gives up the IMU (consistent tap/tilt sensing, modes) and the mic.

**Availability (item 16):**
- Seeed's own store was out of stock on Oct 9, 2026.
- Mouser search results showed XIAO nRF52840 Sense #713-102010469 at $15.90 (~391 in stock), Sense Plus #713-102010694 at $16.40 (~308), and plain XIAO #713-102010448 at $10.38 (~846).
- Order by end of October.

## Final Decision
Use the **Seeed XIAO nRF52840 Sense** (option E) with a 500 mAh protected LiPo (see [DD-3](./DD-3-Power-Source.md)). MuSCoW item 22 (reprogrammable) is met by the platform's USB-C flashing. Firmware will be written so the shipped animations work out of the box, and a curious older cousin can reflash from Arduino IDE.

## Explanation
- **Item 3:** over $20/unit, but the extra ~$17 buys measurable joy (IMU modes, clap mode) *and* an estimated 30-40 min of build time per jar. That is exactly what the item 3 rule asks for.
- **Items 18, 19, 22, 13:** the XIAO Sense is the only option that hits all four with no added parts.
- **Item 21:** the IMU provides the user-initiated wake path, and the LDR provides the dark wake path ([DD-1](./DD-1-Trigger-Mechanism.md)).
- **Battery:** ~7 months per charge under the reference profile is well beyond what a rechargeable gift needs.
  - **Risk:** forum users measured 100 µA to 1.24 mA of XIAO Sense sleep current with an untuned IMU demo ([Seeed forum](https://forum.seeedstudio.com/t/xiao-ble-sense-lsm6ds3-int1-single-tap-interrupt/264206?page=2)). That would mean ~109 days, or as little as ~14 days, per charge.
  - Firmware must apply the low-power settings listed in [DD-1](./DD-1-Trigger-Mechanism.md).
  - Destroy must confirm ≤20 µA on the first prototype **before ordering the remaining 11 boards**. If that can't be met, fall back to option D.
- **Item 16:** in stock domestically at Mouser. Fallback is the Sense Plus, or option D plus a tilt switch.

## Options Summary
| Forced Rank | Option | Est. Cost / Unit | Cost Implication | Battery Life Implication | Joy (1-5) | Upside | Downside |
|---|---|---|---|---|---|---|---|
| 1 | E. XIAO nRF52840 Sense + 500 mAh LiPo | ~$23.56 (platform) | +$17.46 vs. A; over $20/unit total, justified by joy + build time (item 3) | ~214 days/charge (2.10 mAh/day; IMU adds ~10 µA standby) | 5 | IMU tap/tilt/double-tap modes, mic, USB-C charging, USB reprogramming, 3 spare PWM pins, fewest joints | Highest cost; firmware for IMU/mic is more involved; Seeed direct out of stock (Mouser OK) |
| 2 | D. XIAO nRF52840 + LiPo + tilt switch | ~$18.34 | +$12.24 vs. A; $5.22 cheaper than E | ~242 days/charge (1.86 mAh/day); a ball switch resting closed against a pull-up can drain ~250 µA (~56 days) unless handled in firmware | 4 | Same charging, reprogramming, and pins as E; cheaper | Tilt switch is binary and inconsistent unit-to-unit; no double-tap modes or clap mode |
| 3 | B. ATtiny85 + TP4056 + LiPo | ~$11.75 | +$5.65 vs. A; under $20/unit total | ~275 days/charge (1.64 mAh/day) | 3 | Rechargeable over USB-C at low cost | Charlieplexing needed; separate charger board; no reprogramming by recipient; ~50+ joints |
| 4 | A. ATtiny85 + 2xAA (Spec-2 baseline) | ~$6.10 | Baseline; cheapest | ~3.5 years on 2xAA (1.57 mAh/day; alkaline shelf life dominates) | 3 | Cheapest, longest battery life, simplest firmware | Screwdriver to change batteries (fails item 19 "no tools"); no spare I/O (item 13); no recipient reprogramming (item 22); longest build |
| 5 | C. QT Py SAMD21 + Charger BFF + LiPo | ~$21.06 | +$14.96 vs. A; costs more than D with no sensors | ~69 days/charge (est. ~150 µA standby from regulator + SAMD21) | 3 | Plug-together stack, Adafruit support and stock | Poor standby current; still needs tilt switch; no IMU/mic |

## Sources
- [Seeed Studio XIAO nRF52840 Series wiki](https://wiki.seeedstudio.com/XIAO_BLE/)
- [ST LSM6DS3TR-C product page](https://www.st.com/content/st_com/en/products/mems-and-sensors/inemo-inertial-modules/lsm6ds3tr-c.html)
- [Nordic DevZone: nRF52840 System ON sleep current](https://devzone.nordicsemi.com/f/nordic-q-a/106874/unexpected-consumption-using-system-off-sample-nrf52840dk/464634)
- [DigiKey ATTINY85V-10PU](https://www.digikey.com/en/products/detail/microchip-technology/ATTINY85V-10PU/735471)
- [ATtiny25/45/85 datasheet](https://ece-classes.usc.edu/ee459/library/documents/Atmel-2586-AVR-8-bit-Microcontroller-ATtiny25-ATtiny45-ATtiny85_Datasheet.pdf)
- [insidegadgets ATtiny85 sleep](https://www.insidegadgets.com/?p=1156)
- [Adafruit QT Py SAMD21](https://www.adafruit.com/product/4600)
- [Adafruit LiIon/LiPoly Charger BFF](https://www.adafruit.com/product/5397)
- [Adafruit 500 mAh LiPo #1578](https://www.adafruit.com/product/1578)
- [SparkFun 2xAA holder with cover and switch](https://www.sparkfun.com/battery-holder-2xaa-with-cover-and-switch.html)
- [eBay 10-pack TP4056 USB-C](https://www.ebay.com/itm/127115739222)
