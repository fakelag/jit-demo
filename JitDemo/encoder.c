#include "Encoder.h"

struct encoder_t {
	union {
		unsigned char _bytes[ 8 ];
		long _long32;
		unsigned long long _address;
	};
};

unsigned char encode_long( long value, int index ) {
	struct encoder_t encoder;

	encoder._long32 = value;
	return encoder._bytes[ index ];
}

unsigned char encode_address( void* address, int index ) {
	struct encoder_t encoder;

	encoder._address = ( unsigned long long ) address;
	return encoder._bytes[ index ];
}

long decode_long( unsigned char a, unsigned char b, unsigned char c, unsigned char d ) {
	struct encoder_t encoder;

	encoder._bytes[ 0 ] = a;
	encoder._bytes[ 1 ] = b;
	encoder._bytes[ 2 ] = c;
	encoder._bytes[ 3 ] = d;

	return encoder._long32;
}
