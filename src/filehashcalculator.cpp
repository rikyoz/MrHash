#include "filehashcalculator.hpp"

#include <QFile>

#include "qhasher.hpp"

FileHashCalculator::FileHashCalculator(QWidget* parent, QString fileName, bool uppercase) : QThread( parent ),
    file_name( fileName ), use_uppercase( uppercase ) {
}

FileHashCalculator::~FileHashCalculator() {}

void FileHashCalculator::run() {
    QList< QHashCalculator* > hash_calculators = { &QHashAlgorithm::CRC16, &QHashAlgorithm::CRC32, &QHashAlgorithm::CRC64,
                                                     &QHashAlgorithm::MD4, &QHashAlgorithm::MD5, &QHashAlgorithm::SHA1,
                                                     &QHashAlgorithm::SHA224, &QHashAlgorithm::SHA256, &QHashAlgorithm::SHA384,
                                                     &QHashAlgorithm::SHA512, &QHashAlgorithm::SHA3_224,
                                                     &QHashAlgorithm::SHA3_256, &QHashAlgorithm::SHA3_384,
                                                     &QHashAlgorithm::SHA3_512, &QHashAlgorithm::TIGER,
                                                     &QHashAlgorithm::RIPEMD160, &QHashAlgorithm::HAVAL128,
                                                     &QHashAlgorithm::HAVAL160, &QHashAlgorithm::HAVAL192,
                                                     &QHashAlgorithm::HAVAL224, &QHashAlgorithm::HAVAL256 };
    QFile file( file_name );
    if ( file.open( QFile::ReadOnly ) ) {
        QByteArray content = file.readAll(); //TODO: Optimize file read and hash calculation!
        int i = 0;
        for( ; !isInterruptionRequested() && i < hash_calculators.length(); ++i ) {
            emit newHashString( i, QHashCalculator::hash( content, use_uppercase, *hash_calculators[i] ) );
        }
        if ( !isInterruptionRequested() )
            emit newHashString( i, content.toBase64() );
    }
}
