#pragma once

#define INTERP_ADD		1	// ADD r, r
#define INTERP_SUB		2	// SUB r, r
#define INTERP_LOAD		3	// LOAD r, long
#define INTERP_LOOP		4	// LOOP <r if not zero>, rel_addr_backwards
#define INTERP_RET		5	// RET r

#define INTERP_R0 0
#define INTERP_R1 1
#define INTERP_R2 2
#define INTERP_R3 3

long vm_interpret( const unsigned char* code );
