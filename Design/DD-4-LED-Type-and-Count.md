# Design Decision: LED Type and Count ("Swarm Density")

**Asset:** [6080006](./6080006.md)

## Decision
What kind of LED "firefly" should be used, and how many per jar give the most convincing swarm without cluttering the jar? This resolves the [Spec-2](./Spec-2.md) open item "final LED count/placement for the most convincing swarm density." Placement (bottom-up vs. top-hung) is decided in [DD-7](./DD-7-LED-Stem-Mounting.md). Color is decided in [DD-8](./DD-8-LED-Color.md).

## Criteria
- **Item 1, firefly-in-a-jar look (Must):** each light reads as a *point* of light floating in the jar, not a visible bulb.
- **Item 12, maximum lasting joy (Should):** enough fireflies for call-and-response and sync effects ([DD-9](./DD-9-Glow-Pattern.md)), but not a Christmas-light cluster.
- **Item 18, build time of 1 hour or less (Should):** joints per LED and how fiddly they are.
- **Item 17, durability (Must):** the LED-to-wire joint must survive shaking.
- **Item 13, flexible platform (Should):** every LED uses a PWM pin. The XIAO has 11 GPIO, and the LDR uses 1.
- **Item 3, budget (Should)** and battery life (each LED adds ~0.33 mA while active; see the [DD-3 model](./DD-3-Power-Source.md#reference-use-profile-and-battery-model)).
- **Item 16, schedule (Must):** domestic or fast shipping.

## Discussion / Research
**LED types.**
- **Pre-wired 0603 SMD, 3 V, no resistor, 30 cm leads.** [eBay listing](https://www.ebay.com/itm/227363613241) (seller with 33K+ feedback at 100%): $29.99 per 100 + $3 shipping (~$0.33 each). Colors: red, yellow, blue, green, white, orange, warm white. Ships from Hong Kong, with an estimated Sun Prairie delivery of Oct 19-23.
  - An 0603 chip is 1.6 x 0.8 mm. Lit inside a frosted jar, it reads as a true point of light.
  - Zero solder joints at the LED.
  - Rated 3 V, so it runs from a 3.3 V GPIO through a 470 Ω-1 kΩ series resistor.
  - Avoid the 12 V "pre-wired" packs with built-in resistors (for example this [12 V 0603 listing](https://www.ebay.com/itm/153020220975)), which barely glow at 3.3 V.
- **Bare 0603 chips on magnet wire.** For example, [Dialight 5990060007F](https://www.digikey.com/en/products/detail/dialight/5990060007F/9385405), 569 nm yellow-green, 2.1 V Vf. You get the most authentic color ([DD-8](./DD-8-LED-Color.md)), but at the cost of **2 hand-soldered 0603 joints per LED** (12 per jar). That's an estimated +20-30 min per jar, and the joint is the weakest point for durability.
- **3 mm / 5 mm diffused through-hole.** Cheap (~$0.10) and robust, but the visible plastic bulb reads as "an LED," not a firefly, which works against item 1.
- **Copper "fairy wire" strings.** All LEDs on the strand are wired in parallel, so they can't fade independently. That defeats the firefly read (item 1) unless each LED is cut free.

**Count.**
- **4:** reads as a few lights, and call-and-response is thin.
- **6:** enough for overlapping independent flashes plus answer pairs, and leaves **3 spare PWM pins** for item 13 (the LDR uses 2 pins: analog input + switched power).
- **8:** a richer swarm. Uses 10 of 11 pins, leaving 1 spare.
- **10+:** exceeds what the XIAO pins support without charlieplexing, and starts to look like string lights.
- Real *Photinus pyralis* males flash roughly every 5-7 s ([Wikipedia](https://en.wikipedia.org/wiki/Photinus_pyralis)). With 6 LEDs on a ~6 s cadence and randomized offsets, something is almost always glowing, which is what sells "a jar full of fireflies."

**Battery (reference profile, XIAO Sense, 500 mAh):**
| LEDs | mAh/day | Days per charge |
|---|---|---|
| 4 | 1.71 | ~263 |
| 6 | 2.10 | ~214 |
| 8 | 2.48 | ~181 |

## Final Decision
**6 pre-wired 0603 3 V LEDs (30 cm leads) per jar**, each on its own XIAO PWM pin through a 1 kΩ series resistor (tune down to 470 Ω if too dim). Order one 100-pack, which covers 72 LEDs for 12 jars plus 28 spares. Two of the 3 spare PWM pins are reserved so a "+2 fireflies" upgrade, or an extra jar ornament (item 13), can be added later without redesign.

## Explanation
- **Item 1:** an 0603 point source reads as a firefly. A 3/5 mm bulb doesn't.
- **Items 18 and 17:** pre-wired means no fiddly 0603 joints. That's ~12 fewer hand joints per jar than bare chips, and the factory joint is more robust.
- **Item 12:** 6 independently PWM'd LEDs support meadow, call-and-response, and sync patterns ([DD-9](./DD-9-Glow-Pattern.md)).
- **Item 13:** 3 spare PWM pins.
- **Item 3:** ~$2.00/unit for LEDs.
- **Battery:** ~214 days/charge. Going to 8 costs ~33 days, which is acceptable as a later upgrade.

## Options Summary
| Forced Rank | Option | Est. Cost / Unit | Cost Implication | Battery Life Implication | Joy (1-5) | Upside | Downside |
|---|---|---|---|---|---|---|---|
| 1 | 6x pre-wired 0603 3 V (30 cm leads) | ~$2.00 (6 x $0.33) | Baseline | ~214 days/charge | 4 | True point-of-light look; no LED-end joints; 3 spare PWM pins | Overseas shipping (Oct 19-23 est.); limited color choice |
| 2 | 8x pre-wired 0603 3 V | ~$2.65 | +$0.65 vs. rank 1 | ~181 days/charge (−33 days) | 5 | Denser swarm, richer sync effect | Only 1 spare pin; busier lid wiring; +4 joints |
| 3 | 6x bare 0603 (Dialight 569 nm) on 30 AWG magnet wire | ~$2.10 (LEDs est. + wire) | ~+$0.10 vs. rank 1 | ~214 days (slightly better: yellow-green is brightest per mA to the eye, so it can run lower current) | 5 | Most authentic firefly color; domestic stock | +12 fiddly 0603 joints/jar (~+20-30 min); weakest durability point |
| 4 | 4x pre-wired 0603 3 V | ~$1.32 | −$0.68 vs. rank 1 | ~263 days/charge (+49 days) | 3 | Simplest; longest battery | Sparse; weak call-and-response |
| 5 | 6x 3 mm diffused through-hole on wire stems | ~$0.60 (est.) | −$1.40 vs. rank 1 | ~214 days/charge | 2 | Cheap, robust leads | Visible bulb reads as "LED," not firefly (item 1) |
| 6 | Copper fairy-wire LED string | ~$0.50 (est., per jar share) | −$1.50 vs. rank 1 | ~214 days (one PWM channel) | 1 | Cheapest, no soldering | All LEDs in parallel, so no independent fades; fails item 1 effectively |

## Sources
- [eBay: 0603 pre-wired 3 V SMD LEDs, 30 cm](https://www.ebay.com/itm/227363613241)
- [eBay: 12 V pre-wired 0603 (to avoid)](https://www.ebay.com/itm/153020220975)
- [DigiKey: Dialight 5990060007F, 569 nm 0603](https://www.digikey.com/en/products/detail/dialight/5990060007F/9385405)
- [Wikipedia: Photinus pyralis](https://en.wikipedia.org/wiki/Photinus_pyralis)
- [Seeed Studio XIAO nRF52840 Series wiki](https://wiki.seeedstudio.com/XIAO_BLE/)
