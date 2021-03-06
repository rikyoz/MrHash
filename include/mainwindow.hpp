#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include <memory>

#include "filehashcalculator.hpp"

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow {
        Q_OBJECT

    public:
        explicit MainWindow( QWidget* parent = 0 );
        virtual ~MainWindow();
        void closeEvent( QCloseEvent* );
        void dragEnterEvent( QDragEnterEvent* event );
        void dropEvent( QDropEvent* event );

    private slots:
        void on_actionInformazioni_su_Hasher_triggered();
        void on_actionEsci_triggered();
        void on_actionUseUppercase_toggled( bool useUppercase );
        void on_plainTextEdit_textChanged();
        void on_browseButton_clicked();
        void on_tabWidget_currentChanged( int index );
        void on_closeButton_clicked();
        void on_base64button_clicked();

        void on_newHashString( int index, QByteArray hash );
        void on_newChecksumValue( int index, quint64 value );
        void on_progressUpdate( float progress );
        void on_finished();
        void on_pauseButton_clicked();

    private:
        QSettings mSettings;
        QList< QLineEdit* > mHashEdits;
        QMap< QLineEdit*, QString > mHashCache;
        std::unique_ptr< FileHashCalculator > mHashCalculator;

        void openFile( QString filePath );
        void readFileInfo( QString filePath );
        void calculateHashes( QByteArray content, bool show_uppercase );
        void calculateFileHashes( QString fileName );
        void cleanHashEdits( bool usePlaceholder = false , QString placeholder = tr( "Calculating..." ) );
};

#endif // MAINWINDOW_H
