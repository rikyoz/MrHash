#include "filehashcalculator.hpp"

#include <QFile>
#include <QCryptographicHash>

#include <memory>

#include "qtcryptohash/qcryptohash.hpp"
#include "qextrahash.hpp"
#include "crc.hpp"

#define BUFFER_SIZE 8 * 1024 //8 KB

using std::vector;
using std::unique_ptr;

FileHashCalculator::FileHashCalculator( QWidget* parent, QString fileName ) : QThread( parent ),
    file_name( fileName ) {
}

FileHashCalculator::~FileHashCalculator() {}

void FileHashCalculator::run() {
    QFile file( file_name );
    if ( file.open( QFile::ReadOnly ) ) {
        boost_crc16 crc16;
        boost_crc32 crc32;
        boost_crc64 crc64;
        crc16.reset( 0 );

        QCryptographicHash md4( QCryptographicHash::Md4 );
        QCryptographicHash md5( QCryptographicHash::Md5 );
        QCryptographicHash sha1( QCryptographicHash::Sha1 );
        QCryptographicHash sha224( QCryptographicHash::Sha224 );
        QCryptographicHash sha256( QCryptographicHash::Sha256 );
        QCryptographicHash sha384( QCryptographicHash::Sha384 );
        QCryptographicHash sha512( QCryptographicHash::Sha512 );
        QCryptographicHash sha3_224( QCryptographicHash::Sha3_224 );
        QCryptographicHash sha3_256( QCryptographicHash::Sha3_256 );
        QCryptographicHash sha3_384( QCryptographicHash::Sha3_384 );
        QCryptographicHash sha3_512( QCryptographicHash::Sha3_512 );

        QCryptoHash tiger( QCryptoHash::TIGER );
        QCryptoHash ripemd( QCryptoHash::RMD160 );

        QExtraHash haval128( QExtraHash::HAVAL128 );
        QExtraHash haval160( QExtraHash::HAVAL160 );
        QExtraHash haval192( QExtraHash::HAVAL192 );
        QExtraHash haval224( QExtraHash::HAVAL224 );
        QExtraHash haval256( QExtraHash::HAVAL256 );

        while ( !file.atEnd() ) {
            QByteArray data = file.read( BUFFER_SIZE );

            crc16.process_bytes( data.constData(), data.length() );
            crc32.process_bytes( data.constData(), data.length() );
            crc64.process_bytes( data.constData(), data.length() );

            md4.addData( data );
            md5.addData( data );
            sha1.addData( data );
            sha224.addData( data );
            sha256.addData( data );
            sha384.addData( data );
            sha512.addData( data );
            sha3_224.addData( data );
            sha3_256.addData( data );
            sha3_384.addData( data );
            sha3_512.addData( data );

            tiger.addData( data );
            ripemd.addData( data );

            haval128.addData( data );
            haval160.addData( data );
            haval192.addData( data );
            haval224.addData( data );
            haval256.addData( data );
        }
        emit newChecksumValue( 0, crc16.checksum() );
        emit newChecksumValue( 1, crc32.checksum() );
        emit newChecksumValue( 2, crc64.checksum() );

        emit newHashString( 3, md4.result() );
        emit newHashString( 4, md5.result() );
        emit newHashString( 5, sha1.result() );
        emit newHashString( 6, sha224.result() );
        emit newHashString( 7, sha256.result() );
        emit newHashString( 8, sha384.result() );
        emit newHashString( 9, sha512.result() );
        emit newHashString( 10, sha3_224.result() );
        emit newHashString( 11, sha3_256.result() );
        emit newHashString( 12, sha3_384.result() );
        emit newHashString( 13, sha3_512.result() );

        emit newHashString( 14, tiger.result() );
        emit newHashString( 15, ripemd.result() );

        emit newHashString( 16, haval128.result() );
        emit newHashString( 17, haval160.result() );
        emit newHashString( 18, haval192.result() );
        emit newHashString( 19, haval224.result() );
        emit newHashString( 20, haval256.result() );
    }
}
