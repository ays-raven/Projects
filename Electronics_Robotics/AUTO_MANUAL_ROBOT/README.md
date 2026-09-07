# AUTO/MANUAL Robot

An Arduino Nano-based robot with two operating modes:

* **AUTO** — autonomous maze navigation and obstacle avoidance
* **MANUAL** — Bluetooth-controlled movement

## Features

* Autonomous maze navigation
* 3× HC-SR04 ultrasonic sensors
* Bluetooth manual control via HC-05
* TCS3200 color detection
* 16×2 I2C LCD
* 4-wheel differential drive
* AUTO / MANUAL mode switching
* Safety timeout for manual control

## Hardware

* Arduino Nano
* HC-05 Bluetooth module
* 3× HC-SR04
* TCS3200 color sensor
* 2× L298N motor drivers
* 4× GA25-370 geared motors
* 16×2 I2C LCD
* 3× 18650 Li-ion cells
* LM2596 buck converter

## Modes

### AUTO

The robot uses its ultrasonic sensors to navigate the maze, avoid obstacles, correct its position near walls, and detect up to three colored markers.

### MANUAL

The robot can be controlled through Bluetooth:

| Command | Action      |
| ------- | ----------- |
| `F`     | Forward     |
| `B`     | Backward    |
| `L`     | Left        |
| `R`     | Right       |
| `S`     | Stop        |
| `A`     | Auto mode   |
| `M`     | Manual mode |
| `T`     | Toggle mode |

## Project Structure

```text
AUTO-MANUAL-Robot/
├── README.md
├── src/
│   └── robot.ino
├── docs/
│   ├── wiring.md
│   └── design.md
└── media/
	└── robot.jpg
        └── demo.mp4
```

## Documentation

* [Wiring & Pinout](docs/wiring.md)
* [Technical Design](docs/design.md)

---

**Arduino Nano • Embedded Systems • Robotics**
