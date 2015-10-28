#include "qhasher.hpp"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include "boost/crc.hpp"

boost::crc_optimal< 32, 0x04c11db7, 0xffffffff, 0xffffffff, true, true > crc32_calc;
boost::crc_optimal< 64,
                    0x42f0e1eba9ea3693,
                    0xffffffffffffffffULL,
                    0xffffffffffffffffULL, true, true > crc64_calc;

QString QHashCalculator::hash( QByteArray data, bool uppercase, QHashCalculator& calculator ) {
    return calculator.hash( data, uppercase );
}

QString QHashCalculator::hash( QByteArray text, bool uppercase, QCryptographicHash::Algorithm algorithm ) {
    QString result = QCryptographicHash::hash( text, algorithm ).toHex();
    return uppercase ? result.toUpper() : result;
}

QString QCRC16::hash( QByteArray msg, bool uppercase ) {
    QString result = QString::number( qChecksum( msg.constData(), msg.length() ), 16 );
    return uppercase ? result.toUpper() : result;
}

QString QCRC32::hash( QByteArray msg, bool uppercase ) {
    crc32_calc.reset();
    crc32_calc.process_bytes( msg.constData(), msg.size() );
    QString result = QString::number( crc32_calc.checksum(), 16 );
    return uppercase ? result.toUpper() : result;
}

QString QCRC64::hash( QByteArray msg, bool uppercase ) {
    crc64_calc.reset();
    crc64_calc.process_bytes( msg.constData(), msg.size() );
    QString result = QString::number( crc64_calc.checksum(), 16 );
    return uppercase ? result.toUpper() : result;
}

QString QTiger::hash( QByteArray msg, bool uppercase ) {
    tiger_init();
    tiger_write( reinterpret_cast<const byte*>( msg.constData() ), msg.length() );
    string std_result = charToHex( reinterpret_cast<const char *>( tiger_final() ), TIGER_HASHLEN_BYTE );
    QString result = QString::fromStdString( std_result );
    return uppercase ? result : result.toLower();
}

QString QRipeMD::hash( QByteArray msg, bool uppercase ) {
    rmd160_init();
    rmd160_write( reinterpret_cast<const byte*>( msg.constData() ), msg.length() );
    string std_result = charToHex( reinterpret_cast<const char*>( rmd160_final() ), RMD160_HASHLEN_BYTE );
    QString result = QString::fromStdString( std_result );
    return uppercase ? result : result.toLower();
}

QString QHaval::hash( QByteArray msg, bool uppercase ) {
    string msg_str( msg.constData(), msg.length() );
    Haval hav;
    //Note: Mr Hash calculates only Haval hashes with 5 passes
    string std_result = hav.calcHaval( msg_str, _bit, 5 );
    QString result = QString::fromStdString( std_result );
    return uppercase ? result : result.toLower();
}
