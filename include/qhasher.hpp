#ifndef HASHER_H
#define HASHER_H

#include <QString>
#include <QCryptographicHash>

#include "tiger.h"
#include "rmd160.h"
#include "haval.h"

struct QHashCalculator {
    virtual ~QHashCalculator() {}
    virtual QString hash( QByteArray msg, bool uppercase ) = 0;

    static QString hash( QByteArray text, bool uppercase, QHashCalculator& calculator );
};

struct QCRC16 : public QHashCalculator {
    QString hash( QByteArray msg, bool uppercase ) override;
};

struct QCRC32 : public QHashCalculator {
    QString hash( QByteArray msg, bool uppercase ) override;
};

struct QCRC64 : public QHashCalculator {
    QString hash( QByteArray msg, bool uppercase ) override;
};

struct QTiger : private Tiger, public QHashCalculator {
    QString hash( QByteArray msg, bool uppercase ) override;
};

struct QRipeMD : private Rmd160, public QHashCalculator {
    QString hash( QByteArray msg, bool uppercase ) override;
};

struct QHaval : private Haval, public QHashCalculator {
    QHaval( int bit ) : _bit( bit ) {}
    QString hash( QByteArray msg, bool uppercase ) override;

    private:
        const int _bit;
};

struct QCryptoAlgorithm : public QHashCalculator {
    QCryptoAlgorithm( QCryptographicHash::Algorithm algorithm ) : _algorithm( algorithm ) {}
    QString hash(QByteArray msg, bool uppercase) override;

    private:
        const QCryptographicHash::Algorithm _algorithm;
};

namespace QHashAlgorithm {
    extern QCRC16  CRC16;
    extern QCRC32  CRC32;
    extern QCRC64  CRC64;
    extern QTiger  TIGER;
    extern QRipeMD RIPEMD160;
    extern QHaval  HAVAL128;
    extern QHaval  HAVAL160;
    extern QHaval  HAVAL192;
    extern QHaval  HAVAL224;
    extern QHaval  HAVAL256;
    extern QCryptoAlgorithm MD4;
    extern QCryptoAlgorithm MD5;
    extern QCryptoAlgorithm SHA1;
    extern QCryptoAlgorithm SHA224;
    extern QCryptoAlgorithm SHA256;
    extern QCryptoAlgorithm SHA384;
    extern QCryptoAlgorithm SHA512;
    extern QCryptoAlgorithm SHA3_224;
    extern QCryptoAlgorithm SHA3_256;
    extern QCryptoAlgorithm SHA3_384;
    extern QCryptoAlgorithm SHA3_512;
}

#endif // HASHER_H
