## 🧰 About 74LS244NA

* It’s an **octal buffer/line driver** with **tri-state outputs**.
* It has:

  * 8 inputs: `A0–A7`
  * 8 outputs: `Y0–Y7`
  * 2 enable pins: `G1` (controls Y0–Y3) and `G2` (controls Y4–Y7)
  * Active LOW enables → must be `LOW` to output.

| Pin   | Function                  |
| ----- | ------------------------- |
| 1     | G1 (Enable 1, active LOW) |
| 2–5   | A0–A3 (Inputs)            |
| 6–9   | Y0–Y3 (Outputs)           |
| 10    | GND                       |
| 11–14 | Y4–Y7 (Outputs)           |
| 15–18 | A4–A7 (Inputs)            |
| 19    | G2 (Enable 2, active LOW) |
| 20    | Vcc (+5V)                 |

---

## 🔌 Arduino Connections

| 74LS244       | Arduino Pin | Direction | Notes               |
| ------------- | ----------- | --------- | ------------------- |
| 1 (G1)        | 2           | OUTPUT    | LOW to enable Y0–Y3 |
| 19 (G2)       | 3           | OUTPUT    | LOW to enable Y4–Y7 |
| 2–5 (A0–A3)   | 4–7         | OUTPUT    | Test inputs 0–3     |
| 15–18 (A4–A7) | 8–11        | OUTPUT    | Test inputs 4–7     |
| 6–9 (Y0–Y3)   | A0–A3       | INPUT     | Read outputs 0–3    |
| 11–14 (Y4–Y7) | A4–A7       | INPUT     | Read outputs 4–7    |
| 10 (GND)      | GND         | —         | Ground              |
| 20 (Vcc)      | 5V          | —         | Power               |

--- 

### 📦 74LS244NA (Top View, Notch Up)

```
                +-----------------+
         G1  1 |•                20| Vcc (+5V)
         A0  2 |                 19| G2
         A1  3 |                 18| A7
         A2  4 |                 17| A6
         A3  5 |                 16| A5
         Y0  6 |                 15| A4
         Y1  7 |                 14| Y4
         Y2  8 |                 13| Y5
         Y3  9 |                 12| Y6
        GND 10 |_________________11| Y7
                +-----------------+
```

---

### 📘 Pin Function Quick Reference

| Pin   | Label | Description                   |
| ----- | ----- | ----------------------------- |
| 1     | G1    | Enable for Y0–Y3 (active LOW) |
| 2–5   | A0–A3 | Inputs for Y0–Y3              |
| 6–9   | Y0–Y3 | Buffered outputs for A0–A3    |
| 10    | GND   | Ground                        |
| 11–14 | Y7–Y4 | Buffered outputs for A7–A4    |
| 15–18 | A4–A7 | Inputs for Y4–Y7              |
| 19    | G2    | Enable for Y4–Y7 (active LOW) |
| 20    | Vcc   | +5 V power                    |

---

✅ **Orientation tip:** When placing the chip on a breadboard, the **notch or dot points up**. Pin 1 is at the **top-left** and pin 20 is at the **top-right**, then numbering goes **down the left side** and **up the right side**.

 

---

## 🧪 How to Use

1. Wire the circuit as per the table above.
2. Upload the code to your Arduino.
3. Open the **Serial Monitor** (`9600 baud`).
4. You’ll see a result for each test cycle:

   * ✅ “All channels OK!” → chip is good.
   * ❌ “FAIL at channel X” → possible bad pin or IC.

---

## 🧰 Tips

* Use a **5 V Arduino** (like Uno/Nano) — 74LS244 is TTL (not 3.3 V tolerant).
* If outputs always float, check:

  * Enable pins are LOW.
  * Power and GND connected.
* If *some* outputs fail, that buffer line might be damaged.

---

Would you like me to give you a **wiring diagram (schematic)** for this setup too? (I can generate one if you’d like.)
