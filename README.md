# Marine Radar 2.0

An embedded system project implementing a marine radar/safety monitoring system using multiple ECUs (Electronic Control Units) communicating over CAN bus on LPC21xx ARM7 microcontrollers.

## System Architecture

The system consists of three ECUs working together:

```
+----------------+     CAN Bus     +----------------+
|   ECU_1        |---------------->|   ECU_3        |
|  (Distance)    |     0x102,0x103 |  (Receiver)    |
+----------------+                 |                |
                                   |  - LCD Display |
+----------------+                 |  - UART Output |
|   ECU_2        |---------------->|                |
|  (Environment) |     0x201-0x204 +----------------+
+----------------+
```

## ECU Descriptions

### ECU_1_transmit - Distance Sensing Module

**Sensors:**
- HC-SR04 Ultrasonic Sensor (P0.3 Trigger, P0.4 Echo)
- Servo Motor (PWM on P0.3)

**Functionality:**
- Measures distance using ultrasonic sensor
- Sweeps servo motor through 3 positions (0°, 90°, 180°)
- Classifies obstacle status based on distance:
  - SAFE: Distance > 150 cm
  - WARNING: 50 cm < Distance ≤ 150 cm
  - DANGER: Distance ≤ 50 cm or out of range

**CAN IDs:**
| ID    | Data          |
|-------|---------------|
| 0x102 | Distance (cm) |
| 0x103 | Status (0/1/2)|

### ECU_2_transmit - Environment Monitoring Module

**Sensors:**
- LM35 Temperature Sensor (ADC0.1, P0.28)
- Gas Sensor (P0.8, Digital)
- Flame Sensor (P0.9, Digital)

**Functionality:**
- Reads temperature via 10-bit ADC
- Monitors gas and flame detection (active low)
- Classifies temperature status:
  - Normal: ≤ 35°C
  - Warning: 35°C - 45°C
  - High Temperature Alarm: > 45°C

**CAN IDs:**
| ID    | Data                |
|-------|---------------------|
| 0x201 | Temperature (°C)    |
| 0x202 | Temperature Status  |
| 0x203 | Gas Detection (0/1) |
| 0x204 | Flame Detection (0/1)|

### ECU_3_receive - Display & Monitoring Unit

**Peripherals:**
- 20x4 LCD Display
- UART0 (115200 baud)

**Functionality:**
- Receives data from both ECU_1 and ECU_2 via CAN
- Displays real-time data on LCD:
  - Line 1: Distance in cm
  - Line 2: Status (SAFE/WARNING/DANGER)
  - Line 3: Temperature
  - Line 4: Gas and Flame status
- Sends formatted data to UART for serial monitoring

**UART Output Example:**
```
DISTANCE  : 120 CM
STATUS: SAFE
TEMP      : 28 C
GAS       : NORMAL
FLAME     : NORMAL
```

## Hardware Requirements

- LPC21xx Development Board (ARM7TDMI)
- HC-SR04 Ultrasonic Sensor
- LM35 Temperature Sensor
- Gas Sensor (MQ-series)
- Flame Sensor Module
- 20x4 LCD Display
- Servo Motor
- CAN Bus Transceiver (e.g., MCP2551)

## Pin Configuration

### ECU_1
| Pin | Function |
|-----|----------|
| P0.2 | LED Output |
| P0.3 | HC-SR04 Trigger |
| P0.4 | HC-SR04 Echo |
| P0.23 | CAN2 RD2 |
| P0.24 | CAN2 TD2 |
| P0.3 | PWM3 (Servo) |

### ECU_2
| Pin | Function |
|-----|----------|
| P0.8 | Gas Sensor Input |
| P0.9 | Flame Sensor Input |
| P0.23 | CAN2 RD2 |
| P0.24 | CAN2 TD2 |
| P0.28 | ADC0.1 (LM35) |

### ECU_3
| Pin | Function |
|-----|----------|
| P0.14-P0.17 | LCD Data (4-bit) |
| P0.12 | LCD RS |
| P0.13 | LCD Enable |
| P0.0 | UART0 TXD |
| P0.1 | UART0 RXD |
| P0.23 | CAN2 RD2 |
| P0.24 | CAN2 TD2 |

## CAN Bus Configuration

- **Baud Rate:** 125 Kbps
- **PCLK:** 60 MHz (VPBDIV = 1)
- **Mode:** Normal (accept all messages)

## Building

This project is developed using Keil uVision IDE.

1. Open the respective `.uvproj` file for each ECU
2. Build the target
3. Flash to the LPC21xx board

## Project Structure

```
Marine Radar 2.0/
├── ECU_1_transmit/
│   ├── ECU1_transmit.c          # Main application
│   ├── ECU1_transmitdriver.c    # CAN driver
│   ├── CAN_header.h             # Header definitions
│   └── ECU1_transmit.uvproj     # Keil project file
├── ECU_2_transmit/
│   ├── ECU2_main.c              # Main application
│   ├── CAN_Transdriver.c        # CAN driver
│   ├── CAN_header.h             # Header definitions
│   └── ECU_main.uvproj          # Keil project file
└── ECU_3_receive/
    ├── ECU_receivemain.c        # Main application
    ├── CAN_RXdriver.c           # CAN receive driver
    ├── CAN_RXUARTdriver.c       # UART driver
    ├── CAN_header.h             # Header definitions
    ├── lcdheader.h              # LCD driver
    └── ECU_receive.uvproj       # Keil project file
```

## License

This project is for educational purposes.
