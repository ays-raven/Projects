# Astable Multivibrator - Calculations

## Power Supply

The circuit is powered using a **220 V AC → 5 V DC power supply**.

```text
220 V AC
   ↓
Power Supply
   ↓
5 V DC
```

Therefore:

```text
VCC = 5 V
```

---

## Collector Resistors (RC)

The collector resistors limit the collector current and provide the
voltage drop when the transistors are ON.

### Given

```text
VCC = 5 V
VCE(sat) = 0.2 V
IC = 4.8 mA
```

### Voltage across the collector resistor

```text
VR = VCC - VCE(sat)

VR = 5 - 0.2

VR = 4.8 V
```

### Collector resistor

```text
RC = VR / IC

RC = 4.8 / 4.8 mA

RC = 1 kΩ
```

Therefore:

```text
RC1 = RC2 = 1 kΩ
```

---

## Timing Resistors and Capacitors

For a symmetrical astable multivibrator:

```text
T = 1.386 × R × C
```

Using:

```text
R = 47 kΩ
C = 10 μF
```

### Period

```text
T = 1.386 × 47000 × 10 × 10⁻⁶

T = 0.65 s
```

### Frequency

```text
f = 1 / T

f = 1 / 0.65

f = 1.5 Hz
```

So the circuit changes state approximately **1.5 times per second**.

---

## Potentiometers (VR1 & VR2)

The potentiometers are connected in series with the timing resistors
to make the blinking frequency adjustable.

The total timing resistance is:

```text
Rtotal = 47 kΩ + Rpot
```

Since:

```text
T = 1.386 × R × C
```

### Increasing the resistance

```text
R ↑
↓
T ↑
↓
f ↓
↓
Blinking becomes slower
```

### Decreasing the resistance

```text
R ↓
↓
T ↓
↓
f ↑
↓
Blinking becomes faster
```

---

## Final Values

| Component            |          Value |
| -------------------- | -------------: |
| Power Supply         |         5 V DC |
| Q1, Q2               |         2N2222 |
| RC1, RC2             |           1 kΩ |
| R1, R2               |          47 kΩ |
| C1, C2               |          10 μF |
| VR1, VR2             | Potentiometers |
| Calculated Period    |       ≈ 0.65 s |
| Calculated Frequency |       ≈ 1.5 Hz |
