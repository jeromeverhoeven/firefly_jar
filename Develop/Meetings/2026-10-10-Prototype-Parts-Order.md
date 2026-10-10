# Development Meeting: Prototype Parts Order & BOM Verification

**Date:** 2026-10-10
**Attendees:** JP (Jerome Verhoeven)
**Purpose:** First Develop-phase meeting. Agree the first parts order (prototype build) and verify it against the [BOM](../BOM.md) ([6080013](../6080013.md)) and [Spec-2](../../Design/Spec-2.md) before anything is purchased.

## Agenda
1. Confirm cousin age split (PET vs. glass jar count) — [Spec-2 Open Items](../../Design/Spec-2.md#open-items)
2. PET jar vendor risk — the BOM's substitute vendor is unverified for regular-mouth band fit ([DD-10](../../Design/DD-10-Enclosure.md))
3. Define the prototype-only order list and quantities (vs. the full 12-unit batch)
4. Re-confirm vendor/part choices are still current (stock, price) since the BOM was drafted
5. Budget check for the prototype order
6. Owners and dates for placing the order; confirm the next milestone
7. Risks / parking lot

## Discussion & Decisions

### 1. Cousin age split (PET vs. glass)
Still open overall — full list of 12 depends on Brook. Three ages known so far:

| Cousin | Age | Jar (per DD-10: ≤12 → PET, 13-16 → glass) |
|---|---|---|
| Mara | 14 | Glass |
| Paul | 12 | PET |
| Quinn | 9 | PET |

Running tally: 2 PET, 1 glass, 9 cousins still unknown. Not a blocker for the prototype order (item 3 below covers one PET + one glass test jar regardless of the final split), but it is needed before the full 12-unit jar order.

### 2. PET jar vendor risk / enclosure material
**Decision: postponed.** No jar purchase for now. The prototype will be assembled from parts already on hand. The enclosure aesthetic (and possibly the material itself) needs Brook's input as the project's artist, and she isn't at this meeting. A new option surfaced: using an **heirloom glass jar** instead of a purchased PET or Ball glass jar, for some or all cousins. Final jar material/vendor decision — including whether this changes [DD-10](../../Design/DD-10-Enclosure.md) — is deferred to a meeting where Brook is present, and partly depends on the age-split answer from item 1.

### 3. Prototype order list
Decision: order only what's missing. On hand already: hookup wire, hot glue, lid disc material (no purchase needed for these). To order for the prototype:

| Item | Qty | BOM ref |
|---|---|---|
| Seeed XIAO nRF52840 Sense | 1 | [U1](../BOM.md) |
| LiPo 500 mAh battery | 1 | [BT1](../BOM.md) |
| Pre-wired 0603 LED pack | 1 pack (100 ct.) | [D1-D6](../BOM.md) |
| CdS photoresistor (LDR) | 2 | [LDR1](../BOM.md) |

**Resistors (R1-R6 1 kΩ, R7 100 kΩ):** left off this order for now — JP may already have them on hand from existing stock. Check before the prototype build; order only if actually missing.

**Open question raised in the meeting:** the BOM's J1 JST-PH pigtail isn't on this order list — need to confirm whether the on-hand hookup wire substitutes for it on the prototype (solder straight to the battery's JST-PH pins or to the XIAO BAT pads) or whether J1 still needs to be ordered.

### Resistor value review (R1-R6, R7)
Reviewed whether the series/pull-down resistors are necessary and whether the values are right.

- **R1-R6 (LED series, 1 kΩ):** Necessary — an LED without a series resistor relies on two uncharacterized, part-to-part-variable nonlinearities (the LED's exact Vf and the nRF52840 GPIO's internal on-resistance) fighting each other, with no guaranteed current. Confirmed via the [Nordic nRF52840 datasheet](../../Appendix/Datasheets/Nordic-nRF52840-Product-Specification.pdf) (§6.9.3.1 GPIO Electrical Specification): standard-drive output current is 1 mA min / 2 mA typ / 4 mA max, and that's a recommended-operating figure, not a hardware clamp — exceeding it risks reliability, not a safe auto-limit. A comparable branded 0603 LED (Dialight 599-series, cited in [DD-4](../../Design/DD-4-LED-Type-and-Count.md)) is rated 20 mA continuous. At 1 kΩ (≈1.3 mA), we're at ~65% of typical GPIO drive and ~6-7% of a typical 0603 LED's rating — plenty of headroom either direction.
- **R7 (LDR pull-down, 100 kΩ):** Necessary — the LDR alone is just a variable resistor; a fixed resistor is required to form a voltage divider the ADC can read at all. 100 kΩ gives a wide swing (~9% of rail in the dark, ~80-90%+ in a lit room), confirmed against the [GL5528 datasheet](../../Appendix/Datasheets/GL5528-LDR-Datasheet.pdf): 8-20 kΩ at 10 lux, ≥1 MΩ dark (0 lux).

**Decision:** keep both resistors at their current Spec-2 values — **R1-R6 = 1 kΩ, R7 = 100 kΩ** — as the prototype starting point. Brightness (1 kΩ reads dim per the Spec-2 model) can be tuned to 470Ω or similar after seeing the lit prototype; this is a firmware/BOM tweak, not a redesign, since the GPIO and LED both have headroom confirmed above.

### 4. Vendor/part re-check
Caught a real BOM/design-decision mismatch: [BOM](../BOM.md) row U1 listed Seeed's own store as vendor, but [DD-2](../../Design/DD-2-Platform.md) had already found Seeed direct **out of stock as of Oct 9, 2026** and recommended **Mouser (713-102010469)** instead for that reason. **Fixed:** BOM row U1 now cites Mouser, matching DD-2. Logged as [6080013 Rev B](../6080013.md).

Also checked an LDR source JP found to combine shipping: a Chanzon 20-pack GL5528 listing on Amazon (ASIN B08QRYD4GR) whose title oddly reads "1 Ω ohm" — confirmed this is junk text in the listing title, not a real spec; the part is a genuine GL5528 match. Price wasn't independently confirmed (JP to read it off the live listing). Not yet reflected as a BOM vendor change.

### 5. Budget check (prototype order)
| Part | Cost (qty-1) |
|---|---|
| XIAO Sense (Mouser) | $15.90 |
| LiPo 500 mAh | $7.95 |
| LED 100-pack (+ship) | ~$32.99 |
| LDR x2 | $1.90 |
| **Total** | **~$58.74** |

Well inside the item-3 "easy yes" framing; the 100-pack of LEDs also covers spares for the eventual 12-unit batch.

### 6. Owners and dates
JP will place the prototype order **by next week (target 2026-10-17)**.

**Post-meeting update:** JP purchased the prototype parts, consolidating everything into **Amazon-only orders to avoid multiple shipping charges** — a real deviation from the BOM's per-line vendor list (Mouser, Adafruit, eBay, DigiKey, HEB), done for practical reasons. One purchase needs a closer look: the **yellow pre-wired LEDs bought have a resistor built into the lead assembly, rated for 3 V operation** — this differs from [DD-4](../../Design/DD-4-LED-Type-and-Count.md)'s selected part (bare pre-wired 0603, *no* resistor, relying on the external R1-R6 at 1 kΩ decided above). See next meeting's agenda — this needs verifying against the actual part received before we assume today's resistor decision still applies as-is.

## Action Items
| # | Action | Owner | Due | Status |
|---|---|---|---|---|
| 1 | Get ages for the remaining 9 cousins to complete the PET/glass split | Brook | TBD | Open |
| 2 | Review jar aesthetic and material options (PET / Ball glass / heirloom glass) with Brook; revisit [DD-10](../../Design/DD-10-Enclosure.md) if the decision changes | JP + Brook | Next meeting w/ Brook | Open |
| 3 | Order prototype parts: 1x XIAO Sense (Mouser), 1x LiPo 500 mAh, 1x LED 100-pack, 2x LDR | JP | 2026-10-17 | Done (ordered via Amazon, consolidated) |
| 4 | Confirm whether on-hand hookup wire substitutes for BOM's J1 JST-PH pigtail, or order J1 | JP | 2026-10-17 | Open |
| 5 | After prototype is lit, evaluate LED brightness and resistor need, given the purchased LEDs already include a built-in 3V-rated resistor | JP | After prototype build | Open |
| 6 | Verify live Mouser price-break (qty 10+) for the XIAO Sense before the full 12-unit order | JP | Before batch order | Open |
| 7 | Check existing stock for 1 kΩ and 100 kΩ resistors; order only if actually missing | JP | 2026-10-17 | Open |
| 8 | Reconcile actual purchased parts (Amazon-only, LED w/ built-in resistor) against the BOM and DD-4; update both if the substitution sticks | JP | Next meeting | Open |

## Open Items / Parking Lot
- PET jar vendor/fit risk carries forward from [DD-10](../../Design/DD-10-Enclosure.md) — unresolved, tied to action items 1 and 2.
- Mouser's live qty-10 price break for the XIAO Sense was not confirmed this meeting (action item 6).
- Final enclosure material (PET / Ball glass / heirloom glass) is explicitly undecided pending Brook (action item 2) — a candidate future update to [DD-10](../../Design/DD-10-Enclosure.md).
- Chanzon GL5528 Amazon listing (B08QRYD4GR) is a verified-genuine part at an unconfirmed price — worth a BOM vendor swap if cheap enough.
- **Purchased LEDs include a built-in, 3V-rated resistor** — differs from DD-4's bare-LED spec and from today's "keep external 1 kΩ" resistor decision. Needs inspection of the actual part once it arrives (is it still a point-source 0603, or a larger visible-bulb package? what's the built-in resistor's actual value/current?) before deciding whether R1-R6 are still needed at all.

## Next Meeting
Proposed agenda, once the prototype parts have arrived and Brook is available:
1. Cousin age split — review Brook's full list, finalize PET/glass counts (action 1)
2. Jar aesthetic & material — Brook's review of PET vs. Ball glass vs. heirloom glass (action 2)
3. **Reconcile actual purchased parts vs. BOM/DD-4** — Amazon-only sourcing, and the pre-wired LED's built-in 3V resistor vs. the external 1 kΩ decided today (action 8)
4. Prototype build status — first power-on, does it look/feel like fireflies
5. LED brightness/resistor call, based on what was actually built (action 5)
6. Sleep-current test planning (TC-FJ-01) — the gate before ordering the other 11 boards per [Spec-2](../../Design/Spec-2.md)'s deployment plan
7. New open items

Date: TBD — tentatively after the prototype order arrives and is built.
