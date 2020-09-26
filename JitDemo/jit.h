#pragma once

typedef long( *fn )( );

fn alloc_fn();
void free_fn( fn f );
char jit_register_for_virtual( char virtual_register );
int jit_is_volatile_register( char reg );
unsigned long jit_backup_registers( const unsigned char* code );
long jit_code_size( const unsigned char* code );
fn jit_compile( const unsigned char* code );
