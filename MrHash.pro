#-------------------------------------------------
#
# Project created by QtCreator 2010-07-13T15:46:54
#
#-------------------------------------------------

QT       += widgets

TEMPLATE = app

contains(QT_ARCH, i386) {
    PLATFORM = x86
    TARGET       =  mrhash
} else {
    PLATFORM = x64
    TARGET       =  mrhash64
}

VPATH += ./src/          \
         ./include/      \
         ./res/          \
         ./ui/

INCLUDEPATH += ./include/

SOURCES += main.cpp mainwindow.cpp about.cpp \
           crc32.cpp globalstuff.cpp tiger.cpp rmd160.cpp haval.cpp \
           qhasher.cpp

HEADERS  += mainwindow.hpp about.hpp \
            crc32.h bithelp.h globalstuff.h tiger.h rmd160.h haval.h err.h \
            qhasher.hpp

FORMS    += mainwindow.ui about.ui

RESOURCES += res/icon.qrc

# Coming soon...
#TRANSLATIONS = t1_it.ts qt_it.ts

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

######################## OS DEPENDENT OPTIONS ########################
win32 {
    # CONTENT OF THE RC FILE #
    QMAKE_TARGET_PRODUCT = MrHash
    QMAKE_TARGET_COMPANY = rikyoz
    QMAKE_TARGET_COPYRIGHT = Copyright (c) 2015 $${QMAKE_TARGET_COMPANY}
    RC_ICONS = res/icon.ico
    RC_LANG  = 0x0410 #Italian Language
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
