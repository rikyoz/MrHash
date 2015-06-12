/*
Copyright (c) 2010 Debugger20

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

A copy of the GNU General Public License is available at
<http://www.gnu.org/licenses/>.
*/
#include <QCryptographicHash>
#include <QDesktopWidget>

#include "crc32.h"
#include "tiger.h"
#include "rmd160.h"
#include "haval.h"

#include "mainwindow.hpp"
#include "about.hpp"

using namespace std;

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ) {
    setupUi( this );
    this->setFixedSize( this->size() );
    this->setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, this->size(),
                                            qApp->desktop()->availableGeometry() ) );
    this->setWindowTitle( "MrHash v" + QString::number( MAJOR_VER ) + "." + QString::number( MINOR_VER ) );


    connect( actionAboutQt, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );
}

MainWindow::~MainWindow() {}

namespace Hex {
    static string digits = "0123456789abcdef";
    std::string hex( char* bin, int length ) {
        std::string s( length * 2, ' ' );
        for ( int i = 0; i < length; i++ ) {
            s[i * 2] = digits[( bin[i] >> 4 ) & 0xf];
            s[i * 2 + 1] = digits[bin[i] & 0xf];
        }
        return s;
    }
    std::string hex( int bin ) {
        std::string s( sizeof( int ) * 2, ' ' );
        for ( unsigned int i = 0; i < sizeof( int ) * 2; i++ ) {
            s[sizeof( int ) * 2 - 1 - i] = digits[bin & 0xf];
            bin = bin >> 4;
        }
        return s;
    }
}

QString crc32( string msg ) {
    int crc32_ctx = CRC32::crc32( 0, msg.c_str(), msg.length() );
    return QString::fromStdString( Hex::hex( crc32_ctx ) ).toUpper();
}

QString tiger( string msg ) {
    Tiger tigre;
    return QString::fromStdString( tigre.calcTiger( msg ) ).toLower();
}

QString ripemd( string msg ) {
    Rmd160 rmd;
    return QString::fromStdString( rmd.calcRmd160( msg ) ).toLower();
}

void MainWindow::on_textEdit_textChanged() {
    string text = textEdit->toPlainText().toStdString();
    Haval hav;
    QString haval128 = QString::fromStdString( hav.calcHaval( text, 128, 5 ) ).toLower();
    QString haval160 = QString::fromStdString( hav.calcHaval( text, 160, 5 ) ).toLower();
    QString haval192 = QString::fromStdString( hav.calcHaval( text, 192, 5 ) ).toLower();
    QString haval224 = QString::fromStdString( hav.calcHaval( text, 224, 5 ) ).toLower();
    QString haval256 = QString::fromStdString( hav.calcHaval( text, 256, 5 ) ).toLower();
    QString base64 = textEdit->toPlainText().toUtf8().toBase64();
    crc16edit->setText( QString::number( qChecksum( text.c_str(), qstrlen( text.c_str() ) ) ) );
    crc32edit->setText( crc32( text ) );
    md4edit->setText( QCryptographicHash::hash( text.c_str(), QCryptographicHash::Md4 ).toHex() );
    md5edit->setText( QCryptographicHash::hash( text.c_str(), QCryptographicHash::Md5 ).toHex() );
    sha1edit->setText( QCryptographicHash::hash( text.c_str(), QCryptographicHash::Sha1 ).toHex() );
    sha224edit->setText( QCryptographicHash::hash( text.c_str(), QCryptographicHash::Sha224 ).toHex() );
    sha256edit->setText( QCryptographicHash::hash( text.c_str(), QCryptographicHash::Sha256 ).toHex() );
    sha384edit->setText( QCryptographicHash::hash( text.c_str(), QCryptographicHash::Sha384 ).toHex() );
    sha512edit->setText( QCryptographicHash::hash( text.c_str(), QCryptographicHash::Sha512 ).toHex() );
    sha3224edit->setText( QCryptographicHash::hash( text.c_str(), QCryptographicHash::Sha3_224 ).toHex() );
    sha3256edit->setText( QCryptographicHash::hash( text.c_str(), QCryptographicHash::Sha3_256 ).toHex() );
    sha3384edit->setText( QCryptographicHash::hash( text.c_str(), QCryptographicHash::Sha3_384 ).toHex() );
    sha3512edit->setText( QCryptographicHash::hash( text.c_str(), QCryptographicHash::Sha3_512 ).toHex() );
    tigeredit->setText( tiger( text ) );
    ripemdedit->setText( ripemd( text ) );
    haval128edit->setText( haval128 );
    haval160edit->setText( haval160 );
    haval192edit->setText( haval192 );
    haval224edit->setText( haval224 );
    haval256edit->setText( haval256 );
    base64edit->setText( base64 );
}

void MainWindow::on_actionInformazioni_su_Hasher_triggered() {
    About abt_dlg;
    abt_dlg.exec();
}

void MainWindow::on_actionEsci_triggered() {
    this->close();
}
