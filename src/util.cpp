#include "util.hpp"

QString util::hash_hex( QByteArray text, bool uppercase ) {
    QString result = text.toHex();
    return uppercase ? result.toUpper() : result;
}

QString util::checksum_hex( quint64 checksum, bool uppercase ) {
    QString result = QString::number( checksum, 16 );
    return uppercase ? result.toUpper() : result;
}
