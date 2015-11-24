#include "qhasher.hpp"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include "boost/crc.hpp"

using boost::crc_optimal;

typedef crc_optimal< 32, 0x04c11db7, 0xffffffff, 0xffffffff, true, true > boost_crc32;
typedef crc_optimal< 64, 0x42f0e1eba9ea3693, 0xffffffffffffffffULL, 0xffffffffffffffffULL, true, true > boost_crc64;

boost_crc32 crc32_calc;
boost_crc64 crc64_calc;

QString QHashCalculator::hash( QByteArray data, bool uppercase, QHashCalculator &calculator ) {
    return calculator.hash( data, uppercase );
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
    QString result = QCryptoHash::hash( msg, QCryptoHash::TIGER ).toHex();
    return uppercase ? result.toUpper() : result;
}

QString QRipeMD::hash( QByteArray msg, bool uppercase ) {
    QString result = QCryptoHash::hash( msg, QCryptoHash::RMD160 ).toHex();
    return uppercase ? result.toUpper() : result;
}

QString QHaval::hash( QByteArray msg, bool uppercase ) {
    string msg_str( msg.constData(), msg.length() );
    Haval hav( _bit, 5); //Note: Mr Hash calculates only Haval hashes with 5 passes
    string std_result = hav.calcHaval( msg_str );
    QString result = QString::fromStdString( std_result );
    return uppercase ? result : result.toLower();
}

QString QCryptoAlgorithm::hash( QByteArray msg, bool uppercase ) {
    QString result = QCryptographicHash::hash( msg, _algorithm ).toHex();
    return uppercase ? result.toUpper() : result;
}

namespace QHashAlgorithm {
    QCRC16  CRC16;
    QCRC32  CRC32;
    QCRC64  CRC64;
    QTiger  TIGER;
    QRipeMD RIPEMD160;
    QHaval  HAVAL128( 128 );
    QHaval  HAVAL160( 160 );
    QHaval  HAVAL192( 192 );
    QHaval  HAVAL224( 224 );
    QHaval  HAVAL256( 256 );
    QCryptoAlgorithm MD4( QCryptographicHash::Md4 );
    QCryptoAlgorithm MD5( QCryptographicHash::Md5 );
    QCryptoAlgorithm SHA1( QCryptographicHash::Sha1 );
    QCryptoAlgorithm SHA224( QCryptographicHash::Sha224 );
    QCryptoAlgorithm SHA256( QCryptographicHash::Sha256 );
    QCryptoAlgorithm SHA384( QCryptographicHash::Sha384 );
    QCryptoAlgorithm SHA512( QCryptographicHash::Sha512 );
    QCryptoAlgorithm SHA3_224( QCryptographicHash::Sha3_224 );
    QCryptoAlgorithm SHA3_256( QCryptographicHash::Sha3_256 );
    QCryptoAlgorithm SHA3_384( QCryptographicHash::Sha3_384 );
    QCryptoAlgorithm SHA3_512( QCryptographicHash::Sha3_512 );

}
