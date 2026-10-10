# Design Decision: Lid Venting (Condensation / Heat / Sound)

**Asset:** [6080008](./6080008.md)

## Decision
Should the lid have vent holes for condensation or battery/charging heat, independent of sound sensing? This resolves the [Spec-2](./Spec-2.md) open item "whether to add a mechanical lid vent purely for condensation/battery heat."

## Criteria
- **Item 15, safety (Must):** no heat buildup around the LiPo during charging, and no openings that expose electronics to small fingers.
- **Item 17, durability (Must):** no moisture on the electronics.
- **Item 1, firefly-in-a-jar look (Must)** and **item 12, joy (Should):** does venting add to the story?
- **Item 7 / clap mode ([DD-1](./DD-1-Trigger-Mechanism.md)):** the mic hears better through openings.
- **Item 3, budget (Should)** and **item 18, build time (Should).**

## Discussion / Research
**Heat is negligible.**
- While running: 6 LEDs x ~2 mA peak x ~2 V ≈ 24 mW peak (far less on average), plus the nRF52840 at a few mW.
- While charging: the XIAO's linear BQ25101 charger at 100 mA dissipates roughly (5.0 − 3.7 V) x 0.1 A ≈ 0.13 W ([Seeed wiki](https://wiki.seeedstudio.com/XIAO_BLE/) for charge current). That's a warm spot on the board, not a hazard, spread across the lid disc and the air in the jar.
- The protected cell's over-charge/over-discharge cutoffs ([Adafruit #1578](https://www.adafruit.com/product/1578)) handle fault cases.
- Venting is not needed for heat.

**Condensation depends on what goes in the jar.** A jar sealed with moist air condenses when it cools (for example, a jar near a cold window at night).
- **Preserved (dried) moss** carries little moisture.
- **Live moss or anything damp** would fog the glass and could wet the lid electronics.
- Small holes let humidity equalize with the room.

**The thematic bonus.** Every kid who has caught fireflies poked **air holes in the lid**. Visible air holes make the jar instantly read as "fireflies live in here" (items 1 and 12). They also let the clap-mode mic hear clearly.

**Safety and finger access.** Holes of 1.5-2 mm are too small for fingers. The electronics in the lid puck ([DD-7](./DD-7-LED-Stem-Mounting.md)) sit behind the disc, and holes are placed away from the XIAO and the cell.

## Final Decision
Add **5-7 "air holes" (1.5-2 mm) in the lid disc**, arranged like a kid's bug-jar lid and placed clear of the XIAO and battery. Use **preserved (dry) moss only**; no live moss or water features.

## Explanation
- **Items 1 and 12:** air holes are the most recognizable firefly-jar detail, so they're free joy.
- **Item 17:** humidity equalizes; with dry moss only, there is no condensation source.
- **Item 15:** heat is negligible either way, and the holes are too small for fingers.
- **Clap mode:** better mic sensitivity.
- **Items 3 and 18:** $0, and about 2 min with a drill.

## Options Summary
| Forced Rank | Option | Est. Cost / Unit | Cost Implication | Battery Life Implication | Joy (1-5) | Upside | Downside |
|---|---|---|---|---|---|---|---|
| 1 | 5-7 small "air holes" in lid disc + dry preserved moss only | $0 | None | None (slightly better clap-mode mic pickup) | 5 | Iconic bug-jar look; humidity equalizes; better mic | Dust can enter (cosmetic) |
| 2 | Sealed lid + silica gel packet hidden in moss | ~$0.10 (est.) | +$0.10 | None | 3 | Guards against moisture even with damp contents | Hidden part to source; no thematic payoff; mic muffled |
| 3 | Sealed lid, no vent | $0 | None | None | 3 | Simplest; dust-free | Fogging risk if anything damp goes in; mic muffled |
| 4 | Fabric/mesh lid (traditional bug-catcher style) | ~$0.20 (est.) | +$0.20 | None | 4 | Very authentic look | Incompatible with lid-mounted electronics ([DD-7](./DD-7-LED-Stem-Mounting.md)); exposes wiring |

## Sources
- [Seeed Studio XIAO nRF52840 Series wiki (charge current)](https://wiki.seeedstudio.com/XIAO_BLE/)
- [Adafruit 500 mAh LiPo #1578 (protection circuit)](https://www.adafruit.com/product/1578)
