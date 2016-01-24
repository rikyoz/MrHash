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
#include <QMimeData>
#include <QMessageBox>

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include "qtcryptohash/qcryptohash.hpp"
#include "qextrahash.hpp"
#include "crc.hpp"

#include "mainwindow.hpp"
#include "base64dialog.hpp"
#include "about.hpp"
#include "util.hpp"

using namespace std;

#define MAX_BASE64_FILESIZE 5 * 1024 * 1024 // 5 MB

#define UPPERCASE_SETTING QStringLiteral("show_uppercase")

#ifdef Q_OS_WIN
/* Needed to read correctly the file properties on NTFS file systems,
 * see http://doc.qt.io/qt-5/qfiledevice.html#Permission-enum */
extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
#endif

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ),
    mSettings( "settings.ini", QSettings::IniFormat ) {
    setupUi( this );
#ifdef Q_OS_LINUX
    setFixedHeight( height() + 80 );
    foreach ( QLineEdit* lineEdit, findChildren<QLineEdit*>() ) {
        lineEdit->setFixedHeight( 24 );
    }
#endif
    setFixedSize( this->size() );
    setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, this->size(),
                                      qApp->desktop()->availableGeometry() ) );
    setWindowTitle( "Mr. Hash v" + QString( "%1.%2.%3" ).arg( MAJOR_VER ).arg( MINOR_VER ).arg( PATCH_VER ) );
    setAcceptDrops( true );

    actionUseUppercase->setChecked( mSettings.value( UPPERCASE_SETTING, false ).toBool() );
    fileInfoWidget->setVisible( false );
    closeButton->setVisible( false );
    actionClose->setDisabled( true );
    base64button->setVisible( false );
    base64edit->setEnabled( false );
    progressBar->setVisible( false );
    pauseButton->setVisible( false );
    label_7->setEnabled( false );

    connect( actionAboutQt, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );
    connect( actionOpen, SIGNAL( triggered() ), this, SLOT( on_browseButton_clicked() ) );
    connect( actionClose, SIGNAL( triggered() ), this, SLOT( on_closeButton_clicked() ) );

    mHashEdits.push_front( haval256edit );
    mHashEdits.push_front( haval224edit );
    mHashEdits.push_front( haval192edit );
    mHashEdits.push_front( haval160edit );
    mHashEdits.push_front( haval128edit );
    mHashEdits.push_front( ripemdedit );
    mHashEdits.push_front( tigeredit );
    mHashEdits.push_front( sha3512edit );
    mHashEdits.push_front( sha3384edit );
    mHashEdits.push_front( sha3256edit );
    mHashEdits.push_front( sha3224edit );
    mHashEdits.push_front( sha512edit );
    mHashEdits.push_front( sha384edit );
    mHashEdits.push_front( sha256edit );
    mHashEdits.push_front( sha224edit );
    mHashEdits.push_front( sha1edit );
    mHashEdits.push_front( md5edit );
    mHashEdits.push_front( md4edit );
    mHashEdits.push_front( crc64edit );
    mHashEdits.push_front( crc32edit );
    mHashEdits.push_front( crc16edit );
}

MainWindow::~MainWindow() {}

void MainWindow::closeEvent( QCloseEvent* event ) {
    mSettings.setValue( UPPERCASE_SETTING, actionUseUppercase->isChecked() );
    if ( mHashCalculator != nullptr && mHashCalculator->isRunning() ) {
        event->ignore();
        QMessageBox closeMsg( QMessageBox::Question,
                              tr( "Work in progress..." ),
                              tr( "The calculation hasn't finished. Do you really want to quit?" ),
                              QMessageBox::Yes | QMessageBox::No,
                              this );
        closeMsg.setButtonText( QMessageBox::Yes, tr( "Yes" ) );
        closeMsg.setButtonText( QMessageBox::No, tr( "No" ) );
        closeMsg.exec();
        if ( closeMsg.result() == QMessageBox::Yes ) {
            FileHashCalculator* calculator = mHashCalculator.release();
            if ( calculator->isPaused() ) {
                calculator->resume();
            }
            calculator->stop();
            event->accept();
        }
    }
}

void MainWindow::dragEnterEvent( QDragEnterEvent* event ) {
    event->acceptProposedAction();
}

void MainWindow::dropEvent( QDropEvent* event ) {
    const QMimeData* mimeData = event->mimeData();

    if ( mimeData->hasUrls() ) {
        QList< QUrl > urlList = mimeData->urls();
        if ( urlList.length() > 1 ) {
            QMessageBox::information( this, tr( "Too many files!" ), tr( "Mr. Hash can calculate the hashes of just one file!" ) );
            event->ignore();
        } else {
            tabWidget->setCurrentIndex( 0 );
            openFile( urlList.at( 0 ).toLocalFile() );
        }
    }
}

void MainWindow::on_actionInformazioni_su_Hasher_triggered() {
    About abt_dlg;
    abt_dlg.exec();
}

