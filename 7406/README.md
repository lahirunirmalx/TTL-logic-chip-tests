## 🧰 About 7406 (Hex Inverter, Open-Collector)

* **6 inverting buffers (NOT gates)**
* **Open-collector outputs** → They **sink current only** (can pull LOW but not drive HIGH)
* Each output needs a **pull-up resistor** (e.g., 4.7 kΩ–10 kΩ) to +5 V to read logic HIGH.

---

### 📦 7406 Pinout (Top View)

```
               +-----------------+
        1A  1 |•                14| Vcc (+5V)
        1Y  2 |                 13| 6A
        2A  3 |                 12| 6Y
        2Y  4 |                 11| 5A
        3A  5 |                 10| 5Y
        3Y  6 |                  9| 4A
       GND  7 |__________________8| 4Y
               +-----------------+
```

| Pin | Label | Description               |
| --- | ----- | ------------------------- |
| 1   | 1A    | Input 1                   |
| 2   | 1Y    | Output 1 (open collector) |
| 3   | 2A    | Input 2                   |
| 4   | 2Y    | Output 2                  |
| 5   | 3A    | Input 3                   |
| 6   | 3Y    | Output 3                  |
| 7   | GND   | Ground                    |
| 8   | 4Y    | Output 4                  |
| 9   | 4A    | Input 4                   |
| 10  | 5Y    | Output 5                  |
| 11  | 5A    | Input 5                   |
| 12  | 6Y    | Output 6                  |
| 13  | 6A    | Input 6                   |
| 14  | Vcc   | +5 V Power                |

---

## 🔌 Wiring 7406 to Arduino

| 7406 Pin | Function | Arduino Pin | Direction | Notes                                   |
| -------- | -------- | ----------- | --------- | --------------------------------------- |
| 14       | Vcc      | 5V          | —         | Power                                   |
| 7        | GND      | GND         | —         | Ground                                  |
| 1 (1A)   | Input 1  | D4          | OUTPUT    | Arduino drives input                    |
| 2 (1Y)   | Output 1 | D8          | INPUT     | Must use pull-up resistor (e.g., 10 kΩ) |
| 3 (2A)   | Input 2  | D5          | OUTPUT    |                                         |
| 4 (2Y)   | Output 2 | D9          | INPUT     | With pull-up resistor                   |
| 5 (3A)   | Input 3  | D6          | OUTPUT    |                                         |
| 6 (3Y)   | Output 3 | D10         | INPUT     | With pull-up resistor                   |
| 9 (4A)   | Input 4  | D7          | OUTPUT    |                                         |
| 8 (4Y)   | Output 4 | D11         | INPUT     | With pull-up resistor                   |
| 11 (5A)  | Input 5  | (optional)  | OUTPUT    |                                         |
| 10 (5Y)  | Output 5 | (optional)  | INPUT     | With pull-up resistor                   |
| 13 (6A)  | Input 6  | (optional)  | OUTPUT    |                                         |
| 12 (6Y)  | Output 6 | (optional)  | INPUT     | With pull-up resistor                   |

⚠️ **Important:** For each output, connect a **10 kΩ resistor from the output pin to +5 V**.
This is necessary because the 7406 output transistor only pulls LOW — without the resistor, the pin will “float.”

---

## ✅ Arduino Test Code for 7406

```

---

## 🧪 Usage

1. Place the **7406** on a breadboard (notch up).
2. Wire according to the table above.
3. Add **10 kΩ pull-up resistors** from each output pin to `+5V`.
4. Upload the code.
5. Open the **Serial Monitor (9600 baud)** to see the results.

---

## 📊 ASCII Pinout Quick Reference (7406 DIP)

```
               +-----------------+
  1A   D4  1 |•                14| Vcc (+5V)
  1Y   D8  2 |                 13| 6A
  2A   D5  3 |                 12| 6Y
  2Y   D9  4 |                 11| 5A
  3A   D6  5 |                 10| 5Y
  3Y  D10  6 |                  9| 4A (D7)
  GND       7 |__________________8| 4Y (D11)
               +-----------------+
```

📌 **Remember:** Outputs are **inverted** and **open-collector**, so you *must* use pull-up resistors.
