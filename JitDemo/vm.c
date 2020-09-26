#include "stdio.h"
#include "vm.h"
#include "encoder.h"

long vm_interpret( const unsigned char* code ) {
	long r[] ={ 0, 0, 0, 0 };

	for ( const unsigned char* c = code; ; ++c ) {
		switch ( *c ) {
		case INTERP_ADD:
		{
			char a = *++c;
			char b = *++c;
			r[ a ] = r[ a ] + r[ b ];
			break;
		}
		case INTERP_SUB:
		{
			char a = *++c;
			char b = *++c;
			r[ a ] = r[ a ] - r[ b ];
			break;
		}
		case INTERP_LOAD:
		{
			char reg = *++c;
			++c;

			long value = decode_long( c[ 0 ], c[ 1 ], c[ 2 ], c[ 3 ] );
			r[ reg ] = value;

			c += 3;
			break;
		}
		case INTERP_LOOP:
		{
			char reg = *++c;

			if ( r[ reg ] != 0 ) {
				unsigned char offset = *++c;
				c -= offset + 3;
			}
			else {
				++c;
			}

			break;
		}
		case INTERP_RET:
		{
			return r[ *++c ];
		}
		default:
			printf( "vm_interpret: Unknown instruction %i\n", *c );
			break;
		}
	}
}
