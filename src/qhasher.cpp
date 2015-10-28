#include "qhasher.hpp"

#include "tiger.h"
#include "rmd160.h"
#include "haval.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <memory>

#include "boost/crc.hpp"

using boost::crc_optimal;
using std::unique_ptr;

crc_optimal<32, 0x04c11db7, 0xffffffff, 0xffffffff, true, true> crc32_calc;
crc_optimal<64, 0x42f0e1eba9ea3693, 0xffffffffffffffffULL, 0xffffffffffffffffULL, true, true> crc64_calc;

class QCRC16 : public QHashCalculator {
    public:
        QString hash( QByteArray msg, bool uppercase ) override {
            QString result = QString::number( qChecksum( msg.constData(), msg.length() ), 16 );
            return uppercase ? result.toUpper() : result;
        }
};

class QCRC32 : public QHashCalculator {
    public:
        QString hash( QByteArray msg, bool uppercase ) override {
            crc32_calc.reset();
            crc32_calc.process_bytes( msg.constData(), msg.size() );
            QString result = QString::number( crc32_calc.checksum(), 16 );
            return uppercase ? result.toUpper() : result;
        }
};

class QCRC64 : public QHashCalculator {
    public:
        QString hash( QByteArray msg, bool uppercase ) override {
            crc64_calc.reset();
            crc64_calc.process_bytes( msg.constData(), msg.size() );
            QString result = QString::number( crc64_calc.checksum(), 16 );
            return uppercase ? result.toUpper() : result;
        }
};

class QTiger : public Tiger, public QHashCalculator {
    public:
        virtual ~QTiger() {}

        QString hash( QByteArray msg, bool uppercase ) override {
            tiger_init();
            tiger_write( reinterpret_cast<const byte*>( msg.constData() ), msg.length() );
            string std_result = charToHex( reinterpret_cast<const char *>( tiger_final() ), TIGER_HASHLEN_BYTE );
            QString result = QString::fromStdString( std_result );
            return uppercase ? result : result.toLower();
        }
};

class QRipeMD : private Rmd160, public QHashCalculator {
    public:
        QString hash( QByteArray msg, bool uppercase ) override {
            rmd160_init();
            rmd160_write( reinterpret_cast<const byte*>( msg.constData() ), msg.length() );
            string std_result = charToHex( reinterpret_cast<const char*>( rmd160_final() ), RMD160_HASHLEN_BYTE );
            QString result = QString::fromStdString( std_result );
            return uppercase ? result : result.toLower();
        }
};

class QHaval : private Haval, public QHashCalculator {
    public:
        QHaval( int bit, int passes ) : m_bit( bit ), m_passes( passes ) { }

        QString hash( QByteArray msg, bool uppercase ) override {
            string msg_str( msg.constData(), msg.length() );
            Haval hav;
            string std_result = hav.calcHaval( msg_str, m_bit, m_passes );
            QString result = QString::fromStdString( std_result );
            return uppercase ? result : result.toLower();
        }

    private:
        const int m_bit;
        const int m_passes;
};

QString QHashCalculator::hash( QByteArray data, bool uppercase, QHashAlgorithm algorithm ) {
    unique_ptr< QHashCalculator > calculator;
    switch( algorithm ) {
        case QHashAlgorithm::CRC16:
            calculator.reset( new QCRC16() );
            break;
        case QHashAlgorithm::CRC32:
            calculator.reset( new QCRC32() );
            break;
        case QHashAlgorithm::CRC64:
            calculator.reset( new QCRC64() );
            break;
        case QHashAlgorithm::TIGER:
            calculator.reset( new QTiger() );
            break;
        case QHashAlgorithm::RIPEMD160:
            calculator.reset( new QRipeMD() );
            break;
        case QHashAlgorithm::HAVAL128:
            calculator.reset( new QHaval( 128, 5 ) );
            break;
        case QHashAlgorithm::HAVAL160:
            calculator.reset( new QHaval( 160, 5 ) );
            break;
        case QHashAlgorithm::HAVAL192:
            calculator.reset( new QHaval( 192, 5 ) );
            break;
        case QHashAlgorithm::HAVAL224:
            calculator.reset( new QHaval( 224, 5 ) );
            break;
        case QHashAlgorithm::HAVAL256:
            calculator.reset( new QHaval( 256, 5 ) );
            break;
        default:
            return "(algorithm not supported)";
    }
    return calculator->hash( data, uppercase );
}

QString QHashCalculator::hash( QByteArray text, bool uppercase, QCryptographicHash::Algorithm algorithm ) {
    QString result = QCryptographicHash::hash( text, algorithm ).toHex();
    return uppercase ? result.toUpper() : result;
}
