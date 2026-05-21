# Wireless AC Power Detector ⚡🔍

A non-contact AC power detection circuit using ESP32 that senses the electromagnetic field of live AC wires without any physical contact. Uses a coil antenna and NPN transistor amplifier stage, with LED, buzzer, and LCD feedback.

---

## 📋 Features

- Non-contact live wire detection using electromagnetic induction
- NPN transistor (BC547) amplification of induced coil signal
- ESP32 ADC reading with 20-sample averaging for stable detection
- Buzzer alert when live wire is detected
- LED status indicator
- 16x2 I2C LCD display with real-time status
- Adjustable sensitivity via `DETECTION_THRESHOLD` constant
- Serial Monitor output for debugging and calibration

---

## ⚙️ Working Principle

```
Live AC wire
    │  (50Hz EM field radiates outward)
    ▼
[Coil Antenna]  ← picks up induced voltage
    │
  100kΩ
    │
[BC547 Base]
[BC547 Emitter] → GND
[BC547 Collector] → 10kΩ Pull-up (3.3V) → GPIO34 (ADC)

When AC field present: transistor conducts → ADC reads LOW-ish
When no field: transistor off → ADC reads HIGH (pulled up)
```

> The coil can be made from **20–30 turns of insulated wire** wound around a small ferrite rod or pencil. Longer coil = more sensitive.

---

## 🔧 Components Required

| Component             | Quantity | Notes                          |
|-----------------------|----------|--------------------------------|
| ESP32 DevKit v1       | 1        |                                |
| BC547 NPN Transistor  | 1        | Or 2N2222, S8050               |
| Coil / Antenna        | 1        | 20–30 turns on ferrite / air   |
| 100kΩ Resistor        | 1        | Base bias resistor             |
| 10kΩ Resistor         | 1        | Collector pull-up              |
| 220Ω Resistor         | 1        | LED current limit              |
| LED (Red/Yellow)      | 1        | Detection indicator            |
| Piezo Buzzer          | 1        | Alert                          |
| 16x2 I2C LCD          | 1        | Address 0x27 or 0x3F          |
| Breadboard + wires    | —        |                                |

---

## 🔌 Wiring Diagram

```
Coil Antenna
     │
   100kΩ
     │
     ├────────────────────── BC547 BASE
     │
     │    3.3V
     │      │
     │    10kΩ
     │      │
     │      ├──────────────── GPIO34 (ADC)
     │      │
     │    BC547 COLLECTOR
     │    BC547 EMITTER ───── GND

GPIO25 ──► Buzzer (+) ──► Buzzer (–) ──► GND
GPIO26 ──► 220Ω ──► LED (+) ──► LED (–) ──► GND

LCD SDA ──► GPIO21
LCD SCL ──► GPIO22
LCD VCC ──► 5V
LCD GND ──► GND
```

### Pin Summary

| ESP32 Pin | Connected To         |
|-----------|----------------------|
| GPIO34    | Transistor Collector |
| GPIO25    | Buzzer (+)           |
| GPIO26    | LED (via 220Ω)       |
| GPIO21    | LCD SDA              |
| GPIO22    | LCD SCL              |

---

## 🎛️ Calibration

Adjust the `DETECTION_THRESHOLD` in the code:
```cpp
#define DETECTION_THRESHOLD  1500   // ADC value (0–4095)
```
- Open Serial Monitor, bring coil near a known live wire
- Note the ADC value when detecting vs not detecting
- Set threshold between these two values

---

## ⚠️ Safety Note

This device **detects only the electromagnetic field** — it does not contact the live wire. Never touch live wires directly. Use only for detection and field awareness purposes.

---

## 📄 License

MIT License — see [LICENSE](LICENSE)

---

## 👤 Author

**Saksham Chamoli**
B.Tech ECE — College of Technology, GBPUAT Pantnagar
[LinkedIn](https://www.linkedin.com/in/saksham-chamoli-1a7945333/) | [GitHub](https://github.com/your_username)
