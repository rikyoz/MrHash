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
}

#endif // HASHER_H
