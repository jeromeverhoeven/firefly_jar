# Design Decision: LED Color

**Asset:** [6080010](./6080010.md)

## Decision
Which LED color best delivers the firefly look? [MuSCoW](../Define/MuSCoW.md) item 1 calls for "warm, flickering point-light sources," and its note leaves the exact color to a design decision.

## Criteria
- **Item 1, firefly-in-a-jar look (Must):** a warm point of light that reads as "firefly."
- **Item 12, maximum lasting joy (Should):** delight, and possibly personalization per cousin.
- **Item 18, build time (Should):** pre-wired availability ([DD-4](./DD-4-LED-Type-and-Count.md)).
- **Item 3, budget (Should):** minimum order quantities per color.
- **Battery:** forward voltage and perceived brightness per mA.

## Discussion / Research
**What real fireflies look like.** *Photinus pyralis*, the common eastern firefly, gives a **yellow** flash ([Wikipedia](https://en.wikipedia.org/wiki/Photinus_pyralis)). Firefly light generally ranges from yellow-green to yellow-orange. The yellow-green Dialight 0603 part is specified at **569 nm** ([DigiKey](https://www.digikey.com/en/products/detail/dialight/5990060007F/9385405)).

**What's available pre-wired.** The 3 V pre-wired 0603 listing ([eBay](https://www.ebay.com/itm/227363613241)) offers red, yellow, blue, green, white, orange, and **warm white**, but **no yellow-green**. Its "green" is almost certainly standard ~520 nm green, which reads minty or teal through glass rather than firefly.

**Battery and electrical notes.**
- Yellow and yellow-green LEDs have Vf ≈ 2.0-2.1 V. With a 3.3 V GPIO there is plenty of resistor headroom, and the eye is most sensitive near 555 nm, so yellow-green looks brightest per mA.
- Warm white is phosphor-converted (Vf ≈ 2.9-3.1 V), leaving only ~0.2-0.4 V across the resistor at 3.3 V. Brightness is less predictable, and it needs a lower resistor value.
- Same average current per LED in all cases, so battery impact is similar. Yellow and yellow-green can run dimmer for the same perceived brightness.

**Personalization.** Mixing colors per cousin, for example a yellow-green "classic firefly" jar for one and warm white for another, adds a personal touch. It means ordering multiple packs, though each 20-pack is $10.99.

## Final Decision
**Yellow pre-wired 0603 3 V LEDs** as the standard firefly color. It's the closest real-firefly hue available pre-wired, and it satisfies item 1's "warm" wording. Order one 100-pack in yellow. Optionally add one 20-pack of warm white for any cousin who would prefer a cozier night-light glow. Bare yellow-green (569 nm) remains the "authentic" upgrade if the prototype yellow reads too orange.

## Explanation
- **Item 1:** yellow matches the common eastern firefly and is warm.
- **Item 18:** available pre-wired, with no 0603 hand soldering.
- **Electrical/battery:** a 2 V-class LED has good headroom at 3.3 V and stable brightness.
- **Item 12:** optional warm-white jars allow light personalization without changing anything else.
- **Item 3:** one 100-pack covers all 12 jars plus spares.

## Options Summary
| Forced Rank | Option | Est. Cost / Unit | Cost Implication | Battery Life Implication | Joy (1-5) | Upside | Downside |
|---|---|---|---|---|---|---|---|
| 1 | Yellow pre-wired 0603 3 V | ~$2.00 (6 LEDs) | Baseline (one 100-pack) | ~214 days/charge; good resistor headroom | 4 | Closest available pre-wired match to a real firefly; warm | Slightly more orange than yellow-green fireflies |
| 2 | Warm white pre-wired 0603 3 V | ~$2.00 | Same | ~214 days; tight Vf headroom at 3.3 V makes brightness less predictable | 3 | Cozy night-light glow | Reads "fairy light" more than "firefly" |
| 3 | Yellow-green 569 nm bare 0603 (Dialight) on magnet wire | ~$2.10 (est.) | ~+$0.10 | ~214 days; can run lower current for the same perceived brightness (slight gain) | 5 | Most authentic firefly color | +12 hand 0603 joints per jar (~+20-30 min, item 18) |
| 4 | Mixed per cousin (yellow + warm white, or yellow-green for some) | ~$2.00 (yellow jars) to ~$3.30 (jar from a 20-pack at ~$0.55/LED) | +one extra pack ($10.99 for 20 up to ~$32.99 for 100 incl. shipping) | ~214 days | 5 | Personalized gifts | More SKUs to order and track |
| 5 | Pre-wired "green" (~520 nm) | ~$2.00 | Same | ~214 days | 1 | Available pre-wired | Reads teal/minty, not firefly (fails item 1 intent) |

## Sources
- [Wikipedia: Photinus pyralis](https://en.wikipedia.org/wiki/Photinus_pyralis)
- [DigiKey: Dialight 5990060007F, yellow-green 569 nm](https://www.digikey.com/en/products/detail/dialight/5990060007F/9385405)
- [eBay: 0603 pre-wired 3 V LEDs, color options and pricing](https://www.ebay.com/itm/227363613241)
