#include "base64dialog.hpp"

#include <QDesktopWidget>

Base64Dialog::Base64Dialog( QString fileName, QWidget* parent ) : QDialog( parent ) {
    setupUi( this );

    setWindowFlags( Qt::Tool );
    setFixedSize( size() );
    setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, size(),
                                      qApp->desktop()->availableGeometry() ) );

    mBase64Calculator.reset( new FileBase64Calculator( this, fileName ) );
    connect( mBase64Calculator.get(), SIGNAL( completed( QByteArray ) ), this, SLOT( on_complete( QByteArray ) ) );
    mBase64Calculator->start();
}

Base64Dialog::~Base64Dialog() {}

void Base64Dialog::on_complete( QByteArray base64 ) {
    base64edit->setPlainText( base64 );
    progressBar->setVisible( false );
}

void Base64Dialog::closeEvent( QCloseEvent* event ) {
    if ( mBase64Calculator != nullptr && mBase64Calculator->isRunning() ) {
        event->ignore();
        mBase64Calculator->requestInterruption();
        mBase64Calculator->wait();
        event->accept();
    }
}
