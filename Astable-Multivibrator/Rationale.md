# How the Circuit Works

## The Initial State

Now here's the interesting part.

At first, I actually thought both transistors would turn ON together.
I mean... both capacitors start charging at the same time, right?

Well... not really.

In real life, no two electronic components are exactly identical.
One transistor will always turn ON just a tiny fraction of a second
before the other.

Let's assume Q1 wins the first round.

The moment Q1 turns ON, its collector voltage drops from about 5 V
to nearly 0.2 V.

That sudden voltage drop passes through capacitor C1 and pulls the
base voltage of Q2 down, making it even harder for Q2 to turn ON.

Since Q2 is still OFF, its collector stays close to 5 V.

Now here's the trick...

The capacitor transfers the voltage change, giving Q1 one more push
to stay ON.

AKA... Positive Feedback.

Instead of fixing the tiny difference between the two transistors,
the circuit actually amplifies it until:

Q1 → Fully ON
Q2 → Fully OFF

At this point, the fast switching is over.

Now nothing exciting happens for a moment.

The capacitor slowly charges through the 47 kΩ timing resistor from
the 5 V supply.

This charging process is basically the "waiting time" of the circuit.

When the base voltage of Q2 finally reaches about 0.7 V, Q2 starts
conducting.

And ..........

Everything happens in reverse.

Q2's collector suddenly drops.

That drop is transferred through the capacitor to Q1's base, pulling
it down.

Q1 starts turning OFF.

As Q1 turns OFF, its collector rises back toward 5 V.

That rising voltage pushes Q2's base even higher.

Again....

Positive Feedback takes over.

Within a very short time:

Q2 → Fully ON
Q1 → Fully OFF

Now you literally repeat the exact same story, but swap Q1 and Q2.

And that's how the circuit keeps oscillating forever.

---

## Potentiometers (VR1 & VR2)

To make the blinking frequency adjustable, a potentiometer is
connected in series with each timing resistor.

Changing the potentiometer changes the total resistance of the
RC network.

Since:

T = 1.386 × R × C

### Increasing the resistance

Charging takes longer →

Blinking becomes slower →

### Decreasing the resistance

Charging becomes faster →

Blinking becomes faster →

So instead of changing capacitors every time, we can simply rotate
the potentiometer and adjust the blinking speed.
