#include "filebase64calculator.hpp"

#include <QFile>

FileBase64Calculator::FileBase64Calculator( QWidget* parent, QString fileName ) : QThread( parent ),
    mFileName( fileName ) {}

FileBase64Calculator::~FileBase64Calculator() {}

void FileBase64Calculator::run() {
    QFile file( mFileName );
    if ( file.open( QFile::ReadOnly ) ) {
        QByteArray content = file.readAll();
        if ( !isInterruptionRequested() )
            emit completed( content.toBase64( QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals ) );
    }
}
