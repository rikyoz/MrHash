#ifndef _CRC32_H_
#define _CRC32_H_

namespace CRC32 {
	extern int* crc_table;
	int* generate_table();
	int reflect(int data, int bits);
	int crc32(int crc, const char* data, int length);
}

#endif // _CRC32_H_
