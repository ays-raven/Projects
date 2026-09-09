# Arduino Nano Robot — Wiring & Pinout

## Arduino Nano

| Pin | Connection                   |
| --- | ---------------------------- |
| D0  | HC-05 TX                     |
| D1  | HC-05 RX via voltage divider |
| D2  | Left TRIG                    |
| D4  | Front TRIG                   |
| D5  | L298N ENA                    |
| D6  | L298N ENB                    |
| D7  | L298N IN1                    |
| D8  | L298N IN2                    |
| D9  | Right TRIG                   |
| D10 | Front ECHO                   |
| D11 | Right ECHO                   |
| D12 | L298N IN3                    |
| D13 | L298N IN4                    |
| A0  | TCS3200 S2                   |
| A1  | TCS3200 S3                   |
| A2  | TCS3200 OUT                  |
| A3  | Left ECHO                    |
| A4  | LCD SDA                      |
| A5  | LCD SCL                      |

## HC-05

| HC-05 | Connection                  |
| ----- | --------------------------- |
| TX    | Nano D0                     |
| RX    | Nano D1 via voltage divider |
| VCC   | 5V                          |
| GND   | Common GND                  |

`9600 baud`

## HC-SR04

| Sensor | TRIG | ECHO |
| ------ | ---- | ---- |
| Left   | D2   | A3   |
| Front  | D4   | D10  |
| Right  | D9   | D11  |

VCC → 5V
GND → Common GND

## L298N

| L298N | Nano       |
| ----- | ---------- |
| ENA   | D5         |
| IN1   | D7         |
| IN2   | D8         |
| ENB   | D6         |
| IN3   | D12        |
| IN4   | D13        |
| GND   | Common GND |

Motor supply → Battery

## TCS3200

| TCS3200 | Nano       |
| ------- | ---------- |
| S2      | A0         |
| S3      | A1         |
| OUT     | A2         |
| VCC     | 5V         |
| GND     | Common GND |

## I2C LCD

| LCD | Nano       |
| --- | ---------- |
| SDA | A4         |
| SCL | A5         |
| VCC | 5V         |
| GND | Common GND |

Address: `0x27`

## Power

```text
Battery + → Master Switch → L298N
                         → LM2596 IN+

Battery - ───────────────→ LM2596 IN-

LM2596 OUT+ → 5V Rail
LM2596 OUT- → Common GND
```

**All components share Common GND.**

