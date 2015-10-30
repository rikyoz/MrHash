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
#include <QFileDialog>
#include <QMimeDatabase>
#include <QDateTime>
#include <QFileIconProvider>

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include "mainwindow.hpp"
#include "about.hpp"

using namespace std;

#define UPPERCASE_SETTING QStringLiteral("show_uppercase")

#ifdef Q_OS_WIN
/* Needed to read correct file properties on NTFS file systems,
 * see http://doc.qt.io/qt-5/qfiledevice.html#Permission-enum */
extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
#endif

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ),
                                            settings( "settings.ini", QSettings::IniFormat ) {
    setupUi( this );
    setFixedSize( this->size() );
    setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, this->size(),
                                            qApp->desktop()->availableGeometry() ) );
    setWindowTitle( "Mr. Hash v" + QString::number( MAJOR_VER ) + "." + QString::number( MINOR_VER ) );

    actionUseUppercase->setChecked( settings.value( UPPERCASE_SETTING, false ).toBool() );
    fileInfoWidget->setVisible( false );
    closeButton->setVisible( false );
    actionClose->setDisabled( true );

    connect( actionAboutQt, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );
    connect( actionOpen, SIGNAL( triggered() ), this, SLOT( on_browseButton_clicked() ) );
    connect( actionClose, SIGNAL( triggered() ), this, SLOT( on_closeButton_clicked() ) );

    hash_edits.push_front( base64edit );
    hash_edits.push_front( haval256edit );
    hash_edits.push_front( haval224edit );
    hash_edits.push_front( haval192edit );
    hash_edits.push_front( haval160edit );
    hash_edits.push_front( haval128edit );
    hash_edits.push_front( ripemdedit );
    hash_edits.push_front( tigeredit );
    hash_edits.push_front( sha3512edit );
    hash_edits.push_front( sha3384edit );
    hash_edits.push_front( sha3256edit );
    hash_edits.push_front( sha3224edit );
    hash_edits.push_front( sha512edit );
    hash_edits.push_front( sha384edit );
    hash_edits.push_front( sha256edit );
    hash_edits.push_front( sha224edit );
    hash_edits.push_front( sha1edit );
    hash_edits.push_front( md5edit );
    hash_edits.push_front( md4edit );
    hash_edits.push_front( crc64edit );
    hash_edits.push_front( crc32edit );
    hash_edits.push_front( crc16edit );
}

MainWindow::~MainWindow() {}

void MainWindow::closeEvent( QCloseEvent *event ) {
    settings.setValue( UPPERCASE_SETTING, actionUseUppercase->isChecked() );
    if ( hash_calculator != nullptr && hash_calculator->isRunning() ) {
        event->ignore();
        FileHashCalculator* calculator = hash_calculator.release();
        calculator->requestInterruption();
        calculator->wait();
        event->accept();
    }
}

void MainWindow::on_actionInformazioni_su_Hasher_triggered() {
    About abt_dlg;
    abt_dlg.exec();
}

void MainWindow::on_actionEsci_triggered() {
    close();
}

void MainWindow::on_actionUseUppercase_toggled( bool useUppercase ){
    foreach( QLineEdit* lineEdit, findChildren<QLineEdit*>() ) {
        if ( lineEdit != filePathEdit && lineEdit != base64edit ) {
            QString text = lineEdit->text();
            lineEdit->setText( useUppercase ? text.toUpper() : text.toLower() );
        }
    }
}

void MainWindow::on_plainTextEdit_textChanged() {
    QString text = plainTextEdit->toPlainText();
    calculateHashes( text.toUtf8(), actionUseUppercase->isChecked() );
}

void MainWindow::on_browseButton_clicked() {
    QFileDialog fileDialog(this);
    if ( fileDialog.exec() == QFileDialog::Accepted ) {
        if ( fileDialog.selectedFiles().size() == 0 ) return;

        filePathEdit->setText( fileDialog.selectedFiles()[0] );
        readFileInfo( fileDialog.selectedFiles()[0] );

        calculateFileHashes( fileDialog.selectedFiles()[0] );
    }
}

