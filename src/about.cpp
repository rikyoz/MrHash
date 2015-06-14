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
#include <QDesktopWidget>

#include "about.hpp"

About::About( QWidget* parent ) : QDialog( parent ) {
    setupUi( this );
    this->setFixedSize( this->size() );
    this->setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, this->size(),
                                            qApp->desktop()->availableGeometry() ) );
    tabWidget->setCurrentIndex( 0 );
    versionLabel->setText( QString("v%1.%2.%3").arg(MAJOR_VER).arg(MINOR_VER).arg(PATCH_VER) );
}

About::~About() {}
