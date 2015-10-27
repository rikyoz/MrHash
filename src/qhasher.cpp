#include "qhasher.hpp"

#include "tiger.h"
#include "rmd160.h"
#include "haval.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include "boost/crc.hpp"

using boost::crc_optimal;

crc_optimal<32, 0x04c11db7, 0xffffffff, 0xffffffff, true, true> crc32_calc;
crc_optimal<64, 0x42f0e1eba9ea3693, 0xffffffffffffffffULL, 0xffffffffffffffffULL, true, true> crc64_calc;

class QTiger : private Tiger {
    public:
        QString hash( QByteArray msg ) {
            tiger_init();
            tiger_write( reinterpret_cast<const byte*>( msg.constData() ), msg.length() );
            string result = charToHex( reinterpret_cast<const char *>( tiger_final() ), TIGER_HASHLEN_BYTE );
            return QString::fromStdString( result );
        }
};

class QRipeMD : private Rmd160 {
    public:
        QString hash( QByteArray msg ) {
            rmd160_init();
            rmd160_write( reinterpret_cast<const byte*>( msg.constData() ), msg.length() );
            string result = charToHex( reinterpret_cast<const char*>( rmd160_final() ), RMD160_HASHLEN_BYTE );
            return QString::fromStdString( result );
        }
};

class QHaval : private Haval {
    public:
        QString hash( QByteArray msg, int bit, int passes ) {
            string msg_str( msg.constData(), msg.length() );
            Haval hav;
            string result = hav.calcHaval( msg_str, bit, passes );
            return QString::fromStdString( result );
        }
};

QString crc16( QByteArray msg, bool uppercase ) {
    QString result = QString::number( qChecksum( msg.constData(), msg.length() ), 16 );
    return uppercase ? result.toUpper() : result;
}

QString crc32( QByteArray msg, bool uppercase ) {
    crc32_calc.reset();
    crc32_calc.process_bytes( msg.constData(), msg.size() );
    QString result = QString::number( crc32_calc.checksum(), 16 );
    return uppercase ? result.toUpper() : result;
}

QString crc64( QByteArray msg, bool uppercase ) {
    crc64_calc.reset();
    crc64_calc.process_bytes( msg.constData(), msg.size() );
    QString result = QString::number( crc64_calc.checksum(), 16 );
    return uppercase ? result.toUpper() : result;
}

QString tiger( QByteArray msg, bool uppercase ) {
    QTiger tiger;
    QString result = tiger.hash( msg );
    return uppercase ? result : result.toLower();
}

QString ripemd( QByteArray msg, bool uppercase ) {
    QRipeMD rmd;
    QString result = rmd.hash( msg );
    return uppercase ? result : result.toLower();
}

QString haval( QByteArray msg, int bits, bool uppercase ) {
    QHaval hav;
    QString result = hav.hash( msg, bits, 5 );
    return uppercase ? result : result.toLower();
}

QString QHasher::hash( QByteArray data, bool uppercase, QHashAlgorithm algorithm ) {
    switch( algorithm ) {
        case QHashAlgorithm::CRC16:
            return crc16( data, uppercase );
        case QHashAlgorithm::CRC32:
            return crc32( data, uppercase );
        case QHashAlgorithm::CRC64:
            return crc64( data, uppercase );
        case QHashAlgorithm::TIGER:
            return tiger( data, uppercase );
        case QHashAlgorithm::RIPEMD160:
            return ripemd( data, uppercase );
        case QHashAlgorithm::HAVAL128:
            return haval( data, 128, uppercase );
        case QHashAlgorithm::HAVAL160:
            return haval( data, 160, uppercase );
        case QHashAlgorithm::HAVAL192:
            return haval( data, 192, uppercase );
        case QHashAlgorithm::HAVAL224:
            return haval( data, 224, uppercase );
        case QHashAlgorithm::HAVAL256:
            return haval( data, 256, uppercase );
        default:
            return "(algorithm not supported)";
    }
}

QString QHasher::hash( QByteArray text, bool uppercase, QCryptographicHash::Algorithm algorithm ) {
    QString result = QCryptographicHash::hash( text, algorithm ).toHex();
    return uppercase ? result.toUpper() : result;
}
