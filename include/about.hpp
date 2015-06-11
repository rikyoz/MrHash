#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>

#include "ui_about.h"

class About : public QDialog, private Ui::About {
        Q_OBJECT

    public:
        explicit About( QWidget* parent = 0 );
        ~About();
};

#endif // ABOUT_H
