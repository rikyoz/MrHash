#ifndef QEXTRAHASH_HPP
#define QEXTRAHASH_HPP

#include <QByteArray>
#include <QIODevice>
#include <QtGlobal>

class HashAlgorithm;

class QExtraHash {
    public:
        enum Algorithm { CRC16, CRC32, CRC64, HAVAL128, HAVAL160, HAVAL192, HAVAL224, HAVAL256 };

        explicit QExtraHash( Algorithm method );
        ~QExtraHash();

        void reset();

        void addData( const char* data, int length );
        void addData( const QByteArray &data );
        bool addData( QIODevice* device );

        QByteArray result() const;

        static QByteArray hash( const QByteArray &data, Algorithm method );

    private:
        Q_DISABLE_COPY( QExtraHash )

        HashAlgorithm* hash_algorithm;
};

#endif // QEXTRAHASH_HPP
