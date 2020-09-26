# Just-In-Time compiler concept / demo for Intel x86_64 

A small bytecode Just-In-Time compiler / binary translator with a custom assember library and a virtual machine. The purpose of this project is to demonstrate the orders of magnitude faster execution speed of JIT-applied code paths against traditional virtual machines.

## Images
![terminal](https://raw.githubusercontent.com/fakelag/jit-demo/master/img/img0.PNG)

## Languages
* C

## Dependencies
* none

## Setup
* Clone or download the repository
* Compile Release x64 with MSVC
* Run `x64/Release/JitDemo.exe`

## Bytecode and instruction set

The bytecode VM supports 4 registers (r0 to r3) and 5 different instructions to program with. Programs made for the VM can also be compiled to x86_64 machine code with `jit_compile`, and then executed as native functions.

```c
// ADD r, r
// adds the second operand (register) the first operand (register), and stores the result in the first operand
#define INTERP_ADD		1

// SUB r, r
// subtracts the second operand from the first operand, and stores the result in the first operand
#define INTERP_SUB		2

// LOAD r, long
// loads a constant in the second operand to the first operand
#define INTERP_LOAD		3

// LOOP r, rel_addr_backwards
// Checks if the value in the first operand is 0, if not, will jump back the second operand amount of bytes relative to the instruction
#define INTERP_LOOP		4

// RET r
// Returns the value in the first operand
#define INTERP_RET		5
```

<br />
List of registers:

```c
#define INTERP_R0 0
#define INTERP_R1 1
#define INTERP_R2 2
#define INTERP_R3 3
```

## Programming

Programs can be written in `main.c`, executed in the VM and the native processor, and the execution time measured in seconds. Creating programs is done in bytecode.

## Disassembler

The repository also includes a disassembler in `bc_disassembler.c` to help visualize instructions written in bytecode.