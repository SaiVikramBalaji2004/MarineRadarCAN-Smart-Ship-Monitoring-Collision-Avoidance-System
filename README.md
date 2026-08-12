<div align="center">

# ⚓ Marine RadarCAN

## CAN-Based Smart Ship Monitoring & Collision Warning System

A **distributed, real-time maritime safety system** built on three NXP LPC2129 nodes that talk over a **500 kbps CAN 2.0B** bus — scanning the seas ahead, guarding the engine room, and shouting *DANGER* before you hit something.

<br>

![MCU](https://img.shields.io/badge/MCU-NXP%20LPC2129%20(ARM7TDMI--S)-blue)
![Bus](https://img.shields.io/badge/Bus-CAN%202.0B%20%40%20500%20kbps-orange)
![Language](https://img.shields.io/badge/Language-Embedded%20C%20(Bare--Metal)-brightgreen)
![Nodes](https://img.shields.io/badge/Nodes-3%20ECUs-9cf)
![Range](https://img.shields.io/badge/Sensor-HC--SR04%20Ultrasonic-critical)

</div>

---

## 🚀 What Is This?

**Marine RadarCAN** decentralizes a ship's safety-critical tasks across **three specialized Electronic Control Units (ECUs)**:

| Node | Role | The "Big Job" |
|------|------|---------------|
| 🛰️ **ECU-1** | Radar Navigation | Sweeps a servo-mounted ultrasonic radar, measures obstacle distance & angle |
| 🛡️ **ECU-2** | Ship Safety Monitor | Watches engine temperature, fire, gas leaks and bilge water level |
| 📟 **ECU-3** | Alert Master | Captain's HMI — LEDs, buzzer and a 20×4 LCD, fed by a hardware acceptance filter |

> **The golden rule of this system:** *any* valid obstacle echo ⟹ the ship instantly enters a **DANGER** state. Green means clear. Red means stop.

---

## 🧠 System Architecture

```
                        ┌───────────────────────────┐
   ┌──────────────┐     │    CAN Bus 2.0B @ 500k    │     ┌──────────────┐
   │    ECU-1     │────▶│   (differential pair,     │◀────│    ECU-3     │
   │  Radar Node  │ CAN1│    120Ω terminated )      │ CAN1│  Master Node │
   └──────────────┘     │                           │     └──────────────┘
                        │  ◀───────────────────────▶│
                        └───────────┬───────────────┘
                                    │
                        ┌───────────▼───────────────┐
                        │    ECU-2                  │
                        │  Safety Monitor (CAN2)    │
                        └───────────────────────────┘
```

### 🗺️ CAN Message Dictionary

| CAN ID | Source | Byte 0 | Meaning |
|:------:|:------:|:------:|---------|
| `0x101` | ECU-1 | `0..180` | Real-time servo heading (°) |
| `0x102` | ECU-1 | `0..255` | Obstacle proximity (cm) |
| `0x103` | ECU-1 | `0x00 / 0xFF` | Collision status — SAFE / **DANGER** |
| `0x104` | ECU-2 | `0..150` | Engine temperature (°C) |
| `0x105` | ECU-2 | `0x00 / 0xFF` | Fire status |
| `0x106` | ECU-2 | `0x00 / 0xFF` | Gas leak status |
| `0x107` | ECU-2 | `0x00 / 0xFF` | Bilge water level |

---

## ✨ Features

- 🛰️ **Mechanical radar sweep** — SG90/MG995 servo (PWM2) pans `0° → 180°` while the HC-SR04 takes precise echo-timed readings via the `CAP0.2` hardware capture.
- 🧮 **Hardware-timed ranging** — echo pulse width measured with a 1 µs timer capture → `distance = (t × 0.0343) / 2` cm.
- 🛡️ **Multi-sensor safety net** — LM35 thermal sensing (10-bit ADC) + flame, gas and water-level digital inputs.
- 📡 **Hardware acceptance filtering** — ECU-3's AF RAM is programmed to accept *only* IDs `0x101 – 0x107`; every other frame on the bus is dropped by hardware.
- 🚨 **Interrupt-driven alarm** — a Timer1 match interrupt blinks the red LED every 500 ms and drives the buzzer *without* blocking the main loop.
- 📟 **20×4 LCD HMI** — 4-bit interfacing on P0.16–P0.21 to conserve GPIO, rendering live navigation + engine telemetry.
- 🖥️ **Boot splash** — the project title is shown for the first 10 seconds on every power-up and restart.

---

## 🛠️ Hardware Requirements

| Part | Qty | Purpose |
|------|:---:|---------|
| NXP LPC2129 (ARM7TDMI-S) dev board | 3 | One per ECU |
| MCP2551 CAN transceiver | 3 | Bus ↔ MCU line driver |
| 120 Ω termination resistors | 2 | At ECU-1 & ECU-3 bus ends |
| HC-SR04 ultrasonic sensor | 1 | Forward obstacle detection |
| SG90 / MG995 servo motor | 1 | Radar sweep mechanism |
| LM35 temperature sensor | 1 | Engine temperature |
| Flame sensor module | 1 | Fire detection |
| MQ-2 gas sensor module | 1 | Gas leak detection |
| Water level sensor | 1 | Bilge flooding |
| 20×4 HD44780 LCD | 1 | Captain's display |
| Red / Green LEDs | 1 ea | DANGER / SAFE status |
| Active buzzer + NPN driver | 1 | Audible alarm |

---

## 🔌 Pin Mapping

### 🛰️ ECU-1 — Radar Navigation Node
| Signal | Pin | Config |
|--------|:---:|--------|
| CAN1 RX | P0.25 | `RD1` (TD1 is a dedicated pin) |
| HC-SR04 Trigger | P0.8 | GPIO output |
| HC-SR04 Echo | P0.16 | Capture input `CAP0.2` |
| Servo control | P0.7 | `PWM2` — 50 Hz, 1 ms(0°)→2 ms(180°) |

### 🛡️ ECU-2 — Ship Safety Monitoring Node
| Signal | Pin | Config |
|--------|:---:|--------|
| CAN2 RX / TX | P0.23 / P0.24 | `RD2` / `TD2` |
| LM35 temperature | P0.28 | `AD0.1` (10-bit ADC) |
| Flame sensor | P0.10 | Digital input |
| Gas sensor | P0.11 | Digital input |
| Water level sensor | P0.12 | Digital input |

### 📟 ECU-3 — Captain Control & Alert Master Node
| Signal | Pin | Config |
|--------|:---:|--------|
| CAN1 RX | P0.25 | `RD1` (TD1 is a dedicated pin) |
| Green LED (SAFE) | P1.16 | GPIO output |
| Red LED (DANGER) | P1.17 | GPIO output, Timer1 blink |
| Active buzzer | P1.18 | GPIO output (via NPN) |
| LCD RS / EN | P0.16 / P0.17 | GPIO output |
| LCD D4–D7 | P0.18 – P0.21 | GPIO output (RW = GND) |

---

## 📟 HMI Display Layouts

**State 1 — CLEAR PATH**
```
RADAR: CLEAR        
OBJ DIST: NONE      
ENG: 85C | WTR: OK  
FIRE: NO | GAS: NO  
```

**State 2 — OBSTACLE DETECTED**
```
!!! DANGER !!!      
OBJ AT: 45 DEG      
ENG: 85C | WTR: OK  
FIRE: NO | GAS: NO  
```

**Boot Splash (first 10 seconds)**
```
CAN-Based Smart Ship
Monitoring and
Collision Warning
System
```

---

## 📁 Repository Structure

```
ECUs/
├── ECU1_Radar/                 # 🛰️ Radar Navigation Node (CAN1 TX)
│   ├── ecu1.h                  #   pin map, CAN IDs, prototypes
│   ├── main.c                  #   servo sweep + ranging super-loop
│   ├── can.c                   #   CAN1 controller init & transmission
│   ├── hcsr04.c                #   HC-SR04 driver (CAP0.2 capture)
│   ├── servo.c                 #   PWM2 servo driver
│   └── timer.c                 #   Timer1 delay helpers
│
├── ECU2_Safety/                # 🛡️ Ship Safety Monitoring Node (CAN2 TX)
│   ├── ecu2.h                  #   pin map, CAN IDs, prototypes
│   ├── main.c                  #   sensor sampling super-loop
│   ├── can.c                   #   CAN2 controller init & transmission
│   ├── sensors.c               #   LM35 + flame/gas/water drivers
│   └── timer.c                 #   Timer0 delay helpers
│
└── ECU3_Master/                # 📟 Captain Control & Alert Master (CAN1 RX)
    ├── ecu3.h                  #   pin map, CAN IDs, telemetry globals
    ├── main.c                  #   alarm state machine + LCD refresh
    ├── can.c                   #   acceptance filter 0x101–0x107 + Rx ISR
    ├── lcd.c                   #   20×4 HD44780 driver (4-bit mode)
    ├── alarm.c                 #   LEDs, buzzer, Timer1 blink ISR
    └── timer.c                 #   Timer0 delay helpers
```

---

## 🛠️ Getting Started

### Prerequisites
- **Keil µVision** (MDK-ARM) or any ARM7 toolchain with the LPC2129 device pack
- The standard Keil `LPC21xx.h` register header in your include path
- A 60 MHz core clock (15 MHz VPB) — the bit timing tables assume `PCLK = 15 MHz`

### Build
1. Create a **Keil project** for the LPC2129 in `ECU1_Radar`, `ECU2_Safety`, and `ECU3_Master`.
2. Add all `.c` files of that folder to the project (add the folder to the include path).
3. Compile → Flash each board via JTAG / ISP.
4. Wire the three nodes to a shared CAN bus with **120 Ω terminators** at ECU-1 and ECU-3.

### Wire the CAN bus
```
ECU-1 ──CAN_H──┬────────────────────┬──CAN_H── ECU-3
       ──CAN_L──┤ 120Ω       120Ω  ├──CAN_L──
               ├──── CAN_H ────────┤
               └──── CAN_L ────────┘
                        │
                      ECU-2
```
> 🧯 Keep a **common ground** across all nodes and route the differential pair over an unbroken ground plane.

---

## ⚙️ How It Works

```
ECU-1                          ECU-3
┌──────────────────┐           ┌────────────────────────────┐
│ Sweep servo 0-180°│           │  CAN1 Rx ISR updates the   │
│ Fire 10µs trigger │ CAN 0x101-0x103 │  shared telemetry globals │
│ Capture echo time │──────────▶│  Acceptance filter 0x101.. │
│ Compute distance  │           │  0x107 drops all the rest  │
│ d = (t·0.0343)/2  │           │  collision == 0xFF ?       │
└──────────────────┘           │   YES ⟹ red LED blinks     │
                               │        + buzzer ON         │
ECU-2                          │   NO  ⟹ green LED solid    │
┌──────────────────┐           │   LCD refreshed with state │
│ Read LM35 (ADC)  │ CAN 0x104-0x107 │  every 150 ms          │
│ Read fire/gas/   │──────────▶└────────────────────────────┘
│ water inputs     │
└──────────────────┘
```

---

## 📝 Design Notes

- **Deterministic payloads** — every frame is packed into a standard `CAN_Message_t` (`id`, `dlc`, `data[8]`) before hitting the CAN transmit registers.
- **No busy-wait alarms** — the red LED blink lives in a Timer1 ISR, so CAN reception and LCD refresh never stall.
- **Strict binary safety** — any valid echo within the sensor's max range sets `0x103 = 0xFF`, forcing the primary display into the DANGER state.
- **Acceptance filtering offloads the CPU** — ECU-3 never sees bus noise, only the seven telemetry IDs it cares about.
- *Chip suffix check:* the acceptance-filter controller number in `ECU3_Master/can.c` is `1` for no-suffix `/00` LPC2129 devices — set it to `0` on `/01` silicon.

---

## 🧭 What's Next?

- [ ] EEPROM logging of alarms (I²C) for post-trip review
- [ ] UART debug channel per node
- [ ] GPS/AIS track overlay on the HMI
- [ ] FullCAN mode to free the ECU-3 CPU entirely

---

<div align="center">

**Built with ❤️ and a healthy fear of icebergs.**

</div>
