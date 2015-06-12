#include "crc32.h"

int* CRC32::crc_table;

int* CRC32::generate_table() {
    int crc;
    int* table = new int[256];
    for ( int i = 0; i < 256; i++ ) {
        crc = i << 24;
        for ( int j = 0; j < 8; j++ ) {
            if ( crc & 0x80000000 )
                crc = ( crc << 1 ) ^ 0x04c11db7;
            else
                crc = crc << 1;
        }
        table[i] = crc;
    }
    return table;
}

int CRC32::reflect( int data, int bits ) {
    int x = 0;
    for ( int i = 0; i < bits; i++ ) {
        x = x << 1;
        x |= data & 1;
        data = data >> 1;
    }
    return x;
}

int CRC32::crc32( int crc, const char* data, int length ) {
    crc = ~reflect( crc, 32 );
    if ( !crc_table )
        crc_table = generate_table();
    for ( int i = 0; i < length;  i++ )
        crc = ( crc << 8 ) ^ crc_table[( ( crc >> 24 ) ^ reflect( data[i], 8 ) ) & 0xff];
    return ~reflect( crc, 32 );
}
