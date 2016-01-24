#ifndef QEXTRAHASH_HPP
#define QEXTRAHASH_HPP

#include <QByteArray>
#include <QIODevice>
#include <QtGlobal>

class HashAlgorithm;

class QExtraHash {
    public:
        enum Algorithm { HAVAL128, HAVAL160, HAVAL192, HAVAL224, HAVAL256 };

        explicit QExtraHash( Algorithm method );
        ~QExtraHash();

        void reset();

        void addData( const QByteArray &data );

        QByteArray result() const;

        static QByteArray hash( const QByteArray &data, Algorithm method );

    private:
        Q_DISABLE_COPY( QExtraHash )

        HashAlgorithm* mHashAlgorithm;
};

#endif // QEXTRAHASH_HPP
