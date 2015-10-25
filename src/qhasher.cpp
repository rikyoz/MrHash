#include "qhasher.hpp"

#include "crc32.h"
#include "tiger.h"
#include "rmd160.h"
#include "haval.h"

QString crc16( string msg, bool uppercase ) {
    QString result = QString::number( qChecksum( msg.c_str(), qstrlen( msg.c_str() ) ), 16 );
    return uppercase ? result.toUpper() : result;
}

QString crc32( string msg, bool uppercase ) {
    int crc32_ctx = CRC32::crc32( 0, msg.c_str(), msg.length() );
    //note: right(8) removes the unneeded Fs that appear sometimes at the start of the crc32
    QString result = QString::number( crc32_ctx, 16 ).right( 8 );
    return uppercase ? result.toUpper() : result;
}

QString tiger( string msg, bool uppercase ) {
    Tiger tiger;
    QString result = QString::fromStdString( tiger.calcTiger( msg ) );
    return uppercase ? result : result.toLower();
}

QString ripemd( string msg, bool uppercase ) {
    Rmd160 rmd;
    QString result = QString::fromStdString( rmd.calcRmd160( msg ) );
    return uppercase ? result : result.toLower();
}

QString haval( string msg, int bits, bool uppercase ) {
    Haval hav;
    QString result = QString::fromStdString( hav.calcHaval( msg, bits, 5 ) );
    return uppercase ? result : result.toLower();
}

QString QHasher::hash( string text, bool uppercase, QHashAlgorithm algorithm ) {
    switch( algorithm ) {
        case QHashAlgorithm::CRC16:
            return crc16( text, uppercase );
        case QHashAlgorithm::CRC32:
            return crc32( text, uppercase );
        case QHashAlgorithm::TIGER:
            return tiger( text, uppercase );
        case QHashAlgorithm::RIPEMD160:
            return ripemd( text, uppercase );
        case QHashAlgorithm::HAVAL128:
            return haval( text, 128, uppercase );
        case QHashAlgorithm::HAVAL160:
            return haval( text, 160, uppercase );
        case QHashAlgorithm::HAVAL192:
            return haval( text, 192, uppercase );
        case QHashAlgorithm::HAVAL224:
            return haval( text, 224, uppercase );
        case QHashAlgorithm::HAVAL256:
            return haval( text, 256, uppercase );
        default:
            return "(algorithm not supported)";
    }
}

QString QHasher::hash( QByteArray text, bool uppercase, QCryptographicHash::Algorithm algorithm ) {
    QString result = QCryptographicHash::hash( text, algorithm ).toHex();
    return uppercase ? result.toUpper() : result;
}
