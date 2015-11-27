#ifndef HASHER_H
#define HASHER_H

#include <QString>

#include "boost/crc.hpp"

using boost::crc_optimal;
using boost::crc_ccitt_type;

// CRC16-CCITT (X.25) - the same used by qChecksum
typedef crc_optimal< 16, 0x1021, 0xFFFF, 0xFFFF, true, true > boost_crc16;

// CRC-32 (PKZIP) - http://reveng.sourceforge.net/crc-catalogue/17plus.htm#crc.cat-bits.32
typedef crc_optimal< 32, 0x04c11db7, 0xffffffff, 0xffffffff, true, true > boost_crc32;

// CRC-64 (XZ) - http://reveng.sourceforge.net/crc-catalogue/17plus.htm#crc.cat-bits.64
typedef crc_optimal< 64, 0x42f0e1eba9ea3693, 0xffffffffffffffffULL, 0xffffffffffffffffULL, true, true > boost_crc64;

struct QChecksum {
    enum Algorithm { CRC16, CRC32, CRC64 };
    static quint64 checksum( const QByteArray msg , Algorithm algorithm );
};

#endif // HASHER_H
