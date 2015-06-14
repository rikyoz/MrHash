/*
Copyright (c) 2015 rikyoz <rik20@live.it>

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

QString crc16( string msg ) {
    return QString::number( qChecksum( msg.c_str(), qstrlen( msg.c_str() ) ) );
}

QString crc32( string msg ) {
    int crc32_ctx = CRC32::crc32( 0, msg.c_str(), msg.length() );
    //note: right(8) removes the unneeded Fs that appear sometimes at the start of the crc32
    return QString::number( crc32_ctx, 16 ).right( 8 ).toUpper();
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
    crc16edit->setText( crc16( text ) );
    crc32edit->setText( crc32( text ) );
    base64edit->setText( textEdit->toPlainText().toUtf8().toBase64() );
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
    Haval hav;
    haval128edit->setText( QString::fromStdString( hav.calcHaval( text, 128, 5 ) ).toLower() );
    haval160edit->setText( QString::fromStdString( hav.calcHaval( text, 160, 5 ) ).toLower() );
    haval192edit->setText( QString::fromStdString( hav.calcHaval( text, 192, 5 ) ).toLower() );
    haval224edit->setText( QString::fromStdString( hav.calcHaval( text, 224, 5 ) ).toLower() );
    haval256edit->setText( QString::fromStdString( hav.calcHaval( text, 256, 5 ) ).toLower() );
}

void MainWindow::on_actionInformazioni_su_Hasher_triggered() {
    About abt_dlg;
    abt_dlg.exec();
}

void MainWindow::on_actionEsci_triggered() {
    this->close();
}
