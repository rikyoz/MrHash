#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow {
        Q_OBJECT

    public:
        explicit MainWindow( QWidget* parent = 0 );
        ~MainWindow();

    private slots:
        void on_actionEsci_triggered();
        void on_actionInformazioni_su_Hasher_triggered();
        void on_textEdit_textChanged();
};

#endif // MAINWINDOW_H
