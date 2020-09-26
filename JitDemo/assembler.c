#include "stdarg.h"
#include "assembler.h"
#include "encoder.h"

void asm_begin( struct asm_t* context, unsigned char* fn ) {
	context->base = fn;
	context->dst = fn;
}

void asm_write_bytes( struct asm_t* context, unsigned char n, ... ) {
	va_list bytes;

	va_start( bytes, n );
	for ( int i = 0; i < n; ++i ) {
		*( context->dst++ ) = ( char ) va_arg( bytes, int );
	}
	va_end( bytes );
}

void asm_write_byte_array( struct asm_t* context, unsigned char n, unsigned char* bytes ) {
	for ( int i = 0; i < n; ++i ) {
		*( context->dst++ ) = bytes[ i ];
	}
}

void asm_push_reg( struct asm_t* context, unsigned char reg ) {
	asm_write_bytes( context, 1, 0x50 | reg );
}

void asm_pop_reg( struct asm_t* context, unsigned char reg ) {
	asm_write_bytes( context, 1, 0x58 | reg );
}

void asm_add_reg_reg( struct asm_t* context, unsigned char dst, unsigned char src ) {
	asm_write_bytes( context, 3, 0x48, 0x01, 0xC0 | dst | src << 3 );
}

void asm_sub_reg_reg( struct asm_t* context, unsigned char dst, unsigned char src ) {
	asm_write_bytes( context, 3, 0x48, 0x29, 0xC0 | dst | src << 3 );
}

void asm_mov_reg_reg( struct asm_t* context, unsigned char dst, unsigned char src ) {
	asm_write_bytes( context, 3, 0x48, 0x89, 0xC0 | dst | src << 3 );
}

void asm_mov_reg_const32( struct asm_t* context, unsigned char dst, long val32 ) {
	asm_write_bytes( context, 7, 0x48, 0xC7, 0xC0 | dst,
		encode_long( val32, 0 ),
		encode_long( val32, 1 ),
		encode_long( val32, 2 ),
		encode_long( val32, 3 )
	);
}

void asm_mov_reg_mem( struct asm_t* context, unsigned char dst, long* src ) {
	unsigned char srcEncoded[] ={
		encode_address( src, 0 ),
		encode_address( src, 1 ),
		encode_address( src, 2 ),
		encode_address( src, 3 ),
		encode_address( src, 4 ),
		encode_address( src, 5 ),
		encode_address( src, 6 ),
		encode_address( src, 7 )
	};

	if ( dst == REG_RAX ) {
		// If moving to rax, use shorter instruction
		// movabs %rax, [0xADDRESSADDRESS]
		asm_write_bytes( context, 2, 0x48, 0xA1 );
		asm_write_byte_array( context, 8, srcEncoded );
	}
	else {
		// Write 2 instructions
		// movabs rcx, 0xADDRESSADDRESS
		// mov rcx, QWORD PTR [rcx]
		asm_write_bytes( context, 2, 0x48, 0xB8 | dst );
		asm_write_byte_array( context, 8, srcEncoded );
		asm_write_bytes( context, 3, 0x48, 0x8B, dst | dst << 3 );
	}
}

void asm_cmp_reg_reg( struct asm_t* context, unsigned char reg1, unsigned char reg2 ) {
	asm_write_bytes( context, 3, 0x48, 0x39, 0xC0 | reg1 | reg2 << 3 );
}

void asm_cmp_reg_const8( struct asm_t* context, unsigned char reg, unsigned char val8 ) {
	asm_write_bytes( context, 4, 0x48, 0x83, 0xF8 | reg, val8 );
}

void asm_jmp_rel8( struct asm_t* context, unsigned char rel8 ) {
	asm_write_bytes( context, 2, 0xEB, rel8 );
}

void asm_je_rel8( struct asm_t* context, unsigned char rel8 ) {
	asm_write_bytes( context, 2, 0x74, rel8 );
}

void asm_ret( struct asm_t* context ) {
	asm_write_bytes( context, 1, 0xC3 );
}
