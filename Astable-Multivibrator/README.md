# Astable Multivibrator

## Overview

This is a transistor-based astable multivibrator circuit.

The circuit continuously switches between two states, producing an oscillating output.

## Power Supply

The circuit is powered using a **220V AC → 5V DC power supply**.

```text
220V AC
   ↓
Power Supply
   ↓
  5V DC
   ↓
Astable Multivibrator
```

## Components

* 2 × 2N2222 Transistors
* 2 × 10 µF Capacitors
* 2 × 47 kΩ Resistors
* LEDs
* 5V DC Power Supply
* Breadboard
* Jumper Wires

## How It Works

The two transistors continuously switch ON and OFF.

When one transistor turns ON, the other turns OFF. The capacitors charge and discharge through the resistors, causing the circuit to switch states repeatedly.

This produces the oscillating behavior of the astable multivibrator.

## Calculations

The component values and timing calculations are available in:

`Calculations/`

## Proteus Simulation

The circuit was simulated in Proteus before the practical implementation.

Simulation files and screenshots are available in:

`Proteus-Simulation/`

## Practical Implementation

The circuit was built and tested using a **5V DC power supply**.

## Video

A demonstration of the circuit is available in:

`Video/`

# Explanation

My Explanation of how the mechanism of the circuit works.

`EXPLAIN.md`

## What I Learned

* How a transistor can be used as a switch.
* How capacitors charge and discharge in a timing circuit.
* How two transistors can be used to create continuous oscillation.
* How component values affect the switching frequency.
* How to design, simulate, and build a circuit practically.
