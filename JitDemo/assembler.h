#pragma once

#define REG_RAX 0 // 0
#define REG_RCX 1 // 8
#define REG_RDX 2 // 16
#define REG_RBX 3 // 24
#define REG_RSP 4 // 32
#define REG_RBP 5 // 40
#define REG_RSI 6 // 48
#define REG_RDI 7 // 56
#define REG_FIRST REG_RAX
#define REG_LAST REG_RDI + 1

struct asm_t {
	unsigned char* base;
	unsigned char* dst;
};

void asm_begin( struct asm_t* context, unsigned char* fn );

void asm_write_bytes( struct asm_t* context, unsigned char n, ... );
void asm_write_byte_array( struct asm_t* context, unsigned char n, unsigned char* bytes );
void asm_push_reg( struct asm_t* context, unsigned char reg );
void asm_pop_reg( struct asm_t* context, unsigned char reg );
void asm_add_reg_reg( struct asm_t* context, unsigned char dst, unsigned char src );
void asm_sub_reg_reg( struct asm_t* context, unsigned char dst, unsigned char src );
void asm_mov_reg_reg( struct asm_t* context, unsigned char dst, unsigned char src );
void asm_mov_reg_const32( struct asm_t* context, unsigned char dst, long val32 );
void asm_mov_reg_mem( struct asm_t* context, unsigned char dst, long* src );
void asm_cmp_reg_reg( struct asm_t* context, unsigned char reg1, unsigned char reg2 );
void asm_cmp_reg_const8( struct asm_t* context, unsigned char reg, unsigned char val8 );
void asm_jmp_rel8( struct asm_t* context, unsigned char rel8 );
void asm_je_rel8( struct asm_t* context, unsigned char rel8 );
void asm_ret( struct asm_t* context );
