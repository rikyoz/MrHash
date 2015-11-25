#include "include/fileinfowidget.hpp"

#include <QDir>
#include <QMimeDatabase>
#include <QDateTime>
#include <QFileIconProvider>

QString humanReadableSize( qint64 size ) {
    QString byteSize =  QString::number( size ) + " bytes";

    if ( size < 1024 ) return byteSize;

    QStringList list = { "KB", "MB", "GB", "TB" };

    QStringListIterator iter( list );
    float fsize = size;
    while ( fsize >= 1024.0 && iter.hasNext() ) {
        iter.next();
        fsize /= 1024.0;
    }
    return QString("%1 %2 (%3)").arg( fsize, 0, 'f', 2 ).arg( iter.peekPrevious() ).arg( byteSize );
}

FileInfoWidget::FileInfoWidget(QWidget *parent) : QWidget(parent) {
    setupUi(this);
}

inline QString FileInfoWidget::boolToStr( bool value ) { return value ? tr("yes") : tr("no"); }


void FileInfoWidget::loadFileInfo( const QFileInfo& fileInfo ) {
    pathLabel->setText( QDir::toNativeSeparators( fileInfo.absolutePath() ) );
    nameLabel->setText( fileInfo.fileName() );
    extLabel->setText( fileInfo.suffix() );

    QMimeDatabase mimeDatabase;
    mimeLabel->setText( mimeDatabase.mimeTypeForFile( fileInfo, QMimeDatabase::MatchExtension ).name() );
    ctypeLabel->setText( mimeDatabase.mimeTypeForFile( fileInfo, QMimeDatabase::MatchContent ).name() );

    sizeLabel->setText( humanReadableSize( fileInfo.size() ) );
    lastReadLabel->setText( fileInfo.lastRead().toString( Qt::DefaultLocaleLongDate ) );
    lastChangeLabel->setText( fileInfo.lastModified().toString( Qt::DefaultLocaleLongDate ) );
    creationLabel->setText( fileInfo.created().toString( Qt::DefaultLocaleLongDate ) );
    hiddenLabel->setText( boolToStr( fileInfo.isHidden() ) );
    readableLabel->setText( boolToStr( fileInfo.isReadable() ) );
    writableLabel->setText( boolToStr( fileInfo.isWritable() ) );
    executableLabel->setText( boolToStr( fileInfo.isExecutable() ) );
    ownerLabel->setText( fileInfo.owner() );

    QFileIconProvider iconProvider;
    QIcon icon = iconProvider.icon( fileInfo );
    fileIconLabel->setPixmap( icon.pixmap( icon.availableSizes().last() ) );
}
