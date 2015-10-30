#ifndef FILEINFOWIDGET_H
#define FILEINFOWIDGET_H

#include "ui_fileinfowidget.h"

#include <QFileInfo>

class FileInfoWidget : public QWidget, private Ui::FileInfoWidget {
        Q_OBJECT

        QString boolToStr( bool value );

    public:
        explicit FileInfoWidget( QWidget *parent = 0 );
        void loadFileInfo( const QFileInfo& fileInfo );
};

#endif // FILEINFOWIDGET_H
