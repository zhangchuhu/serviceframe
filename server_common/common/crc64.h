#ifndef _CRC64_H_
#define _CRC64_H_

#include <stdint.h>

#define UINT64CONST(x) ((uint64_t) x##ULL)

namespace crc64
{


extern const uint64_t crc64_table[256];

inline void init_crc(uint64_t & crc)
{
	crc = UINT64CONST(0xffffffffffffffff);
}

inline void fin_crc(uint64_t & crc)
{
	crc ^= UINT64CONST(0xffffffffffffffff);
}

inline void accumulate_crc(uint64_t & crc,unsigned char* pbData, uint64_t len)
{
	unsigned char* _pbData=pbData;
	uint64_t _len=len;
	while (_len-- > 0)
	{
		unsigned int _tab_index = ((unsigned int) (crc >> 56) ^ *_pbData++) & 0xFF;
		crc = crc64_table[_tab_index] ^ (crc << 8);
	}
}


}

#endif

