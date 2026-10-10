# House of Quality — Firefly Jar

This matrix traces every [MuSCoW](../Define/MuSCoW.md) item (the **WHATs**) to measurable engineering characteristics (the **HOWs**). It scores how strongly each design lever drives each requirement and shows which levers matter most. It reflects the design as resolved in DD-1 through DD-10 (**Rev B**), compared against the original [Spec-2](./Spec-2.md) Rev A concept (ATtiny85 + 2xAA + 5 mm LEDs + tilt switch/LDR + glass jar with bottom deck).

## How to read it
- **Weight** (from MuSCoW): **Must = 5**, *Should* = 3, *Could* = 1, Won't = 0. Won't items are shown for completeness and contribute nothing.
- **Relationship strength:** ● strong = 9, ○ medium = 3, ▽ weak = 1, blank = none.
- **Technical importance** = Σ (requirement weight x relationship strength) down each column. Higher means that lever deserves more design and test attention.
- **Rev A / Rev B (1-5):** how well each concept satisfies each requirement (5 = fully). The weighted totals at the bottom compare the two concepts.

## Engineering characteristics (HOWs)
| ID | Characteristic | Direction | Unit | Rev B target | Decided in |
|---|---|---|---|---|---|
| H1 | Unit BOM cost | ↓ | $/unit | ≤ $20 easy yes; Rev B ~$30, justified per item 3 | [DD-2](./DD-2-Platform.md), [Spec-2](./Spec-2.md#cost-feasibility) |
| H2 | Hands-on build time | ↓ | min/unit | ≤ 60 (est. 40-50) | [DD-2](./DD-2-Platform.md), [DD-4](./DD-4-LED-Type-and-Count.md), [DD-7](./DD-7-LED-Stem-Mounting.md) |
| H3 | Standby (sleep) current | ↓ | µA | ≤ 20 | [DD-1](./DD-1-Trigger-Mechanism.md), [DD-3](./DD-3-Power-Source.md) |
| H4 | Average active current | ↓ | mA | ≤ 3 | [DD-3](./DD-3-Power-Source.md), [DD-9](./DD-9-Glow-Pattern.md) |
| H5 | Runtime per charge (reference profile) | ↑ | days | ≥ 180 (model ~214) | [DD-3](./DD-3-Power-Source.md), [DD-5](./DD-5-Glow-Cycle.md) |
| H6 | Independent wake paths | ↑ | count | 2 (LDR + IMU) | [DD-1](./DD-1-Trigger-Mechanism.md) |
| H7 | Independently PWM'd LEDs | ↑ | count | 6 | [DD-4](./DD-4-LED-Type-and-Count.md) |
| H8 | Spare PWM/GPIO for accessories | ↑ | count | ≥ 2 (3 available) | [DD-2](./DD-2-Platform.md), [DD-4](./DD-4-LED-Type-and-Count.md) |
| H9 | Animation modes + cues | ↑ | count | ≥ 3 | [DD-9](./DD-9-Glow-Pattern.md) |
| H10 | Enclosure impact resistance | ↑ | drop height survived | 0.75 m onto carpet | [DD-10](./DD-10-Enclosure.md), [DD-7](./DD-7-LED-Stem-Mounting.md) |
| H11 | Hand solder joints | ↓ | joints/unit | ≤ 30 (~24 est.) | [DD-2](./DD-2-Platform.md), [DD-4](./DD-4-LED-Type-and-Count.md) |
| H12 | USB-reprogrammable | ↑ | Y/N | Y | [DD-2](./DD-2-Platform.md) |
| H13 | Tool-free power restore | ↑ | Y/N | Y (USB-C on lid) | [DD-3](./DD-3-Power-Source.md), [DD-7](./DD-7-LED-Stem-Mounting.md) |
| H14 | Unwanted wakes | ↓ | per night | ≤ 1 | [DD-1](./DD-1-Trigger-Mechanism.md), [DD-5](./DD-5-Glow-Cycle.md) |
| H15 | Parts lead time | ↓ | days | ≤ 21; order by Oct 31 | [DD-2](./DD-2-Platform.md), [DD-4](./DD-4-LED-Type-and-Count.md) |

## Relationship matrix (WHATs x HOWs)
| # | Requirement (WHAT) | MuSCoW | Weight | H1 | H2 | H3 | H4 | H5 | H6 | H7 | H8 | H9 | H10 | H11 | H12 | H13 | H14 | H15 | Rev A (1-5) | Rev B (1-5) |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | Firefly-in-a-jar look | **Must** | 5 |  |  |  | ○ |  |  | ● |  | ○ | ○ |  |  |  |  |  | 3 | 5 |
| 2 | Battery operated | **Must** | 5 |  |  | ● | ● | ● |  |  |  |  |  |  |  | ○ |  |  | 5 | 5 |
| 3 | Budget control ($20 easy yes) | *Should* | 3 | ● | ○ |  |  |  |  |  |  |  |  | ▽ |  |  |  | ▽ | 5 | 3 |
| 4 | Shake to activate (accelerometer) | *Could* | 1 | ○ |  | ○ |  |  | ○ |  |  |  |  |  |  |  | ○ |  | 1 | 5 |
| 5 | Shake to activate (tilt/vibration switch) | *Could* | 1 | ▽ |  |  |  |  | ○ |  |  |  |  | ▽ |  |  | ○ |  | 4 | 2 |
| 6 | Low-light to activate | *Could* | 1 |  |  | ○ |  |  | ○ |  |  |  |  |  |  |  | ○ |  | 3 | 5 |
| 7 | Sound/clap to activate | *Could* | 1 | ▽ |  |  | ● |  | ○ |  |  |  |  |  |  |  | ● |  | 1 | 4 |
| 8 | PIR to activate | *Could* | 1 | ○ |  | ○ |  |  | ○ |  |  |  |  |  |  |  | ● |  | 1 | 1 |
| 9 | Single-trigger activation | *Could* | 1 |  |  |  |  |  | ● |  |  |  |  |  |  |  |  |  | 1 | 1 |
| 10 | OR'd multi-trigger | *Could* | 1 |  |  |  |  |  | ● |  |  |  |  |  |  |  | ○ |  | 4 | 5 |
| 11 | AND'd multi-trigger | *Could* | 1 |  |  |  |  |  | ● |  |  |  |  |  |  |  | ● |  | 1 | 1 |
| 12 | Maximum, lasting joy | *Should* | 3 |  |  |  |  | ○ | ○ | ● |  | ● |  |  | ○ |  | ○ |  | 2 | 5 |
| 13 | Flexible power/animation platform | *Should* | 3 |  |  |  |  |  |  | ○ | ● |  |  |  | ○ |  |  |  | 1 | 4 |
| 14 | Audience: ages 7-16 | **Must** | 5 |  |  |  |  |  |  |  |  | ○ | ○ |  |  | ○ | ▽ |  | 3 | 5 |
| 15 | Safe for the audience | **Must** | 5 |  |  |  |  |  |  |  |  |  | ● | ▽ |  | ○ |  |  | 3 | 4 |
| 16 | Delivered by Christmas | **Must** | 5 |  | ● |  |  |  |  |  |  |  |  | ○ |  |  |  | ● | 4 | 4 |
| 17 | Durable | **Must** | 5 |  |  |  |  |  |  |  |  |  | ● | ● |  |  |  |  | 2 | 4 |
| 18 | Build time <= 1 hour | *Should* | 3 |  | ● |  |  |  |  |  |  |  |  | ● |  |  |  |  | 2 | 4 |
| 19 | Kid can restore power | *Should* | 3 |  |  |  |  | ○ |  |  |  |  |  |  |  | ● |  |  | 2 | 5 |
| 20 | Quantity: 12 units | **Must** | 5 | ● | ● |  |  |  |  |  |  |  |  |  |  |  |  | ○ | 5 | 5 |
| 21 | Night light AND toy | **Must** | 5 |  |  | ○ |  | ● | ● |  |  | ○ |  |  |  |  |  |  | 3 | 5 |
| 22 | User-reprogrammable | *Could* | 1 |  |  |  |  |  |  |  |  |  |  |  | ● |  |  |  | 1 | 5 |
| 23 | Quiet/sleep mode (Won't) | Won't | 0 |  |  |  |  |  |  |  |  | ▽ |  |  |  |  | ○ |  | 3 | 3 |
| 24 | Shipping/mailing (Won't) | Won't | 0 |  |  |  |  |  |  |  |  |  | ▽ |  |  |  |  |  | 3 | 3 |
| | **Direction of improvement** | | | ↓ | ↓ | ↓ | ↓ | ↑ | ↑ | ↑ | ↑ | ↑ | ↑ | ↓ | ↑ | ↑ | ↓ | ↓ | | |
| | **Unit** | | | $/unit | min/unit | µA | mA | days | count | count | count | count | drop height survived | joints/unit | Y/N | Y/N | per night | days | | |
| | **Target (Rev B)** | | | ≤ $20 easy yes; Rev B ~$30, justified in DD-2 | ≤ 60 (est. 40-50) | ≤ 20 | ≤ 3 | ≥ 180 (model ~214) | 2 (LDR + IMU) | 6 | ≥ 2 (3) | ≥ 3 | 0.75 m onto carpet | ≤ 30 (~24) | Y | Y (USB-C) | ≤ 1 | ≤ 21; order by Oct 31 | | |
| | **Technical importance (Σ weight x strength)** | | | 80 | 126 | 69 | 69 | 108 | 96 | 81 | 27 | 72 | 120 | 96 | 27 | 72 | 53 | 63 | **193** | **277** |
| | **Relative importance (%)** | | | 6.9 | 10.9 | 6.0 | 6.0 | 9.3 | 8.3 | 7.0 | 2.3 | 6.2 | 10.4 | 8.3 | 2.3 | 6.2 | 4.6 | 5.4 | 60% | 87% |
| | **Priority rank** | | | 7 | 1 | 10 | 11 | 3 | 4 | 6 | 14 | 8 | 2 | 5 | 15 | 9 | 13 | 12 | | |

## Correlation roof (HOW vs. HOW)
`++` strong synergy, `+` synergy, `−` conflict (improving one tends to worsen the other), `−−` strong conflict. Only non-neutral pairs are listed.

| Pair | Correlation | Why |
|---|---|---|
| H3 standby current ↔ H5 runtime | ++ | Standby dominates days per charge once active time is capped ([DD-3](./DD-3-Power-Source.md)) |
| H4 active current ↔ H5 runtime | ++ | Active time x current is the other half of the daily budget |
| H2 build time ↔ H11 solder joints | ++ | Joints are most of the hands-on time |
| H1 cost ↔ H2 build time | −− | Integrated modules (XIAO Sense, pre-wired LEDs) cost more but remove hand work; this is the item 3 trade |
| H1 cost ↔ H13 tool-free power | − | USB-C LiPo charging costs more than a 2xAA holder |
| H1 cost ↔ H10 impact resistance | − | PET jars ~$3.12 vs. glass ~$1.12 ([DD-10](./DD-10-Enclosure.md)) |
| H1 cost ↔ H15 lead time | − | Domestic distributors are faster but pricier than overseas sellers |
| H3 standby current ↔ H6 wake paths | − | Every armed sensor adds standby draw (IMU ~9 µA, PIR ~8 µA, mic ~3 mA) |
| H6 wake paths ↔ H14 unwanted wakes | − | More wake sources mean more chances of false wakes |
| H7 LEDs ↔ H5 runtime | − | Each LED adds ~0.33 mA active (~16 days per LED) ([DD-4](./DD-4-LED-Type-and-Count.md)) |
| H7 LEDs ↔ H8 spare I/O | − | LEDs and accessories compete for the same 11 XIAO pins |
| H7 LEDs ↔ H11 solder joints | − | More LEDs mean more resistor and lead joints |
| H9 modes ↔ H12 reprogrammable | + | Same USB-programmable platform; modes written as editable constants |
| H9 modes ↔ H6 wake paths | + | IMU double-tap doubles as the mode button |
| H13 tool-free power ↔ H10 impact resistance | + | Lid-mounted USB-C means the jar is never opened, so less handling stress |

## Technical priorities
| Rank | ID | Engineering characteristic | Importance | % | Target |
|---|---|---|---|---|---|
| 1 | H2 | Hands-on build time | 126 | 10.9 | ≤ 60 (est. 40-50) |
| 2 | H10 | Enclosure impact resistance | 120 | 10.4 | 0.75 m onto carpet |
| 3 | H5 | Runtime per charge (ref. profile) | 108 | 9.3 | ≥ 180 (model ~214) |
| 4 | H6 | Independent wake paths | 96 | 8.3 | 2 (LDR + IMU) |
| 5 | H11 | Hand solder joints | 96 | 8.3 | ≤ 30 (~24) |
| 6 | H7 | Independently PWM'd LEDs | 81 | 7.0 | 6 |
| 7 | H1 | Unit BOM cost | 80 | 6.9 | ≤ $20 easy yes; Rev B ~$30, justified in DD-2 |
| 8 | H9 | Animation modes + cues | 72 | 6.2 | ≥ 3 |
| 9 | H13 | Tool-free power restore | 72 | 6.2 | Y (USB-C) |
| 10 | H3 | Standby (sleep) current | 69 | 6.0 | ≤ 20 |
| 11 | H4 | Average active current | 69 | 6.0 | ≤ 3 |
| 12 | H15 | Parts lead time | 63 | 5.4 | ≤ 21; order by Oct 31 |
| 13 | H14 | Unwanted wakes | 53 | 4.6 | ≤ 1 |
| 14 | H8 | Spare PWM/GPIO for accessories | 27 | 2.3 | ≥ 2 (3) |
| 15 | H12 | USB-reprogrammable | 27 | 2.3 | Y |

## Concept comparison (planning matrix)
Weighted satisfaction = Σ (weight x rating) / Σ (weight x 5).

| Concept | Weighted score | Max | Satisfaction |
|---|---|---|---|
| Rev A (Spec-2 original: ATtiny85, 2xAA, 5 mm LEDs, tilt + LDR, glass, bottom deck) | 193 | 320 | 60% |
| Rev B (DD-1 to DD-10: XIAO Sense, 500 mAh LiPo, pre-wired 0603, LDR + IMU, mixed PET/glass, top-hung lid) | 277 | 320 | 87% |

Rev B scores lower than Rev A only on **item 3, budget** (3 vs. 5), at ~$30 vs. ~$11 per unit. It scores lower on **item 5** (tilt switch) because it uses the accelerometer instead. Per the item 3 rule, the ~$19/unit increase (~$229 across 12) is bought back as joy (items 12, 21, 22), build time (item 18), and tool-free charging (item 19).

## What the House of Quality says to focus on
1. **Build time (H2) and solder joints (H11)** together carry ~19% of the technical importance. Time the first prototype build and count joints, and re-plan before the batch if it exceeds 60 min.
2. **Enclosure impact resistance (H10)** is #2 because safety and durability are both Musts. Confirm the cousin age split and the PET jar thread fit early ([DD-10](./DD-10-Enclosure.md)).
3. **Runtime (H5) and standby current (H3).** The model gives ~214 days, but forum-measured untuned XIAO Sense sleep (100 µA to 1.24 mA) would cut that to 109 days, or as little as 14. **First Destroy test: measure sleep current ≤ 20 µA before ordering the other 11 boards.**
4. **Wake paths (H6)** are settled (LDR + IMU); keep false wakes (H14) in check with LDR hysteresis and the re-arm rule ([DD-5](./DD-5-Glow-Cycle.md)).
5. Cost (H1) sits mid-table (#7). This matches your stated intent: cost keeps the budget in control and is traded deliberately, not minimized.
