#include "stdio.h"
#include "limits.h"
#include "vm.h"
#include "encoder.h"

char* reg_to_string( char virtual_register ) {
	switch ( virtual_register ) {
	case INTERP_R0: return "R0";
	case INTERP_R1: return "R1";
	case INTERP_R2: return "R2";
	case INTERP_R3: return "R3";
	default: {
		printf( "reg_to_string: Unknown virtual register %i\n", virtual_register );
		return "R0";
	}
	}
}

void print_bytecode( const unsigned char* code ) {
	for ( const unsigned char* c = code; ; ++c ) {
		long inst_offset = ( long ) ( c - code );

		switch ( *c ) {
		case INTERP_ADD:
		{
			char* a = reg_to_string( *++c );
			char* b = reg_to_string( *++c );
			printf( "%04d %19s%6s, %3s%s\n", inst_offset, "INTERP_ADD", a, "", b );
			break;
		}
		case INTERP_SUB:
		{
			char* a = reg_to_string( *++c );
			char* b = reg_to_string( *++c );
			printf( "%04d %19s%6s, %3s%s\n", inst_offset, "INTERP_SUB", a, "", b );
			break;
		}
		case INTERP_LOAD:
		{
			char* reg = reg_to_string( *++c );
			++c;

			long constant = decode_long( c[ 0 ], c[ 1 ], c[ 2 ], c[ 3 ] );

			printf( "%04d %20s%5s, %3s%d %s\n", inst_offset, "INTERP_LOAD", reg, "", constant, constant == INT_MAX ? "(constant, INT_MAX)" : "(constant)" );
			c += 3;
			break;
		}
		case INTERP_LOOP:
		{
			char* reg = reg_to_string( *++c );
			long loop_offset = ( long ) *++c;

			printf( "%04d %20s%5s, %3s%02x (-> %04d)\n", inst_offset, "INTERP_LOOP", reg, "", loop_offset, inst_offset - loop_offset );
			break;
		}
		case INTERP_RET:
		{
			char* reg = reg_to_string( *++c );
			printf( "%04d %19s%6s\n", inst_offset, "INTERP_RET", reg );
			return;
		}
		default:
			printf( "print_bytecode: Unknown instruction %i\n", *c );
			break;
		}
	}
}
