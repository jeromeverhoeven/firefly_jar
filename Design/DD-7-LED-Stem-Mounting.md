# Design Decision: LED Stem Mounting (Bottom-Up vs. Top-Hung) and Electronics Location

**Asset:** [6080009](./6080009.md)

## Decision
Should the LED "fireflies" rise on stems from a false-bottom deck (as in the [Spec-2](./Spec-2.md) Rev A renderings), or hang from the lid? Where do the electronics and battery live? This resolves the Spec-2 open item "LED stem mounting: bottom-up vs. top-hung."

## Criteria
- **Item 19, kid can restore power (Should):** with a USB-C LiPo ([DD-3](./DD-3-Power-Source.md)), the charge port must be reachable **without opening the jar**.
- **Item 17, durability (Must):** shaking and tapping are the main interaction ([DD-1](./DD-1-Trigger-Mechanism.md)), so stems and joints must survive repeated motion.
- **Item 1, look (Must)** and **item 12, joy (Should):** lights should appear to float.
- **Item 18, build time of 1 hour or less (Should):** one sub-assembly, simple wiring.
- **Item 15, safety (Must):** no loose wires or sharp ends in reach.
- **Fit:** a regular-mouth mason jar has a ~70 mm thread and a ~60 mm opening (verify with calipers). The XIAO (21 x 17.5 mm) and the 500 mAh cell (29 x 36 x 4.75 mm, [Adafruit](https://www.adafruit.com/product/1578)) must fit in the lid.
- **Compatibility with [DD-10](./DD-10-Enclosure.md):** the same lid assembly should fit both glass and PET jars.

## Discussion / Research
**The power decision changes this decision.** Rev A assumed 2xAA in the base, so bottom-up was natural. With a USB-C LiPo charged on the XIAO, a base-mounted board needs either a cable routed up to the lid or a port through the glass. Both are impractical. Putting all electronics in the lid puts the USB-C port on top, where a kid can plug in a phone charger without opening the jar.

**Top-hung mechanics.**
- The 30 cm pre-wired leads ([DD-4](./DD-4-LED-Type-and-Count.md)) hang from the lid at staggered lengths.
- A hanging wire carries only its own tiny weight, with no bending moment at the base. Shaking makes it *swing* rather than bend at the joint, which is better for item 17.
- The swing is also a joy bonus: shake the jar and the fireflies drift and sway before settling.
- Rev A's concern was that "opening/closing the jar stresses the wire connections every time." With USB-C charging there is no reason to open the jar in normal use. Opening is for service only.

**Lid construction.**
- Replace the flat metal lid insert with a **custom disc**: 3 mm plywood or acrylic cut to the insert diameter, or a 3D-printed disc, held by the standard screw band.
- The XIAO mounts **on edge** (perpendicular to the disc) on the underside, so its end-mounted **USB-C port faces up through a ~9.5 x 3.5 mm slot** in the disc. See the [lid layout drawing](./Spec-2-media/lid-disc-layout.svg).
- The cell is glued beside it, and the LDR peeks through a small hole in the disc.
- Each LED lead is **anchored at its own point** on the disc underside: it loops up through a pair of small anchor holes and back down (a mechanical strain relief), then gets a dab of hot glue and a short service loop to the XIAO pin.
- The air holes from [DD-6](./DD-6-Lid-Vent.md) go in the same disc.
- One sub-assembly per jar, and it swaps between glass and PET jars as long as both use regular-mouth threads.

**Tangle risk.** Leads of very different lengths can wrap around each other after hard shaking. Mitigations:
- Space anchor points ≥8 mm apart (the lid layout uses ~18 mm on a 26 mm-radius ring).
- Use lengths of roughly 6, 9, 12, 15, 18, and 21 cm.
- Optionally tie the longest two lightly into the moss so they hang in a gentle arc.

## Final Decision
**Top-hung.** All electronics (XIAO Sense, LiPo, LDR) mount on a **custom lid disc** under the standard regular-mouth screw band. The **USB-C port is exposed on top** of the lid, and 6 LED leads hang at staggered lengths anchored at ≥8 mm-spaced loop-through points with hot-glue strain relief. The XIAO mounts on edge so its USB-C port faces up through a slot. The false-bottom deck is dropped. The jar base holds only moss and dressing.

## Explanation
- **Item 19:** charge from the top of the lid; the jar never needs opening.
- **Item 17:** hanging leads swing instead of bending at the joint, and loop-through anchors protect the joints.
- **Items 1 and 12:** fireflies float and sway when the jar is moved.
- **Item 18:** a single lid sub-assembly is built flat on the bench, with no deck to fit inside the jar.
- **[DD-10](./DD-10-Enclosure.md):** the same lid fits both glass and PET regular-mouth jars.
- Note: the Spec-2 Rev A renderings show the superseded bottom-up layout.

## Options Summary
| Forced Rank | Option | Est. Cost / Unit | Cost Implication | Battery Life Implication | Joy (1-5) | Upside | Downside |
|---|---|---|---|---|---|---|---|
| 1 | Top-hung from custom lid disc; all electronics in lid; USB-C on top | ~$0.50 (disc, est.) | +$0.50; saves the false-bottom deck (~$0.50 est.), so about even | Enables the USB-C LiPo (DD-3): ~214 days/charge | 5 | Charge without opening; leads swing (durable, delightful); one sub-assembly; fits glass and PET | Lid is heavier; leads can tangle without spacing discipline; disc must be cut accurately |
| 2 | Hybrid: electronics in lid; leads anchored into moss base (taut) | ~$0.60 (est.) | +$0.10 vs. rank 1 | Same as rank 1 | 4 | Fixed, deliberate firefly heights | Jar can't be opened without disturbing the anchors; taut leads take shake loads at the joints |
| 3 | Bottom-up stems from false-bottom deck, LiPo charged via cable routed to lid | ~$1.00 (deck + cable, est.) | +$0.50 vs. rank 1 | Same as rank 1 | 3 | Matches the Rev A renderings | Extra cable and flex point; stems must be stiff; shaking bends stems at the base joints |
| 4 | Bottom-up stems with 2xAA holder in base (Rev A) | ~$0.50 (deck, est.) + DD-3 AA option | Ties the design to AA power | ~3.5 years on ATtiny / ~953 days on XIAO, but fails item 19 | 3 | Very long battery life | Must open the jar and remove the deck to change cells (fails item 19); stems bend under shake loads |

## Sources
- [Adafruit 500 mAh LiPo #1578 (dimensions)](https://www.adafruit.com/product/1578)
- [Seeed Studio XIAO nRF52840 Series wiki](https://wiki.seeedstudio.com/XIAO_BLE/)
- [Ball regular-mouth pint jars (HEB listing)](https://www.heb.com/product-detail/ball-regular-mouth-12-pk-pint-mason-jars-lids/30016)
