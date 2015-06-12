#include <QDesktopWidget>
#include "about.hpp"

About::About( QWidget* parent ) : QDialog( parent ) {
    setupUi( this );
    this->setFixedSize( this->size() );
    this->setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, this->size(),
                                            qApp->desktop()->availableGeometry() ) );
    tabWidget->setCurrentIndex( 0 );
    versionLabel->setText( QString("v%1.%2.%3").arg(MAJOR_VER).arg(MINOR_VER).arg(PATCH_VER) );
}

About::~About() {}
