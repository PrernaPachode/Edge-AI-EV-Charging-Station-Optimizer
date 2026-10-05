# Edge AI Based Smart EV Charging Station Optimizer

A 3-bay EV charging station prototype that uses **ESP32, Edge AI, MQTT and ThingsBoard** to monitor charging demand and manage power across multiple charging bays.
## Problem Statement

- EV owners have no visibility into which bay is free or how long they will wait.
- Charging bays are used unevenly, with no coordination between them.
- Several EVs arriving together can demand more power than the station can supply (for example, about 8 kW demand against about 5 kW available).
- Operators have no predictive insight to plan power allocation ahead of demand.

## Proposed Solution

Each bay is an independent smart node controlled by its own ESP32. It senses voltage, current, power and temperature, runs an on-device Edge AI model, and decides whether to **ALLOW**, **THROTTLE** or **DEFER** charging. All telemetry is published over MQTT to a ThingsBoard dashboard for live monitoring, alarms and remote control.

Because decisions are made at the edge, each bay keeps working during a brief connectivity drop.

## Features

- 3 independently controlled charging bays
- Real-time sensing of voltage, current, power, energy and temperature per bay
- On-device Edge AI for EV arrival probability and charging-duration prediction
- ALLOW / THROTTLE / DEFER optimization logic based on bay load and station-wide load
- MQTT telemetry to a ThingsBoard dashboard (live status, trends, total station load)
- Alarms for overcurrent, offline sensors and high forecasted demand
- Remote relay override and threshold updates through RPC, with no reflashing needed
- Full circuit simulation in Wokwi

## Tech Stack

| Layer | Technologies |
|---|---|
| Hardware | ESP32, ACS712 current sensor, voltage sensing (divider / ZMPT101B), DHT22, relay, push buttons, LEDs |
| Firmware | C/C++ (Arduino framework), PlatformIO |
| Libraries | PubSubClient (MQTT), ArduinoJson |
| Edge AI | Pre-trained model exported as C code (`edge_ai.h`) |
| Communication | MQTT |
| Cloud | ThingsBoard (dashboards, rule chains, alarms, RPC) |
| Simulation | Wokwi (VS Code extension) |

## Installation & Setup

1. Install [Visual Studio Code](https://code.visualstudio.com/).
2. Install the **PlatformIO IDE** extension.
3. Install the **Wokwi Simulator** extension and activate its (free) license.
4. Clone the repository:
   ```bash
   git clone https://github.com/PrernaPachode/Edge-AI-EV-Charging-Station-Optimizer.git
   ```
5. Create a ThingsBoard account (cloud or Community Edition) and add one device per bay. Copy each device's access token.
6. In each bay's `config.h`, set the Wi-Fi settings, MQTT broker address and that bay's ThingsBoard device token.

## How to Run the Project

1. Open one bay folder (for example `BAY1/esp32_blink`) in VS Code.
2. Build the firmware with PlatformIO (**PlatformIO: Build**).
3. Start the simulation: press `F1` and choose **Wokwi: Start Simulator**.
4. Use the simulated buttons to plug an EV in or out and watch the bay change between FREE and CHARGING.
5. Raise the current to trigger a THROTTLE decision.
6. Open your ThingsBoard dashboard to see live telemetry, alarms and RPC controls.
7. Repeat for Bay2 and Bay3 (each bay uses its own device token).

## Project Structure

```
Edge-AI-EV-Charging-Station-Optimizer/
├── BAY1/esp32_blink/      # Firmware + Wokwi setup for Bay 1
├── Bay2/esp32_blink/      # Firmware + Wokwi setup for Bay 2
├── Bay3/esp32_blink/      # Firmware + Wokwi setup for Bay 3
└── README.md
```

Each bay folder follows the same modular layout:

## Screenshots

<img width="940" height="479" alt="image" src="https://github.com/user-attachments/assets/999635d8-f7fe-467c-9876-5368352a1b48" />
<img width="940" height="425" alt="image" src="https://github.com/user-attachments/assets/286e1c62-77c3-4a31-ba7a-26080f85ce61" />
<img width="940" height="413" alt="image" src="https://github.com/user-attachments/assets/46b39e3c-316c-4340-bb1b-9a2e58d77b38" />
<img width="940" height="423" alt="image" src="https://github.com/user-attachments/assets/68b7dad5-5ec0-457a-bb60-1d2e8554764a" />


| File | Purpose |
|---|---|
| `config.h` | Pins, thresholds and network settings |
| `State.cpp` | Bay state (FREE / CHARGING) and status handling |
| `Peripherals.cpp` | Sensor, relay, button and LED handling |
| `Telemetry.cpp` | Builds and publishes telemetry |
| `Network.h` | Wi-Fi and MQTT connection |
| `edge_ai.h` | Exported Edge AI model (runs on the ESP32) |
| `optimization.h` | ALLOW / THROTTLE / DEFER decision logic |
| `rpc.cpp` | Remote commands from ThingsBoard |
| `test/wokwi.toml`, `test/diagram.json` | Wokwi simulation configuration |

> The Edge AI model was trained offline and is included as the exported header `edge_ai.h`. No training script is included in this repository.


## Team Members

| Role | Name | Email |
|---|---|---|
| Team Leader | Arya Navrang | arya.navrang24@pccoepune.org |
| Team Member | Prerna Pachode | prerna.pachode24@pccoepune.org |

## Future Scope

- Upgrade to **TensorFlow Lite Micro** with a model trained on real charging data
- EV-owner app showing live bay availability and estimated wait time
- Over-the-air (OTA) model and firmware updates
- Live electricity tariff integration for cost-aware scheduling
