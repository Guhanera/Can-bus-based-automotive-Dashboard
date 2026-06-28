---

## ECU Breakdown

### ECU1 — Speed & Gear
- **Speed** — Potentiometer → ADC → processed to 0–250 km/h
- **Gear Position** — Two push buttons (Gear Up / Gear Down)
- Transmits speed and gear data over CAN

### ECU2 — RPM, Engine Temp & Indicator
- **RPM** — Potentiometer → ADC → processed to RPM value
- **Engine Temperature** — LM35 temperature sensor → ADC
- **Indicator** — Three buttons (Left, Right, Hazard)
- Transmits RPM, engine temp, and indicator status over CAN

### ECU3 — Dashboard Display
- Receives all CAN messages
- Parses data based on Message ID
- Displays all parameters on 16x2 LCD in real time
- Controls indicator LEDs with 1-second blink rate using Timer2

---

## CAN Message IDs

| Message | ID |
|---|---|
| Speed | 0x10 |
| Gear | 0x20 |
| RPM | 0x30 |
| Engine Temp | 0x40 |
| Indicator | 0x50 |

---

## LCD Dashboard Layout
