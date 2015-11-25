#include "crc.hpp"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include "boost/crc.hpp"
#include "qtcryptohash/qcryptohash.hpp"

#include "qextrahash.hpp"

boost_crc16 crc16_calc;
boost_crc32 crc32_calc;
boost_crc64 crc64_calc;

quint64 QChecksum::checksum( const QByteArray msg, QChecksum::Algorithm algorithm ) {
    switch ( algorithm ) {
        case QChecksum::CRC16:
            crc16_calc.reset( 0 );
            crc16_calc.process_bytes( msg.constData(), msg.length() );
            return crc16_calc.checksum();
        case QChecksum::CRC32:
            crc32_calc.reset();
            crc32_calc.process_bytes( msg.constData(), msg.size() );
            return crc32_calc.checksum();
        case QChecksum::CRC64:
            crc64_calc.reset();
            crc64_calc.process_bytes( msg.constData(), msg.size() );
            return crc64_calc.checksum();
        default:
            throw std::invalid_argument( "Checksum algorithm not valid!" );
    }
}
