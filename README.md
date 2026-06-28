# 🚗 CAN Bus Based Automotive Dashboard

## Overview

This project implements a **CAN Bus Based Automotive Dashboard** using three Electronic Control Units (ECUs). Each ECU is responsible for processing specific vehicle parameters and communicating with other ECUs over the **Controller Area Network (CAN)**. The receiving ECU displays all vehicle information on a **16×2 LCD**, simulating a real automotive dashboard.

The project demonstrates distributed embedded system design, CAN communication, ADC interfacing, sensor integration, interrupt handling, and real-time dashboard updates.

---

## Features

* CAN-based communication between multiple ECUs
* Real-time vehicle parameter monitoring
* Speed measurement using ADC
* Gear position control using push buttons
* RPM measurement using ADC
* Engine temperature monitoring using LM35 sensor
* Left, Right, and Hazard indicator control
* LCD dashboard displaying all parameters in real time
* CAN message parsing using Message IDs
* Timer2-based indicator blinking

---

## ECU Breakdown

### ECU1 — Speed & Gear

* **Speed** — Potentiometer → ADC → Processed to **0–250 km/h**
* **Gear Position** — Two push buttons

  * Gear Up
  * Gear Down
* Transmits **Speed** and **Gear Position** over the CAN bus.

---

### ECU2 — RPM, Engine Temperature & Indicator

* **RPM** — Potentiometer → ADC → Processed to RPM value
* **Engine Temperature** — LM35 Temperature Sensor → ADC
* **Indicator Controls**

  * Left Indicator
  * Right Indicator
  * Hazard Indicator
* Transmits **RPM**, **Engine Temperature**, and **Indicator Status** over the CAN bus.

---

### ECU3 — Dashboard Display

* Receives CAN messages from ECU1 and ECU2
* Parses incoming data based on **CAN Message ID**
* Displays all vehicle parameters on a **16×2 LCD**
* Controls indicator LEDs with a **1-second blink rate** using **Timer2**

---

## CAN Message IDs

| Parameter          | Message ID |
| ------------------ | ---------- |
| Speed              | `0x10`     |
| Gear Position      | `0x20`     |
| RPM                | `0x30`     |
| Engine Temperature | `0x40`     |
| Indicator Status   | `0x50`     |

---

## LCD Dashboard Layout

```
SPD:120 G:3
RPM:3500 T:85
```

Indicator status is displayed whenever an indicator is active.

---

## Hardware Used

* PIC18F4580 Microcontrollers (3 ECUs)
* MCP2515 CAN Controller
* MCP2551 CAN Transceiver
* 16×2 LCD Display
* LM35 Temperature Sensor
* Potentiometers
* Push Buttons
* LEDs
* CAN Bus Network

---

## Software Used

* Embedded C
* MPLAB X IDE
* XC8 Compiler
* CAN Protocol
* ADC
* Timer2 Interrupts
* LCD Driver

---

## Project Workflow

1. **ECU1** reads speed and gear position and transmits them over the CAN bus.
2. **ECU2** reads RPM, engine temperature, and indicator status and transmits them over the CAN bus.
3. **ECU3** continuously listens for CAN messages.
4. Based on the received **Message ID**, ECU3 extracts the corresponding data.
5. The dashboard updates the LCD in real time and controls the indicator LEDs with a 1-second blink interval.

---

## Learning Outcomes

* CAN Bus communication between multiple ECUs
* Embedded C firmware development
* ADC and sensor interfacing
* Timer interrupt programming
* Message-based communication protocols
* Real-time embedded system design
* Automotive dashboard simulation

---

## Future Enhancements

* Odometer and trip meter
* Fuel level monitoring
* Battery voltage monitoring
* CAN error detection and diagnostics
* TFT graphical dashboard
* Data logging using EEPROM/SD card
* UART/Bluetooth-based diagnostics
