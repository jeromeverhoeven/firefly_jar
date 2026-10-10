# Datasheets

Manufacturer/vendor datasheets for the electronic parts on the [Bill of Materials](../../Develop/BOM.md) ([asset 6080013](../../Develop/6080013.md)). Craft/consumable items (jars, plywood, vellum, moss, glue, hookup wire) don't have manufacturer datasheets and aren't included here — see the BOM's vendor link for those.

| File | Covers (BOM ref) | Source |
|---|---|---|
| [Seeed-XIAO-SOM-Datasheet.pdf](./Seeed-XIAO-SOM-Datasheet.pdf) | U1 — Seeed XIAO nRF52840 Sense module | [files.seeedstudio.com](https://files.seeedstudio.com/wiki/XIAO/Seeed-Studio-XIAO-Series-SOM-Datasheet.pdf) |
| [Nordic-nRF52840-Product-Specification.pdf](./Nordic-nRF52840-Product-Specification.pdf) | U1 — nRF52840 SoC on the XIAO Sense | [files.seeedstudio.com](https://files.seeedstudio.com/wiki/XIAO-BLE/Nano_BLE_MCU-nRF52840_PS_v1.1.pdf) (Nordic's own product specification, mirrored by Seeed) |
| [ST-LSM6DS3-Datasheet.pdf](./ST-LSM6DS3-Datasheet.pdf) | U1 — LSM6DS3-family IMU on the XIAO Sense (tap/shake/tilt sensing, [DD-1](../../Design/DD-1-Trigger-Mechanism.md)) | [content.arduino.cc](https://content.arduino.cc/assets/st_imu_lsm6ds3_datasheet.pdf) mirror of the ST datasheet |
| [Adafruit-LSM6DS3TR-C-Guide.pdf](./Adafruit-LSM6DS3TR-C-Guide.pdf) | U1 — LSM6DS3TR-C variant specifically (pinout/registers), supplements the ST datasheet above | [cdn-learn.adafruit.com](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-lsm6ds3tr-c-6-dof-accel-gyro-imu.pdf) |
| [Adafruit-1578-LiPo-500mAh-Datasheet.pdf](./Adafruit-1578-LiPo-500mAh-Datasheet.pdf) | BT1 — 500 mAh protected LiPo cell ([DD-3](../../Design/DD-3-Power-Source.md)) | [datasheet.octopart.com](https://datasheet.octopart.com/1578-Adafruit-Industries-datasheet-26697955.pdf) |
| [JST-PH-Connector-Datasheet.pdf](./JST-PH-Connector-Datasheet.pdf) | J1 — JST-PH 2-pin connector (battery pigtail) | [jst-mfg.com](https://www.jst-mfg.com/product/pdf/eng/ePH.pdf) |
| [GL5528-LDR-Datasheet.pdf](./GL5528-LDR-Datasheet.pdf) | LDR1 — CdS photoresistor ([DD-1](../../Design/DD-1-Trigger-Mechanism.md)) | [handsontec.com](https://www.handsontec.com/dataspecs/sensor/GL55-LDR.pdf) |
| [YAGEO-CFR-Resistor-Datasheet.pdf](./YAGEO-CFR-Resistor-Datasheet.pdf) | R1-R7 — 1 kΩ and 100 kΩ 1/4 W carbon-film resistors | [cdn.sparkfun.com](https://cdn.sparkfun.com/assets/a/9/3/0/e/yageo-cfr_datasheet.pdf) mirror of the YAGEO CFR series datasheet |

Not included (no manufacturer datasheet exists for these): pre-wired 0603 LEDs (unbranded eBay listing), glass/PET jars, lid disc material, frosted vellum liner, preserved moss, hot-glue sticks, hookup wire. See their BOM rows for vendor links instead.