void MainWindow::on_tabWidget_currentChanged( int index ) {
    if ( index != 0 )
        on_plainTextEdit_textChanged();
    else if ( filePathEdit->text().isEmpty() ) {
        //no file selected, no hash to show
        cleanHashEdits();
    } else
        calculateFileHashes( filePathEdit->text() );
}

void MainWindow::on_closeButton_clicked() {
    if ( hash_calculator != nullptr && hash_calculator->isRunning() ) {
        hash_calculator->disconnect();
        hash_calculator->requestInterruption();
        hash_calculator->wait();
    }
    filePathEdit->clear();
    fileInfoWidget->setVisible( false );
    closeButton->setVisible( false );
    actionClose->setDisabled( true );
    cleanHashEdits();
}

void MainWindow::on_newHashString( int index, QString hash ) {
    hash_edits[index]->setText( hash );
    hash_edits[index]->setCursorPosition( 0 );
}

void MainWindow::cleanHashEdits() {
    foreach( QLineEdit* lineEdit, findChildren<QLineEdit*>() ) {
        if ( lineEdit != filePathEdit )
            lineEdit->clear();
    }
}

void MainWindow::readFileInfo( QString filePath ) {
#ifdef Q_OS_WIN
    qt_ntfs_permission_lookup++;
#endif

    QFileInfo fileInfo( filePath );
    fileInfoWidget->loadFileInfo( fileInfo );
    fileInfoWidget->setVisible( true );
    closeButton->setVisible( true );
    actionClose->setDisabled( false );

#ifdef Q_OS_WIN
    qt_ntfs_permission_lookup--;
#endif
}

void MainWindow::calculateFileHashes( QString fileName ) {
    foreach( QLineEdit* lineEdit, findChildren<QLineEdit*>() ) {
        if ( lineEdit != filePathEdit ) {
            lineEdit->setText( tr("Calculating...") );
            lineEdit->setCursorPosition( 0 );
        }
    }
    if ( hash_calculator != nullptr && hash_calculator->isRunning() ) {
        hash_calculator->disconnect();
        hash_calculator->requestInterruption();
        hash_calculator->wait();
    }
    hash_calculator.reset( new FileHashCalculator( this, fileName, actionUseUppercase->isChecked() ) );
    connect( hash_calculator.get(), SIGNAL( newHashString(int, QString) ), this, SLOT( on_newHashString(int, QString) ) );
    hash_calculator->start();
}

void MainWindow::calculateHashes( QByteArray content, bool show_uppercase ) {
    crc16edit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::CRC16 ) );
    crc32edit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::CRC32 ) );
    crc64edit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::CRC64 ) );
    md4edit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::MD4 ) );
    md5edit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::MD5 ) );
    sha1edit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::SHA1 ) );
    sha224edit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::SHA224 ) );
    sha256edit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::SHA256 ) );
    sha384edit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::SHA384 ) );
    sha512edit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::SHA512 ) );
    sha3224edit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::SHA3_224 ) );
    sha3256edit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::SHA3_256 ) );
    sha3384edit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::SHA3_384 ) );
    sha3512edit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::SHA3_512 ) );
    tigeredit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::TIGER ) );
    ripemdedit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::RIPEMD160 ) );
    haval128edit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::HAVAL128 ) );
    haval160edit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::HAVAL160 ) );
    haval192edit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::HAVAL192 ) );
    haval224edit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::HAVAL224 ) );
    haval256edit->setText( QHashCalculator::hash( content, show_uppercase, QHashAlgorithm::HAVAL256 ) );
    base64edit->setText( content.toBase64() );

    sha384edit->setCursorPosition(0);
    sha512edit->setCursorPosition(0);
    sha3384edit->setCursorPosition(0);
    sha3512edit->setCursorPosition(0);
}
