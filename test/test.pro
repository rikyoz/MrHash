#-------------------------------------------------
#
# Project created by QtCreator 2015-10-25T13:43:00
#
#-------------------------------------------------

QT       += testlib
QT       -= gui

TARGET = tst_hash
CONFIG   += console testcase
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += c++14
CONFIG(debug, debug|release) {
    BUILD = debug
} else {
    BUILD = release
}
MOC_DIR     = ./$${BUILD}/.moc

VPATH += ../src/          \
         ../lib/          \
         ../include/

INCLUDEPATH += $$PWD/../include/ \
               $$PWD/../lib/

QMAKE_CXXFLAGS += -I\"$$PWD/../include/\" -I\"$$PWD/../lib/\"

SOURCES += tst_hash.cpp qhasher.cpp tiger.cpp rmd160.cpp haval.cpp globalstuff.cpp
HEADERS += qhasher.hpp tiger.h rmd160.h haval.h globalstuff.h boost/crc.hpp
