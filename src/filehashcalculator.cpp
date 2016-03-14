#include "filehashcalculator.hpp"

#include <QFile>
#include <QCryptographicHash>

#include <memory>

#include "qtcryptohash/qcryptohash.hpp"
#include "crc.hpp"

#define BUFFER_SIZE 8 * 1024 //8 KB

using std::vector;
using std::unique_ptr;

FileHashCalculator::FileHashCalculator( QWidget* parent, QString fileName )
    : QThread( parent ), mFileName( fileName ), mIsPaused( false ) {
}

FileHashCalculator::~FileHashCalculator() {}

void FileHashCalculator::stop() {
    disconnect();
    requestInterruption();
    wait();
}

void FileHashCalculator::resume() {
    mMutex.lock();
    mIsPaused = false;
    mMutex.unlock();
    mPauseCondition.wakeAll();
}

void FileHashCalculator::pause() {
    QMutexLocker locker( &mMutex );
    mIsPaused = true;
}

bool FileHashCalculator::isPaused() {
    QMutexLocker locker( &mMutex );
    return mIsPaused;
}

void FileHashCalculator::run() {
    QFile file( mFileName );
    if ( file.open( QFile::ReadOnly ) ) {
        boost_crc16 crc16;
        boost_crc32 crc32;
        boost_crc64 crc64;

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

        quint64 current = 0;
        quint64 total = file.size();
        while ( !isInterruptionRequested() && !file.atEnd() ) {
            mMutex.lock();
            if ( mIsPaused ) {
                mPauseCondition.wait( &mMutex ); //wait for a call to resume()
            }
            mMutex.unlock();

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

            current += data.size();
            emit progressUpdate( ( float )current / total );
        }
        if ( !isInterruptionRequested() )
            emit newChecksumValue( 0, crc16.checksum() );
        if ( !isInterruptionRequested() )
            emit newChecksumValue( 1, crc32.checksum() );
        if ( !isInterruptionRequested() )
            emit newChecksumValue( 2, crc64.checksum() );

        if ( !isInterruptionRequested() )
            emit newHashString( 3, md4.result() );
        if ( !isInterruptionRequested() )
            emit newHashString( 4, md5.result() );
        if ( !isInterruptionRequested() )
            emit newHashString( 5, sha1.result() );
        if ( !isInterruptionRequested() )
            emit newHashString( 6, sha224.result() );
        if ( !isInterruptionRequested() )
            emit newHashString( 7, sha256.result() );
        if ( !isInterruptionRequested() )
            emit newHashString( 8, sha384.result() );
        if ( !isInterruptionRequested() )
            emit newHashString( 9, sha512.result() );
        if ( !isInterruptionRequested() )
            emit newHashString( 10, sha3_224.result() );
        if ( !isInterruptionRequested() )
            emit newHashString( 11, sha3_256.result() );
        if ( !isInterruptionRequested() )
            emit newHashString( 12, sha3_384.result() );
        if ( !isInterruptionRequested() )
            emit newHashString( 13, sha3_512.result() );

        if ( !isInterruptionRequested() )
            emit newHashString( 14, tiger.result() );
        if ( !isInterruptionRequested() )
            emit newHashString( 15, ripemd.result() );
    }
}
