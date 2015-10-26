#ifndef HASHER_H
#define HASHER_H

#include <iostream>

#include <QString>
#include <QCryptographicHash>

enum class QHashAlgorithm { CRC16, CRC32, CRC64, TIGER, RIPEMD160, HAVAL128, HAVAL160,
                            HAVAL192, HAVAL224, HAVAL256 };

class QHasher {
    private:
        QHasher();

    public:
        static QString hash( std::string text, bool uppercase, QHashAlgorithm algorithm );
        static QString hash( QByteArray text, bool uppercase, QCryptographicHash::Algorithm algorithm );
};

#endif // HASHER_H
