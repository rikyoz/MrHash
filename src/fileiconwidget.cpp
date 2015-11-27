#include "fileiconwidget.hpp"

#include <QFileIconProvider>

FileIconWidget::FileIconWidget( QWidget* parent ) :
    QWidget( parent ) {
    setupUi( this );
}

void FileIconWidget::loadFileIcon( const QFileInfo &fileInfo ) {
    QFileIconProvider iconProvider;
    QIcon icon = iconProvider.icon( fileInfo );
    QList<QSize> availableSizes = icon.availableSizes();
    if ( availableSizes.contains( QSize( 16, 16 ) ) ) {
        availableSizes.removeOne( QSize( 16, 16 ) );
        icon16->setPixmap( icon.pixmap( QSize( 16, 16 ) ) );
    }
    if ( availableSizes.contains( QSize( 32, 32 ) ) ) {
        availableSizes.removeOne( QSize( 32, 32 ) );
        icon32->setPixmap( icon.pixmap( QSize( 32, 32 ) ) );
    }
    if ( availableSizes.contains( QSize( 48, 48 ) ) ) {
        availableSizes.removeOne( QSize( 48, 48 ) );
        icon48->setPixmap( icon.pixmap( QSize( 48, 48 ) ) );
    }
    for ( const QSize &size : availableSizes ) {
        sizeList->addItem( QString( "%1x%2\n" ).arg( size.width() ).arg( size.height() ) );
    }
}
