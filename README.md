# 🏭 Smart Factory IoT Monitoring System
### Emertxe Internship Project — IoT Domain

## 📌 Project Overview

A two-node IoT monitoring system for a simulated smart factory environment, built as part of the **Emertxe IoT Internship Program**. The system collects real-time sensor data from a production line and a warehouse, transmits telemetry to **ThingsBoard Cloud** via **MQTT**, and enables remote monitoring, automated actuation, and alarm management through a live dashboard.

The hardware is fully simulated using **PicsimLab** (Arduino Uno), eliminating the need for physical components.

---

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    PicsimLab (Simulation)               │
│                                                         │
│  ┌──────────────────┐      ┌──────────────────┐         │
│  │  Node 1          │      │  Node 2          │         │
│  │  Production Line │      │  Warehouse       │         │
│  │  192.168.1.101   │      │  192.168.1.102   │         │
│  └────────┬─────────┘      └────────┬─────────┘         │
└───────────┼─────────────────────────┼───────────────────┘
            │   MQTT (Port 1883)      │
            ▼                         ▼
┌─────────────────────────────────────────────────────────┐
│           ThingsBoard Cloud (cloud.thingsboard.io)      │
│     Dashboards · Alarms · Rule Engine · RPC Control     │
└─────────────────────────────────────────────────────────┘
```

---

## 🔧 Nodes & Sensors

### Node 1 — Production Line (`192.168.1.101`)

| Sensor / Component | Pin | Description                           |
|--------------------|-----|---------------------------------------|
|DHT22               | D2  | Air temperature & humidity            |
| LM35               | A1  | Machine surface temperature           |
| Vibration (ADC)    | A0  | 4-sample averaged vibration reading   |
| Push Button        | D3  | Machine ON/OFF detection (active LOW) |
| Relay              | D4  | Production relay control              |
| LED Green          | D5  | Normal / connected                    |
| LED Yellow         | D6  | Warning / sensor error                |
| LED Red            | D7  | Fault / critical                      |

**Telemetry keys published:** `temperature`, `humidity`, `lm35Temp`, `vibration`, `machineStatus`, `relayState`, `sensorError`, `uptime`

---

### Node 2 — Warehouse (`192.168.1.102`)

| Sensor / Component | Pin | Description                        |
|--------------------|-----|------------------------------------|
| DHT22              | D2  | Air temperature & humidity         |
| PIR HC-SR501       | D3  | Motion detection (10s hold latch)  |
| Ventilation Relay  | D4  | Auto-activates when humidity ≥ 85% |
| LED Green          | D5  | Connected & normal                 |
| LED Yellow         | D6  | Warning / dark warehouse           |
| Reed Switch        | D7  | Door open/close detection          |
| LDR                | A0  | Ambient light (isDark when < 300)  |

**Telemetry keys published:** `temperature`, `humidity`, `ldrValue`, `isDark`, `motionDetected`, `doorOpen`, `relayState`, `sensorError`, `uptime`

---

## 💡 Key Features

- 📡 **MQTT Telemetry** — Both nodes publish all sensor data to ThingsBoard every 5 seconds
- 🌀 **Auto-Ventilation** — Warehouse relay activates automatically when humidity ≥ 85%
- 🕹️ **Remote RPC Control** — `setRelay`, `setLED`, `getStatus` commands from ThingsBoard dashboard
- 🔁 **Auto-Reconnect** — MQTT reconnection within 30 seconds of disconnection
- 🛡️ **Sensor Fault Handling** — DHT22 error caching prevents false alarms
- 🚨 **Alarm Rule Engine** — ThingsBoard raises `HIGH_TEMPERATURE` CRITICAL alarm when temp > 35°C
- 🧩 **Modular Code Architecture** — Clean separation across `SensorManager`, `ActuatorManager`, `NetworkManager`, `TelemetryManager`, `RPCHandler`

---

## 🗂️ Project Structure

```
emertxe-iot-internship/
│
├── node1_production/
│   ├── node1_production.ino      # Main sketch (setup + loop)
│   ├── config.h                  # Pins, thresholds, MQTT config (⚠️ add token here)
│   ├── SensorManager.h/.cpp      # DHT22, LM35, vibration ADC reads
│   ├── ActuatorManager.h/.cpp    # Relay & LED control
│   ├── NetworkManager.h/.cpp     # Ethernet init, MQTT connect/reconnect
│   ├── TelemetryManager.h/.cpp   # JSON payload builder & publisher
│   └── RPCHandler.h/.cpp         # Incoming RPC parser & dispatcher
│
├── node2_warehouse/
│   ├── node2_warehouse.ino
│   ├── config.h
│   ├── SensorManager.h/.cpp      # DHT22, PIR, LDR, reed switch
│   ├── ActuatorManager.h/.cpp
│   ├── NetworkManager.h/.cpp
│   ├── TelemetryManager.h/.cpp
│   └── RPCHandler.h/.cpp
│
├── docs/
│   └── Smart_Factory_Requirements_Design_v1.0.pdf
│
└── README.md
```

---

## ⚙️ Setup & Getting Started

### Prerequisites
- [Arduino IDE](https://www.arduino.cc/en/software)
- [PicsimLab](https://github.com/lcgamboa/picsimlab) simulator
- [ThingsBoard Cloud](https://cloud.thingsboard.io) account (free)
- Arduino Libraries: `PubSubClient`, `DHT sensor library`, `Ethernet`

### Step 1 — Increase MQTT Buffer Size
In `Arduino/libraries/PubSubClient/src/PubSubClient.h`, change:
```cpp
#define MQTT_MAX_PACKET_SIZE 128
// → change to:
#define MQTT_MAX_PACKET_SIZE 256
```
Restart Arduino IDE after saving.

### Step 2 — ThingsBoard Device Setup
1. Go to [cloud.thingsboard.io](https://cloud.thingsboard.io)
2. **Entities → Devices → Add Device**
3. Create `Production Line Monitor` and `Warehouse Monitor`
4. For each device: **Manage Credentials → copy the Access Token**

### Step 3 — Configure Each Node
Paste your token into `config.h`:
```cpp
#define TB_SERVER      "mqtt.thingsboard.cloud"
#define ACCESS_TOKEN   "YOUR_DEVICE_TOKEN_HERE"   // ← paste token here
```

> ⚠️ **Never commit your real access token to GitHub.** Use `YOUR_TOKEN_HERE` as a placeholder in the repo.

### Step 4 — Compile & Load into PicsimLab
1. Open the `.ino` sketch in Arduino IDE
2. **Sketch → Export Compiled Binary** → generates `.hex` file
3. In PicsimLab: **File → Load Hex** → select the `.hex` file
4. Click **Play** to start simulation
5. Open PicsimLab Serial Terminal — confirm `[INIT] Ready` and IP address

### Step 5 — Verify on ThingsBoard
- Go to your device → **Latest Telemetry** tab
- All keys should appear within 10 seconds of simulation start

---

## 📊 ThingsBoard Dashboards

| Dashboard              | Description                                                               |
|------------------------|---------------------------------------------------------------------------|
| Smart Factory Overview | Unified view of both nodes — temp trend, alarm table                      |
| Node 1 Detail          | Gauges for temp/humidity/vibration, relay & LED indicators, RPC switch    |
| Node 2 Detail          | Humidity gauge, motion/door/light indicators, ventilation override switch |

---

## 🚨 Alarm Thresholds

| Parameter            | Warning     | Critical | Action                       |
|----------------------|-------------|----------|------------------------------|
| Air Temperature      | > 30°C      | > 35°C   | CRITICAL alarm + RED LED     |
| Humidity (Warehouse) | > 70%       | > 85%    | Auto-ventilation relay       |
| Machine Temp (LM35)  | > 30°C      | > 40°C   | Operator visual alert        |
| Vibration            | > 700 ADC   | —        | Chart threshold line         |
| MQTT Disconnect      | 5s          | 30s      | RED LED + auto-reconnect     |
| DHT22 Sensor Error   | Any failure | —        | YELLOW blink + cached values |

---

## 🛠️ Tech Stack

| Tool                 | Purpose                                                               |
|----------------------|-----------------------------------------------------------------------|
| Arduino IDE          | Code development, library management, `.hex` compilation              |
| PicsimLab            | Full Arduino Uno simulation — sensors, relays, LEDs, virtual Ethernet |
| ThingsBoard Cloud    | IoT platform — MQTT broker, dashboards, alarms, RPC, rule engine      |
| MQTT (PubSubClient)  | Lightweight publish/subscribe protocol for telemetry                  |
| C++                  | Firmware language for all Arduino modules                             |

---

## 📅 Internship Progress

| # | Task                                                   | Status |
|---|--------------------------------------------------------|---|
| 1 | Assignment 1 — Blinking LED demo                       | ✅ Submitted |
| 2 | Assignment 2 — DHT22 sensor operation                  | ✅ Submitted |
| 3 | Assignment 3 — ThingsBoard latest telemetry screenshot | ✅ Submitted |
| 4 | Weekend Quiz 1                                         | ✅ 9/10 |
| 5 | Weekend Quiz 2                                         | ✅ 9/10 |
| 6 | Smart Factory — Node 1 (Production Line) coding        | 🔄 In Progress |
| 7 | Smart Factory — Node 2 (Warehouse) coding              | ⏳ Upcoming |
| 8 | ThingsBoard dashboard setup                            | ⏳ Upcoming |
| 9 | Full system testing & acceptance criteria              | ⏳ Upcoming |

---

## 👤 Author

**Emertxe Internship — IoT Domain**
Batch: 2026 | Platform: PicsimLab + ThingsBoard Cloud

---

*Requirements & Design Document v1.0 — Smart Factory IoT Monitoring System — Team Emertxe — February 2026*
