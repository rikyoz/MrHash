#ifndef FILEICONWIDGET_HPP
#define FILEICONWIDGET_HPP

#include "ui_fileiconwidget.h"

#include <QFileInfo>

class FileIconWidget : public QWidget, private Ui::FileIconWidget {
        Q_OBJECT

    public:
        explicit FileIconWidget( QWidget* parent = 0 );
        void loadFileIcon( const QFileInfo& fileInfo );
};

#endif // FILEICONWIDGET_HPP
