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
    static QString hash( QByteArray text, bool uppercase, QCryptographicHash::Algorithm algorithm );
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
    QHaval( int bit ) : _bit( bit ) { }
    QString hash( QByteArray msg, bool uppercase ) override;

    private:
        const int _bit;
};

namespace QHashAlgorithm {
    static QCRC16  CRC16;
    static QCRC32  CRC32;
    static QCRC64  CRC64;
    static QTiger  TIGER;
    static QRipeMD RIPEMD160;
    static QHaval  HAVAL128(128);
    static QHaval  HAVAL160(160);
    static QHaval  HAVAL192(192);
    static QHaval  HAVAL224(224);
    static QHaval  HAVAL256(256);
}

#endif // HASHER_H
