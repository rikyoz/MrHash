#include "qextrahash.hpp"

#include "hashalgorithm.hpp"
#include "haval.hpp"

#include <stdexcept>

QExtraHash::QExtraHash( Algorithm method ) {
    switch ( method ) {
        case QExtraHash::HAVAL128:
            mHashAlgorithm = new Haval( 128, 5 );
            break;
        case QExtraHash::HAVAL160:
            mHashAlgorithm = new Haval( 160, 5 );
            break;
        case QExtraHash::HAVAL192:
            mHashAlgorithm = new Haval( 192, 5 );
            break;
        case QExtraHash::HAVAL224:
            mHashAlgorithm = new Haval( 224, 5 );
            break;
        case QExtraHash::HAVAL256:
            mHashAlgorithm = new Haval( 256, 5 );
            break;
        default:
            throw std::invalid_argument( "Algorithm not supported" );
    }
    reset();
}

QExtraHash::~QExtraHash() {
    if ( mHashAlgorithm ) {
        delete mHashAlgorithm;
        mHashAlgorithm = NULL;
    }
}

void QExtraHash::reset() {
    mHashAlgorithm->init();
}

void QExtraHash::addData( const QByteArray &data ) {
    mHashAlgorithm->write( reinterpret_cast< const byte* >( data.constData() ), data.length() );
}

QByteArray QExtraHash::result() const {
    byte* hash_array = mHashAlgorithm->final();
    return QByteArray( reinterpret_cast< char* >( hash_array ), mHashAlgorithm->hash_length() );
}

QByteArray QExtraHash::hash( const QByteArray &data, QExtraHash::Algorithm method ) {
    QExtraHash hash( method );
    hash.addData( data );
    return hash.result();
}
