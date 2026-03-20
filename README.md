# 8-bit ALU Simulator

A software simulation of an 8-bit ALU built from the ground up in C++, starting from a single NAND gate. All logic gates and arithmetic components are implemented manually — no bitwise operators used.

---

## Features

- All logic gates derived from **NAND**: NOT, AND, OR, NOR, XOR
- **Half adder** and **Full adder** (1-bit)
- **1-bit ALU** with enable controls and operation multiplexer
- **8-bit ALU** with a 3-to-8 decoder for operation selection
- **8-bit barrel shifter** (left/right shift by 1)
- **Equality check** (A == B)
- **Addition** (A + B) with carry flag
- **Subtraction** (A - B) via two's complement with carry flag
- Multiplication opcode slot (`111`) exists but is **not yet implemented**

---

## Build

Standard C++, no external dependencies. Compile with any C++ compiler:

```bash
g++ -o alu main.cpp
./alu
```

---

## Operation Select (D[3] decoder input)

| D[2] D[1] D[0] | Operation       | Notes                        |
|----------------|-----------------|------------------------------|
| 0 0 0          | —               | No operation                 |
| 0 0 1          | A == B          | Sets F flag if equal         |
| 0 1 0          | —               | No operation                 |
| 0 1 1          | A - B           | Two's complement subtraction |
| 1 0 0          | —               | No operation                 |
| 1 0 1          | Shift           | `Cntr=0` → right, `Cntr=1` → left |
| 1 1 0          | A + B           | Sets carry flag              |
| 1 1 1          | A * B           | Not implemented              |

---

## Input / Output

All values are 8-element `bool` arrays representing bits **MSB-first** (index 0 = most significant bit).

| Variable | Description                        |
|----------|------------------------------------|
| `A[8]`   | First operand                      |
| `B[8]`   | Second operand                     |
| `D[3]`   | Operation selector (3-bit)         |
| `Cntr`   | Shift direction (0=right, 1=left)  |
| `Out[8]` | Result                             |
| `Cflag`  | Carry out flag                     |
| `F`      | Equality flag (1 if A == B)        |

---
- The `AND` of `B[i]` with each bit of the shifted A (gating the addend) is already handled by the `ENA`/`ENB` inputs on `ALU1bit` — you could feed `B[i]` as `ENA` to zero out the addend when the bit is 0, instead of using an `if` branch.
