#include "filehashcalculator.hpp"

#include <QFile>
#include <QCryptographicHash>

#include <memory>

#include "qtcryptohash/qcryptohash.hpp"
#include "qextrahash.hpp"
#include "crc.hpp"

using std::vector;
using std::unique_ptr;

FileHashCalculator::FileHashCalculator( QWidget* parent, QString fileName ) : QThread( parent ),
    file_name( fileName ) {
}

FileHashCalculator::~FileHashCalculator() {}

void FileHashCalculator::run() {
    QFile file( file_name );
    if ( file.open( QFile::ReadOnly ) ) {
        QByteArray content = file.readAll(); //TODO: Optimize file read and hash calculation!
        uint i = QChecksum::CRC16;
        for( ; !isInterruptionRequested() && i <= QChecksum::CRC64; ++i )
            emit newChecksumValue( i, QChecksum::checksum( content, static_cast<QChecksum::Algorithm>( i ) ) );

        uint j = QCryptographicHash::Md4;
        for( ; !isInterruptionRequested() && j <= QCryptographicHash::Sha3_512; ++j )
            emit newHashString( i + j, QCryptographicHash::hash( content, static_cast<QCryptographicHash::Algorithm>( j ) ) );

        uint k = QCryptoHash::TIGER;
        for( ; !isInterruptionRequested() && k <= QCryptoHash::RMD160; ++k )
            emit newHashString( i + j + k, QCryptoHash::hash( content, static_cast<QCryptoHash::Algorithm> ( k ) ) );

        uint l = QExtraHash::HAVAL128;
        for( ; !isInterruptionRequested() && l <= QExtraHash::HAVAL256; ++l )
            emit newHashString( i + j + k + l, QExtraHash::hash( content, static_cast<QExtraHash::Algorithm> ( l ) ) );

        if ( !isInterruptionRequested() )
            emit newHashString( i + j + k + l, content.toBase64( QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals ) );
    }
}
