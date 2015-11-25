#ifndef UTIL_HPP
#define UTIL_HPP

#include <QString>

namespace util {
    extern QString hash_hex( QByteArray text, bool uppercase );
    extern QString checksum_hex( quint64 checksum, bool uppercase );
}

#endif // UTIL_HPP
