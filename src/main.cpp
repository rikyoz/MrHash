#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include "mainwindow.hpp"

int main( int argc, char* argv[] ) {
    QApplication a( argc, argv );

    QTranslator translator;
    translator.load( QLocale::system().name(), ":/lang/" );
    a.installTranslator( &translator );

    MainWindow w;
    w.show();
    return a.exec();
}
