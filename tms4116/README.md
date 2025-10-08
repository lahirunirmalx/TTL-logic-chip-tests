## TMS4116 DRAM — Arduino Tester

Minimal Arduino UNO (5 V) test rig and sketch for 16,384×1 DRAMs (e.g., TI **TMS4116** / MK4116 / MCM4116).
**Important:** 4116-class DRAMs need **three power rails** and specific power-sequencing. Read the safety notes below before wiring. ([righto.com][1])

### What it does

* Writes/reads several bit patterns across all **16 384** addresses (128 rows × 128 columns).
* Reports any failing row/column and first failing bit.
* Runs safely with very relaxed timings (Arduino is much slower than the DRAM’s 150–300 ns spec).

---

## Pinout (top view, DIP-16)

```
           ┌─────────────────────────┐
   VBB  1 ─┤•                        ├─ 16  VSS (0V/GND)
    D   2 ─┤                         ├─ 15  CAS#
   WE#  3 ─┤   TMS4116 / 4116 DRAM   ├─ 14  Q  (Data Out)
   RAS# 4 ─┤                         ├─ 13  A6
    A0  5 ─┤                         ├─ 12  A5
    A2  6 ─┤                         ├─ 11  A4
    A1  7 ─┤                         ├─ 10  A3
   VDD  8 ─┤ (+12V)                  ├─  9  VCC (+5V)
           └─────────────────────────┘
```

Notes:

* **Separate D (in) and Q (out)** pins (not a shared DQ). Inputs are TTL-compatible; Q buffer is powered from **VCC (+5 V)**. 
* Address is multiplexed: A0–A6 carry **row** during **RAS#**, then **column** during **CAS#**. ([unige.ch][2])

---

## Safety & Power

4116 requires **+12 V (VDD)**, **+5 V (VCC)**, and **–5 V (VBB)**.

* **Sequencing (recommended):** Apply **VBB (–5 V) first**, then the positive rails; remove **VBB last**. Some datasheets explicitly call this out. ([minuszerodegrees.net][3])
* Decouple generously: 100 nF close to pins 8/9/16 and a few 10–47 µF bulk caps on +12 V and +5 V rails.
* Arduino pins are **5 V TTL**; connect only to logic pins (A0–A6, RAS#, CAS#, WE#, D, Q). **Never** expose the Arduino to ±12/–5 V rails.
* Use a bench supply (3 rails) or DC-DC modules for +12 V and –5 V. (An ATX PSU can supply +12 V and +5 V; you still need –5 V from a converter.) ([Wikipedia][4])

---

## Wiring (Arduino UNO example)

Suggested mapping (keeps contiguous ports and avoids conflicts). This follows a known-good hobby mapping for 4116 testers. ([dmcmillan.co.uk][5])

**Power (external):**

* Pin 1  VBB  → **–5 V**
* Pin 8  VDD  → **+12 V**
* Pin 9  VCC  → **+5 V**
* Pin 16 VSS  → **GND** (common ground with Arduino)

**Control/data:**

* Pin 4  RAS# → Arduino **A3**
* Pin 15 CAS# → Arduino **D9**
* Pin 3  WE#  → Arduino **A2**
* Pin 2  D    → Arduino **A1** (output from Arduino)
* Pin 14 Q    → Arduino **D8** (input to Arduino)

**Address (multiplexed A0–A6):**

* Pin 5  A0 → **A4**
* Pin 7  A1 → **D2**
* Pin 6  A2 → **A5**
* Pin 10 A3 → **D6**
* Pin 11 A4 → **D5**
* Pin 12 A5 → **D4**
* Pin 13 A6 → **D7**

> Keep leads short; add 100 Ω series resistors on control lines if you see ringing.

---

## Test patterns

1. **All-0** then verify
2. **All-1** then verify
3. **Checkerboard** (row⊕col) and inverse
4. **Address-based** (LSB of address)

The sketch purposely uses microsecond-scale delays (tRCD, tRAS, tCAS) so it’s very gentle on any speed grade.

---

## How the cycle works (basics)

**READ**

1. Put **row** (7 bits) on A0–A6 → **RAS# LOW**
2. Put **column** (7 bits) on A0–A6, **WE# HIGH** → **CAS# LOW**
3. Sample **Q**, then **CAS# HIGH**, **RAS# HIGH**

**WRITE**

1. Row on A0–A6 → **RAS# LOW**
2. Column on A0–A6, set **D** to bit value
3. **WE# LOW**, then **CAS# LOW** (early-write is fine at Arduino speeds)
4. **CAS# HIGH**, **WE# HIGH**, **RAS# HIGH**

(Addresses are refreshed whenever a row is opened with RAS#; the rapid test loop more than satisfies refresh.) ([unige.ch][2])

---
 

## Credits / References

* Texas Instruments TMS4116 datasheet (pinout, timing overview). 
* Intersil/MOSTEK 4116 notes — explicit **VBB sequencing** guidance. ([minuszerodegrees.net][3])
* Addressing and control-signal primer for 4116. ([unige.ch][2])
* Example Arduino pin mapping used by hobby testers. ([dmcmillan.co.uk][5])

---

 
---

## Quick checklist

* [ ] Common ground between Arduino and DRAM PSU
* [ ] –5 V present before +5/+12 V (and removed last) ([minuszerodegrees.net][3])
* [ ] Decouplers fitted right at the chip
* [ ] Q is **input** to Arduino, D is **output** from Arduino
* [ ] Short leads; watch CAS/RAS edges on a scope if available

---

If you want this packaged as an actual repo (with folders/files) just say the word and I’ll lay it out exactly how you prefer.

[1]: https://www.righto.com/2020/11/reverse-engineering-classic-mk4116-16.html?utm_source=chatgpt.com "Reverse-engineering the classic MK4116 16-kilobit DRAM ..."
[2]: https://www.unige.ch/medecine/nouspikel/ti99/mem32k.htm?utm_source=chatgpt.com "The 32K memory expansion card"
[3]: https://minuszerodegrees.net/memory/4116/datasheet_IM4116.pdf?utm_source=chatgpt.com "T"
[4]: https://en.wikipedia.org/wiki/ATX?utm_source=chatgpt.com "ATX"
[5]: https://www.dmcmillan.co.uk/blog/testing-4116-ram-ics?utm_source=chatgpt.com "Testing 4116 RAM ICs | Damian McMillan"
