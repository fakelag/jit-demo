#pragma once

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

#define INTERP_R0 0
#define INTERP_R1 1
#define INTERP_R2 2
#define INTERP_R3 3

long vm_interpret( const unsigned char* code );
