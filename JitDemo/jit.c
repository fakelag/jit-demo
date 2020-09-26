#include "windows.h"
#include "stdio.h"
#include "stdlib.h"
#include "jit.h"
#include "vm.h"
#include "assembler.h"

fn alloc_fn() {
	unsigned char* memory = ( unsigned char* ) VirtualAllocEx( ( HANDLE ) -1, NULL, 4098, MEM_COMMIT, PAGE_EXECUTE_READWRITE );

	if ( memory == NULL ) {
		printf( "Executable memory allocation failed\n" );
		return NULL;
	}

	return ( fn ) memory;
}

void free_fn( fn f ) {
	VirtualFree( f, 0, MEM_RELEASE );
}

char jit_register_for_virtual( char virtual_register ) {
	switch ( virtual_register ) {
	case INTERP_R0: return REG_RAX;
	case INTERP_R1: return REG_RCX;
	case INTERP_R2: return REG_RDX;
	case INTERP_R3: return REG_RBX;
	default: {
		printf( "jit_register_for_virtual: Unknown virtual register %i\n", virtual_register );
		return REG_RAX;
	}
	}
}

int jit_is_volatile_register( char reg ) {
	return reg == REG_RAX || reg == REG_RCX || reg == REG_RDX;
}

unsigned long jit_backup_registers( const unsigned char* code ) {
	unsigned long backup_register_bits = 0;
	for ( const unsigned char* c = code; *c != INTERP_RET; ++c ) {
		switch ( *c ) {
		case INTERP_ADD:
		{
			char reg1 = jit_register_for_virtual( *++c );
			char reg2 = jit_register_for_virtual( *++c );

			if ( !jit_is_volatile_register( reg1 ) ) backup_register_bits |= ( 1 << reg1 );
			if ( !jit_is_volatile_register( reg2 ) ) backup_register_bits |= ( 1 << reg2 );

			break;
		}
		case INTERP_SUB:
		{
			char reg2 = jit_register_for_virtual( *++c );
			char reg1 = jit_register_for_virtual( *++c );

			if ( !jit_is_volatile_register( reg1 ) ) backup_register_bits |= ( 1 << reg1 );
			if ( !jit_is_volatile_register( reg2 ) ) backup_register_bits |= ( 1 << reg2 );

			break;
		}
		case INTERP_LOAD:
		{
			char reg = jit_register_for_virtual( *++c );

			if ( !jit_is_volatile_register( reg ) ) backup_register_bits |= ( 1 << reg );

			break;
		}
		case INTERP_LOOP:
		case INTERP_RET:
			break;
		}
	}

	return backup_register_bits;
}

long jit_code_size( const unsigned char* code ) {
	long code_size = 0;

	for ( const unsigned char* c = code; *c != INTERP_RET; ++c ) {
		code_size++;
	}

	return code_size;
}

fn jit_compile( const unsigned char* code ) {
	fn f = alloc_fn();

	struct asm_t context;
	asm_begin( &context, ( unsigned char* ) f );

	long* inst_count = malloc( jit_code_size( code ) * sizeof( long ) );
	inst_count[ 0 ] = 0;

	unsigned long backup_register_bits = jit_backup_registers( code );

	for ( char reg = REG_FIRST; reg < REG_LAST; ++reg ) {
		if ( backup_register_bits & ( 1 << reg ) )
			asm_push_reg( &context, reg );
	}

	// Clear registers
	asm_mov_reg_const32( &context, REG_RAX, 0 );
	asm_mov_reg_const32( &context, REG_RCX, 0 );
	asm_mov_reg_const32( &context, REG_RDX, 0 );

	if ( backup_register_bits & ( 1 << REG_RBX ) )
		asm_mov_reg_const32( &context, REG_RBX, 0 );

	for ( const unsigned char* c = code; ; ++c ) {
		switch ( *c ) {
		case INTERP_ADD:
		{
			char a = jit_register_for_virtual( *++c );
			char b = jit_register_for_virtual( *++c );
			asm_add_reg_reg( &context, a, b );
			break;
		}
		case INTERP_SUB:
		{
			char a = jit_register_for_virtual( *++c );
			char b = jit_register_for_virtual( *++c );
			asm_sub_reg_reg( &context, a, b );
			break;
		}
		case INTERP_LOAD:
		{
			char reg = jit_register_for_virtual( *++c );
			long* value = ( long* ) ++c;

			asm_mov_reg_const32( &context, reg, *value );
			c += 3;
			break;
		}
		case INTERP_LOOP:
		{
			char reg = jit_register_for_virtual( *++c );
			char rel8 = *++c;

			uintptr_t inst_count_offset = ( uintptr_t ) c - ( uintptr_t ) code - 3 - rel8;

			long target_inst_count = inst_count[ inst_count_offset ];

			asm_cmp_reg_const8( &context, reg, 0 );
			asm_je_rel8( &context, 2 );

			long current_offset = ( long ) ( context.dst - context.base );
			uintptr_t jmp_hopback = 0xFF - ( current_offset - target_inst_count ) - 1;

			asm_jmp_rel8( &context, ( unsigned char ) jmp_hopback );

			break;
		}
		case INTERP_RET:
		{
			char reg = jit_register_for_virtual( *++c );

			if ( reg != REG_RAX ) {
				asm_mov_reg_reg( &context, REG_RAX, reg );
			}

			// Backup non-volatile registers
			for ( char reg = REG_FIRST; reg < REG_LAST; ++reg ) {
				if ( backup_register_bits & ( 1 << reg ) )
					asm_pop_reg( &context, reg );
			}

			asm_ret( &context );

			free( inst_count );
			return f;
		}
		default:
			printf( "jit_compile: Unknown instruction %i\n", *c );
			break;
		}

		uintptr_t inst_count_offset = c - code;
		inst_count[ inst_count_offset ] = ( long ) ( context.dst - context.base );
	}
}
