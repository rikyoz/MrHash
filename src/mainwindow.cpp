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

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include "mainwindow.hpp"
#include "about.hpp"
#include "qhasher.hpp"

using namespace std;

#define UPPERCASE_SETTING QStringLiteral("show_uppercase")

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ),
                                            settings( "settings.ini", QSettings::IniFormat ) {
    setupUi( this );
    setFixedSize( this->size() );
    setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, this->size(),
                                            qApp->desktop()->availableGeometry() ) );
    setWindowTitle( "MrHash v" + QString::number( MAJOR_VER ) + "." + QString::number( MINOR_VER ) );

    connect( actionAboutQt, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );

    actionShowUppercase->setChecked( settings.value( UPPERCASE_SETTING, false ).toBool() );

    on_plainTextEdit_textChanged();
}

MainWindow::~MainWindow() {}

void MainWindow::closeEvent( QCloseEvent * ) {
    settings.setValue( UPPERCASE_SETTING, actionShowUppercase->isChecked() );
}

void MainWindow::on_actionInformazioni_su_Hasher_triggered() {
    About abt_dlg;
    abt_dlg.exec();
}

void MainWindow::on_actionEsci_triggered() {
    close();
}

void MainWindow::on_actionShowUppercase_toggled( bool ){
    on_plainTextEdit_textChanged();
}

void MainWindow::on_plainTextEdit_textChanged() {
    bool show_uppercase = actionShowUppercase->isChecked();
    QString text = plainTextEdit->toPlainText();
    QByteArray utf_text = text.toUtf8();
    string std_text = text.toStdString();
    crc16edit->setText( QHasher::hash( std_text, show_uppercase, QHashAlgorithm::CRC16 ) );
    crc32edit->setText( QHasher::hash( std_text, show_uppercase, QHashAlgorithm::CRC32 ) );
    crc64edit->setText( QHasher::hash( std_text, show_uppercase, QHashAlgorithm::CRC64 ) );
    md4edit->setText( QHasher::hash( utf_text, show_uppercase, QCryptographicHash::Md4 ) );
    md5edit->setText( QHasher::hash( utf_text, show_uppercase, QCryptographicHash::Md5 ) );
    sha1edit->setText( QHasher::hash( utf_text, show_uppercase, QCryptographicHash::Sha1 ) );
    sha224edit->setText( QHasher::hash( utf_text, show_uppercase, QCryptographicHash::Sha224 ) );
    sha256edit->setText( QHasher::hash( utf_text, show_uppercase, QCryptographicHash::Sha256 ) );
    sha384edit->setText( QHasher::hash( utf_text, show_uppercase, QCryptographicHash::Sha384 ) );
    sha512edit->setText( QHasher::hash( utf_text, show_uppercase, QCryptographicHash::Sha512 ) );
    sha3224edit->setText( QHasher::hash( utf_text, show_uppercase, QCryptographicHash::Sha3_224 ) );
    sha3256edit->setText( QHasher::hash( utf_text, show_uppercase, QCryptographicHash::Sha3_256 ) );
    sha3384edit->setText( QHasher::hash( utf_text, show_uppercase, QCryptographicHash::Sha3_384 ) );
    sha3512edit->setText( QHasher::hash( utf_text, show_uppercase, QCryptographicHash::Sha3_512 ) );
    tigeredit->setText( QHasher::hash( std_text, show_uppercase, QHashAlgorithm::TIGER ) );
    ripemdedit->setText( QHasher::hash( std_text, show_uppercase, QHashAlgorithm::RIPEMD160 ) );
    haval128edit->setText( QHasher::hash( std_text, show_uppercase, QHashAlgorithm::HAVAL128 ) );
    haval160edit->setText( QHasher::hash( std_text, show_uppercase, QHashAlgorithm::HAVAL160 ) );
    haval192edit->setText( QHasher::hash( std_text, show_uppercase, QHashAlgorithm::HAVAL192 ) );
    haval224edit->setText( QHasher::hash( std_text, show_uppercase, QHashAlgorithm::HAVAL224 ) );
    haval256edit->setText( QHasher::hash( std_text, show_uppercase, QHashAlgorithm::HAVAL256 ) );
    base64edit->setText( utf_text.toBase64() );

    sha384edit->setCursorPosition(0);
    sha512edit->setCursorPosition(0);
    sha3384edit->setCursorPosition(0);
    sha3512edit->setCursorPosition(0);
}
