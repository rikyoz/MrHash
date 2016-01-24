#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include <memory>

#include "crc.hpp"
#include "filehashcalculator.hpp"

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow {
        Q_OBJECT

    public:
        explicit MainWindow( QWidget* parent = 0 );
        virtual ~MainWindow();
        void closeEvent( QCloseEvent* );
        void dragEnterEvent(QDragEnterEvent *event);
        void dropEvent( QDropEvent* event );
        void openFile( QString fileDialog );

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

    private:
        QSettings settings;
        QList< QLineEdit* > hash_edits;
        QMap< QLineEdit*, QString > hash_cache;
        std::unique_ptr< FileHashCalculator > hash_calculator;

        QString boolToStr( bool value );
        void readFileInfo( QString filePath );
        void calculateHashes( QByteArray content, bool show_uppercase );
        void calculateFileHashes( QString fileName );
        void cleanHashEdits();
};

#endif // MAINWINDOW_H
