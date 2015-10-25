#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow {
        Q_OBJECT

    public:
        explicit MainWindow( QWidget* parent = 0 );
        ~MainWindow();
        void closeEvent( QCloseEvent* );

    private slots:
        void on_actionEsci_triggered();
        void on_actionInformazioni_su_Hasher_triggered();
        void on_actionShowUppercase_toggled( bool );
        void on_plainTextEdit_textChanged();

    private:
        QSettings settings;
};

#endif // MAINWINDOW_H