void MainWindow::on_actionEsci_triggered() {
    close();
}

void MainWindow::on_actionUseUppercase_toggled( bool useUppercase ) {
    foreach ( QLineEdit* lineEdit, findChildren<QLineEdit*>() ) {
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
    QFileDialog fileDialog( this );
    if ( fileDialog.exec() == QFileDialog::Accepted && fileDialog.selectedFiles().size() != 0 ) {
        openFile( fileDialog.selectedFiles()[0] );
    }
}

void MainWindow::on_tabWidget_currentChanged( int index ) {
    base64edit->setText( "" );
    base64edit->setEnabled( index != 0 );
    label_7->setEnabled( index != 0 );
    if ( index != 0 ) { // text tab selected!
        foreach ( QLineEdit* lineEdit, findChildren<QLineEdit*>() ) {
            if ( lineEdit != filePathEdit && lineEdit != base64edit ) {
                mHashCache.insert( lineEdit, lineEdit->text() );
            }
        }
        progressBar->setVisible( false );
        pauseButton->setVisible( false );
        on_plainTextEdit_textChanged();
    } else if ( filePathEdit->text().isEmpty() ) { // no file selected, no hash to show
        cleanHashEdits();
    } else if ( mHashCache.isEmpty() ) { // file selected but calculation not yet started
        progressBar->setVisible( true );
        pauseButton->setVisible( true );
        calculateFileHashes( filePathEdit->text() );
    } else { // a previous file hash calculation was completed (cached)
        foreach ( QLineEdit* lineEdit, mHashCache.keys() ) {
            QString hash_text = mHashCache.value( lineEdit );
            lineEdit->setText( actionUseUppercase->isChecked() ? hash_text.toUpper() : hash_text.toLower() );
        }
        mHashCache.clear();
    }
}

void MainWindow::on_base64button_clicked() {
    QFileInfo fileInfo( filePathEdit->text() );
    if ( fileInfo.size() <= MAX_BASE64_FILESIZE ) {
        Base64Dialog base64dlg( filePathEdit->text(), this );
        base64dlg.exec();
    } else {
        QMessageBox::information( this, "Base64", tr( "Base64 encoding of files with size greater than 5 MB is not allowed!" ) );
    }
}

void MainWindow::on_closeButton_clicked() {
    if ( mHashCalculator != nullptr && mHashCalculator->isRunning() ) {
        QMessageBox closeMsg( QMessageBox::Question,
                              tr( "Close?" ),
                              tr( "The calculation hasn't finished yet. Do you really want to close the file?" ),
                              QMessageBox::Yes | QMessageBox::No,
                              this );
        closeMsg.setButtonText( QMessageBox::Yes, tr( "Yes" ) );
        closeMsg.setButtonText( QMessageBox::No, tr( "No" ) );
        closeMsg.exec();
        if ( closeMsg.result() == QMessageBox::No ) {
            return;
        } else if ( mHashCalculator->isPaused() ) {
            mHashCalculator->resume();
        }
        mHashCalculator->stop();
    }
    tabWidget->tabBar()->setEnabled( true );
    filePathEdit->clear();
    dragDropLabel->setVisible( true );
    fileInfoWidget->setVisible( false );
    closeButton->setVisible( false );
    actionClose->setDisabled( true );
    base64button->setVisible( false );
    pauseButton->setVisible( false );
    progressBar->setVisible( false );
    cleanHashEdits();
}

void MainWindow::on_newHashString( int index, QByteArray hash ) {
    if ( mHashEdits[index] == base64edit )
        mHashEdits[index]->setText( hash );
    else
        mHashEdits[index]->setText( util::hash_hex( hash, actionUseUppercase->isChecked() ) );
    mHashEdits[index]->setCursorPosition( 0 );
}

void MainWindow::on_newChecksumValue( int index, quint64 value ) {
    mHashEdits[index]->setText( util::checksum_hex( value, actionUseUppercase->isChecked() ) );
    mHashEdits[index]->setCursorPosition( 0 );
}

void MainWindow::on_progressUpdate( float progress ) {
    progressBar->setValue( progress * 100 );
}

void MainWindow::on_finished() {
    progressBar->setVisible( false );
    pauseButton->setVisible( false );
    tabWidget->tabBar()->setEnabled( true );
}

void MainWindow::on_pauseButton_clicked() {
    if ( mHashCalculator != nullptr && mHashCalculator->isRunning() ) {
        if ( mHashCalculator->isPaused() ) {
            cleanHashEdits( true );
            pauseButton->setText( tr( "Pause" ) );
            mHashCalculator->resume();
        } else {
            cleanHashEdits( true, tr( "Calculation suspended" ) );
            pauseButton->setText( tr( "Resume" ) );
            mHashCalculator->pause();
        }
    }
}

void MainWindow::cleanHashEdits( bool usePlaceholder, QString placeholder ) {
    foreach ( QLineEdit* lineEdit, findChildren<QLineEdit*>() ) {
        if ( lineEdit != filePathEdit ) {
            lineEdit->setPlaceholderText( usePlaceholder && lineEdit != base64edit ? placeholder : "" );
            lineEdit->clear();
        }
    }
}

void MainWindow::openFile( QString filePath ) {
    dragDropLabel->setVisible( false );

    filePathEdit->setText( QDir::toNativeSeparators( filePath  ) );
    readFileInfo( filePath );

    base64button->setVisible( true );
    progressBar->setVisible( true );
    pauseButton->setVisible( true );
    calculateFileHashes( filePath );
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
    if ( mHashCalculator != nullptr && mHashCalculator->isRunning() ) {
        mHashCalculator->stop();
    }
    tabWidget->tabBar()->setEnabled( false );
    cleanHashEdits( true );
    mHashCalculator.reset( new FileHashCalculator( this, fileName ) );
    connect( mHashCalculator.get(), SIGNAL( newHashString( int, QByteArray ) ), this, SLOT( on_newHashString( int, QByteArray ) ) );
    connect( mHashCalculator.get(), SIGNAL( newChecksumValue( int, quint64 ) ), this, SLOT( on_newChecksumValue( int, quint64 ) ) );
    connect( mHashCalculator.get(), SIGNAL( progressUpdate( float ) ), this, SLOT( on_progressUpdate( float ) ) );
    connect( mHashCalculator.get(), SIGNAL( finished() ), this, SLOT( on_finished() ) );
    mHashCalculator->start();
}

void MainWindow::calculateHashes( QByteArray content, bool show_uppercase ) {
    using namespace util;
    crc16edit->setText( checksum_hex( QChecksum::checksum( content, QChecksum::CRC16 ), show_uppercase ) );
    crc32edit->setText( checksum_hex( QChecksum::checksum( content, QChecksum::CRC32 ), show_uppercase ) );
    crc64edit->setText( checksum_hex( QChecksum::checksum( content, QChecksum::CRC64 ), show_uppercase ) );
    md4edit->setText( hash_hex( QCryptographicHash::hash( content, QCryptographicHash::Md4 ), show_uppercase ) );
    md5edit->setText( hash_hex( QCryptographicHash::hash( content, QCryptographicHash::Md5 ), show_uppercase ) );
    sha1edit->setText( hash_hex( QCryptographicHash::hash( content, QCryptographicHash::Sha1 ), show_uppercase ) );
    sha224edit->setText( hash_hex( QCryptographicHash::hash( content, QCryptographicHash::Sha224 ), show_uppercase ) );
    sha256edit->setText( hash_hex( QCryptographicHash::hash( content, QCryptographicHash::Sha256 ), show_uppercase ) );
    sha384edit->setText( hash_hex( QCryptographicHash::hash( content, QCryptographicHash::Sha384 ), show_uppercase ) );
    sha512edit->setText( hash_hex( QCryptographicHash::hash( content, QCryptographicHash::Sha512 ), show_uppercase ) );
    sha3224edit->setText( hash_hex( QCryptographicHash::hash( content, QCryptographicHash::Sha3_224 ), show_uppercase ) );
    sha3256edit->setText( hash_hex( QCryptographicHash::hash( content, QCryptographicHash::Sha3_256 ), show_uppercase ) );
    sha3384edit->setText( hash_hex( QCryptographicHash::hash( content, QCryptographicHash::Sha3_384 ), show_uppercase ) );
    sha3512edit->setText( hash_hex( QCryptographicHash::hash( content, QCryptographicHash::Sha3_512 ), show_uppercase ) );
    tigeredit->setText( hash_hex( QCryptoHash::hash( content, QCryptoHash::TIGER ), show_uppercase ) );
    ripemdedit->setText( hash_hex( QCryptoHash::hash( content, QCryptoHash::RMD160 ), show_uppercase ) );
    haval128edit->setText( hash_hex( QExtraHash::hash( content, QExtraHash::HAVAL128 ), show_uppercase ) );
    haval160edit->setText( hash_hex( QExtraHash::hash( content, QExtraHash::HAVAL160 ), show_uppercase ) );
    haval192edit->setText( hash_hex( QExtraHash::hash( content, QExtraHash::HAVAL192 ), show_uppercase ) );
    haval224edit->setText( hash_hex( QExtraHash::hash( content, QExtraHash::HAVAL224 ), show_uppercase ) );
    haval256edit->setText( hash_hex( QExtraHash::hash( content, QExtraHash::HAVAL256 ), show_uppercase ) );
    base64edit->setText( content.toBase64( QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals ) );

    sha384edit->setCursorPosition( 0 );
    sha512edit->setCursorPosition( 0 );
    sha3384edit->setCursorPosition( 0 );
    sha3512edit->setCursorPosition( 0 );
}
