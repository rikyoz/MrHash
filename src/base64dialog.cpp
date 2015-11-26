#include "base64dialog.hpp"

#include <QDesktopWidget>

Base64Dialog::Base64Dialog( QString fileName, QWidget* parent ) : m_filename( fileName ),
    QDialog( parent ) {
    setupUi( this );

    setWindowFlags( Qt::Tool );
    setFixedSize( size() );
    setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, size(),
                                      qApp->desktop()->availableGeometry() ) );

    m_base64calculator.reset( new FileBase64Calculator( this, fileName ) );
    connect( m_base64calculator.get(), SIGNAL( completed( QByteArray ) ), this, SLOT( on_complete( QByteArray ) ) );
    m_base64calculator->start();
}

Base64Dialog::~Base64Dialog() {}

void Base64Dialog::on_complete( QByteArray base64 ) {
    base64edit->setPlainText( base64 );
    progressBar->setVisible( false );
}

void Base64Dialog::closeEvent( QCloseEvent* event ) {
    if ( m_base64calculator != nullptr && m_base64calculator->isRunning() ) {
        event->ignore();
        m_base64calculator->requestInterruption();
        m_base64calculator->wait();
        event->accept();
    }
}
