#include "limits.h"
#include "assembler.h"
#include "stdio.h"
#include "time.h"

#include "encoder.h"
#include "jit.h"
#include "vm.h"
#include "bc_disassembler.h"

void profile_jit( const unsigned char* code ) {
#ifdef _WIN64
	clock_t start = clock();

	fn jit_function = jit_compile( code );
	long return_value = jit_function();

	clock_t end = clock();

	double elapsed_time = ( double ) ( end - start ) / CLOCKS_PER_SEC;
	printf( "JIT returned %i and took %.2f seconds\n", return_value, elapsed_time );

	free_fn( jit_function );
#endif
}

void profile_interpreter( const unsigned char* code ) {
	clock_t start = clock();

	long return_value = vm_interpret( code );

	clock_t end = clock();

	double elapsed_time = ( double ) ( end - start ) / CLOCKS_PER_SEC;
	printf( "Interpreter returned %i and took %.2f seconds\n", return_value, elapsed_time );
}

int main() {
	long a = INT_MAX;
	long b = 1;

	const unsigned char code[] ={
		INTERP_LOAD,
		INTERP_R0,
		encode_long( a, 0 ),
		encode_long( a, 1 ),
		encode_long( a, 2 ),
		encode_long( a, 3 ),

		INTERP_LOAD,
		INTERP_R1,
		encode_long( b, 0 ),
		encode_long( b, 1 ),
		encode_long( b, 2 ),
		encode_long( b, 3 ),

		INTERP_SUB,
		INTERP_R0,
		INTERP_R1,

		INTERP_ADD,
		INTERP_R2,
		INTERP_R1,

		INTERP_LOOP,
		INTERP_R0,
		6,

		INTERP_RET,
		INTERP_R2
	};

	printf( "Loaded program (bytecode):\n" );
	print_bytecode( code );

	printf( "Executing...\n\n" );
	profile_jit( code );
	profile_interpreter( code );

	getchar();
	return 0;
}
