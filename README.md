# Autonomous Navigation Robot – ESP32 + React + Node.js

A smart autonomous robot car that navigates to selected destinations within a mapped environment, using real-time sensor data and Wi-Fi communication.

---

## Project Overview

- Autonomous robot car powered by **ESP32**
- Navigation based on map and selected classroom
- Initial learning phase to scan and store the environment
- Bidirectional path execution (go and return)
- Real-time communication with a **React** interface

---

## Hardware Components

- **ESP32 DOIT DEVKIT V1**
- **DC Motors** with motor driver and encoders
- **3x Ultrasonic sensors** (HC-SR04)
- **TF Mini LiDAR** sensor (UART1 – RX: GPIO 16, TX: GPIO 17)

---

## Software Architecture

### Frontend (React)
- Interactive map UI to select destination
- Displays route and live movement updates
- Connects via WebSocket to backend
- WebSerial used for direct communication option

### Backend (Node.js)
- WebSocket server to bridge React <-> ESP32
- TCP socket connection with the robot
- Receives status updates and relays them to UI
- Sends command sequence to ESP32

### Firmware (ESP32)
- Connects to Wi-Fi and TCP server
- Parses and executes path instructions
- Waits after reaching destination, then returns
- Sends real-time updates (distance, status, sensor data)

---

## How Navigation Works

1. User selects destination in React UI
2. Server calculates and sends path to ESP32
3. Robot moves step-by-step using sensor feedback
4. Status updates sent back to React for live display
5. Upon completion, robot waits, then returns on reverse path

---

## Running the System

### ESP32
- Flash firmware via Arduino IDE or PlatformIO
- Ensure Wi-Fi credentials and TCP server IP are set

### Server
```bash
cd server
npm install
node index.js
```

### Client
```bash
cd client
npm install
npm start
```

---

## Communication

- **React ↔️ Node.js**: WebSocket (`socket.io`)
- **Node.js ↔️ ESP32**: TCP Socket
- **Sensors ↔️ ESP32**: UART + GPIO

---

## Author

**Efrat Sharet**
