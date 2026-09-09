# Arduino Nano Robot — Wiring & Pinout

## Arduino Nano Pinout

| Nano Pin | Function   | Connected To  |
| -------- | ---------- | ------------- |
| D2       | Left TRIG  | HC-SR04 Left  |
| D4       | Front TRIG | HC-SR04 Front |
| D5       | ENA (PWM)  | L298N         |
| D6       | ENB (PWM)  | L298N         |
| D7       | IN1        | L298N         |
| D8       | IN2        | L298N         |
| D9       | Right TRIG | HC-SR04 Right |
| D10      | Front ECHO | HC-SR04 Front |
| D11      | Right ECHO | HC-SR04 Right |
| D12      | IN3        | L298N         |
| D13      | IN4        | L298N         |
| A0       | S2         | TCS3200       |
| A1       | S3         | TCS3200       |
| A2       | OUT        | TCS3200       |
| A3       | Left ECHO  | HC-SR04 Left  |
| A4       | SDA        | I2C LCD       |
| A5       | SCL        | I2C LCD       |

---

## HC-SR04 ×3

### Front

| Sensor | Nano       |
| ------ | ---------- |
| TRIG   | D4         |
| ECHO   | D10        |
| VCC    | 5V         |
| GND    | Common GND |

### Right

| Sensor | Nano       |
| ------ | ---------- |
| TRIG   | D9         |
| ECHO   | D11        |
| VCC    | 5V         |
| GND    | Common GND |

### Left

| Sensor | Nano       |
| ------ | ---------- |
| TRIG   | D2         |
| ECHO   | A3         |
| VCC    | 5V         |
| GND    | Common GND |

---

## L298N Motor Drivers

### Control

| Nano | L298N |
| ---- | ----- |
| D5   | ENA   |
| D7   | IN1   |
| D8   | IN2   |
| D6   | ENB   |
| D12  | IN3   |
| D13  | IN4   |
| GND  | GND   |


---

## LM2596 Buck Converter

```text
Battery + → Switch → IN+
Battery - ─────────→ IN-

OUT+ → 5V Rail
OUT- → Common GND
```

5V rail → Arduino Nano, HC-SR04 ×3, TCS3200, LCD.

---

## TCS3200

| TCS3200 | Nano       |
| ------- | ---------- |
| S2      | A0         |
| S3      | A1         |
| OUT     | A2         |
| VCC     | 5V         |
| GND     | Common GND |

---

## 16×2 I2C LCD

| LCD | Nano       |
| --- | ---------- |
| SDA | A4         |
| SCL | A5         |
| VCC | 5V         |
| GND | Common GND |

I2C address: `0x27`

---

## Power & Ground

```text
Battery
  │
  ├── (+) → Master Switch → L298N
  │                       → LM2596 → 5V Rail
  │
  └── (-) ──────────────── Common GND
```

**All components share the same GND.**
