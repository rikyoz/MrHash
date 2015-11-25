#-------------------------------------------------
#
# Project created by QtCreator 2010-07-13T15:46:54
#
#-------------------------------------------------

QT       += widgets

TEMPLATE = app

contains(QT_ARCH, i386) {
    PLATFORM = x86
} else {
    PLATFORM = x64
    ARCH_SUFFIX = 64
}

TARGET       =  mrhash$${ARCH_SUFFIX}

VPATH += ./src/          \
         ./include/      \
         ./lib/          \
         ./res/          \
         ./ui/

INCLUDEPATH += ./include/ \
               ./lib/

SOURCES += \
           src/main.cpp               \
           src/mainwindow.cpp         \
           src/about.cpp              \
           src/globalstuff.cpp        \
           src/haval.cpp              \
           src/qextrahash.cpp         \
           src/fileinfowidget.cpp     \
           src/filehashcalculator.cpp \
           src/util.cpp               \
           src/crc.cpp                \
           src/fileiconwidget.cpp


HEADERS  += \
            include/mainwindow.hpp         \
            include/about.hpp              \
            include/haval.hpp              \
            include/qextrahash.hpp         \
            include/hashalgorithm.hpp      \
            include/fileinfowidget.hpp     \
            include/filehashcalculator.hpp \
            include/globalstuff.hpp        \
            include/util.hpp               \
            include/crc.hpp                \
            include/fileiconwidget.hpp

FORMS    += mainwindow.ui about.ui fileinfowidget.ui fileiconwidget.ui

RESOURCES += res/icon.qrc res/translations.qrc

TRANSLATIONS = res/lang/it.ts

########################### CONFIGURATION ############################
CONFIG += c++14
CONFIG(debug, debug|release) {
    BUILD = debug
} else {
    BUILD = release
}
DESTDIR     = $$PWD/bin/$${PLATFORM}/$${BUILD}/
OBJECTS_DIR = ./$${BUILD}/.obj
RCC_DIR     = ./$${BUILD}/.rcc
MOC_DIR     = ./$${BUILD}/.moc
UI_DIR      = ./$${BUILD}/.ui

MAJOR_VER  = 0
MINOR_VER  = 3
PATCH_VER  = 0
VERSION    = $${MAJOR_VER}.$${MINOR_VER}.$${PATCH_VER}
DEFINES   += "MAJOR_VER=$${MAJOR_VER}" "MINOR_VER=$${MINOR_VER}" "PATCH_VER=$${PATCH_VER}"

LIBS += -L$$PWD/lib/qtcryptohash/bin/$${PLATFORM}/$${BUILD}/ -lQtCryptoHash$${ARCH_SUFFIX}
DEFINES += STATIC_LIB
######################## OS DEPENDENT OPTIONS ########################
win32 {
    # CONTENT OF THE RC FILE #
    QMAKE_TARGET_PRODUCT = MrHash
    QMAKE_TARGET_COMPANY = rikyoz
    QMAKE_TARGET_COPYRIGHT = Copyright (c) 2015 $${QMAKE_TARGET_COMPANY}
    RC_ICONS = res/icon.ico
    !win32-g++ {
        contains(QT_ARCH, i386) {
            QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
        } else {
            QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.02
        }
    }
}

unix {
    QMAKE_CXXFLAGS_CXX11 = -std=c++1y
    release: QMAKE_CXXFLAGS += -s
    release: QMAKE_LFLAGS = -s
}
