#include "qextrahash.hpp"

#include "hashalgorithm.hpp"
#include "haval.hpp"

#include <stdexcept>

QExtraHash::QExtraHash( Algorithm method ) {
    switch ( method ) {
        case QExtraHash::HAVAL128:
            hash_algorithm = new Haval( 128, 5 );
            break;
        case QExtraHash::HAVAL160:
            hash_algorithm = new Haval( 160, 5 );
            break;
        case QExtraHash::HAVAL192:
            hash_algorithm = new Haval( 192, 5 );
            break;
        case QExtraHash::HAVAL224:
            hash_algorithm = new Haval( 224, 5 );
            break;
        case QExtraHash::HAVAL256:
            hash_algorithm = new Haval( 256, 5 );
            break;
        default:
            throw std::invalid_argument( "Algorithm not supported" );
    }
    reset();
}

QExtraHash::~QExtraHash() {
    if ( hash_algorithm ) {
        delete hash_algorithm;
        hash_algorithm = NULL;
    }
}

void QExtraHash::reset() {
    hash_algorithm->init();
}

void QExtraHash::addData( const QByteArray &data ) {
    hash_algorithm->write( reinterpret_cast< const byte* >( data.constData() ), data.length() );
}

QByteArray QExtraHash::result() const {
    byte* hash_array = hash_algorithm->final();
    return QByteArray( reinterpret_cast< char* >( hash_array ), hash_algorithm->hash_length() );
}

QByteArray QExtraHash::hash( const QByteArray &data, QExtraHash::Algorithm method ) {
    QExtraHash hash( method );
    hash.addData( data );
    return hash.result();
}
