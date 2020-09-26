#pragma once

unsigned char encode_long( long value, int index );
unsigned char encode_address( void* address, int index );
long decode_long( unsigned char a, unsigned char b, unsigned char c, unsigned char d );
