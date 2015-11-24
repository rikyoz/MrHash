/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2015 by Riccardo Ostani                                 *
 *   email: rik20@live.it                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 2        *
 *   as published by the Free Software Foundation.                         *
 *                                                                         *
 ***************************************************************************/

#ifndef QCRYPTOHASH_HPP
#define QCRYPTOHASH_HPP

#include <QByteArray>
#include <QIODevice>
#include <QtGlobal>

#ifdef STATIC_LIB
#define LIBSHARED_EXPORT
#elif defined(LIB_LIBRARY)
#define LIBSHARED_EXPORT Q_DECL_EXPORT
#else
#define LIBSHARED_EXPORT Q_DECL_IMPORT
#endif

class HashAlgorithm;

class LIBSHARED_EXPORT QCryptoHash {
    public:
        enum Algorithm { TIGER, RMD160, WHIRLPOOL };

        explicit QCryptoHash( Algorithm method );
        ~QCryptoHash();

        void reset();

        void addData( const char* data, int length );
        void addData( const QByteArray &data );
        bool addData( QIODevice* device );

        QByteArray result() const;

        static QByteArray hash( const QByteArray &data, Algorithm method );

    private:
        Q_DISABLE_COPY( QCryptoHash )

        HashAlgorithm* hash_algorithm;
};

#endif // QCRYPTOHASH_HPP
