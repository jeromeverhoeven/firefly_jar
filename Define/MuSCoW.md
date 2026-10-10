# MuSCoW

|Instructions|
|----|
|A MuSCoW mean **Mu**st, **S**hould, **Co**uld, **W**ont: each line should be a user story, constraint, criteria, feature, parameter, or something else that describes and defines you project. If it is important to the project write it down, if you don't how is the next guy supposed to know to consider it.|
|Each line must explained and then dispositioned as Must/Should/Could/Wont below are examples of each disposition.  Feel free to cut and paste the samples below|



| Num | Item | Expansion | MuSCoW | notes |
|----|----|----|----|----|
| 1 | Firefly-in-a-jar look | The product must visually read as fireflies glowing/captured inside a jar - a jar-style enclosure with warm, flickering point-light sources inside | **Must** | Core aesthetic of the product, not optional |
| 2 | Battery operated | The unit must run entirely on battery power, no wall adapter/mains power required | **Must** | Enables gifting/portability use case |
| 3 | Unit cost < $20 @ Qty 10 | Total BOM + assembly cost per unit should be under $20 when built in a batch of 10 units | *Should* | Target cost ceiling for the pilot batch; drives component selection, but not an absolute blocker |
| 4 | Shake to activate (accelerometer) | Device could use a low-cost accelerometer (e.g. ADXL-series) to detect motion/shaking of the jar and trigger the firefly glow effect | *Could* | More sensitive/configurable than a tilt switch, but higher part cost and needs MCU logic to debounce |
| 5 | Shake to activate (tilt/vibration switch) | Device could use a simple mechanical tilt or vibration switch (ball switch / spring switch) to detect shaking and trigger the glow effect | *Could* | Cheapest shake-detection option; binary on/off, no analog logic needed - good budget fit |
| 6 | Low-light to activate | Device could use a light sensor (e.g. photoresistor) so dim/dark ambient conditions trigger the glow effect, mimicking real fireflies at dusk | *Could* | Likely cheapest sensor option overall; good budget fit |
| 7 | Sound/clap to activate | Device could use a microphone or sound sensor so talking or clapping near the jar triggers the glow effect | *Could* | Needs a mic + simple threshold detection circuit; check cost impact |
| 8 | Single-trigger activation | Device could use just one trigger input (one sensor from items 4-7) to activate the glow effect | *Could* | Simplest and cheapest logic; lowest part count, fits the $20 budget most easily |
| 9 | OR'd multi-trigger activation | Device could activate the glow effect when ANY one of the selected sensors (items 4-7) fires - e.g. shake OR low-light OR sound | *Could* | More responsive/forgiving trigger behavior; needs combining logic (diode-OR or MCU) across chosen sensors |
| 10 | AND'd multi-trigger activation | Device could require that two or more of the selected sensors (items 4-7) fire together/in sequence before activating the glow effect - e.g. low-light AND shake | *Could* | More deliberate/novel trigger behavior but adds logic complexity and may need an MCU; check cost and false-trigger-avoidance tradeoff |
| 11 | Maximum joy in use | The product should bring maximum joy to the user in its use and operation - the experience of discovering/triggering the glow and watching it should feel delightful, not just functional | *Should* | Qualitative/experiential requirement; weigh against other Design decisions (timing, LED behavior, enclosure feel) even though it isn't directly measurable like cost or power draw |
| 12 | Flexible power/animation platform | The driver electronics should be flexible enough to power, light, and/or animate other small items placed in the jar - not hard-wired to only the firefly LED stems | *Should* | Pushes toward a general-purpose output (e.g. a few spare switched/PWM outputs or a simple connector header) rather than a fixed, single-purpose LED driver; impacts Design's choice of MCU I/O count and connector strategy |

