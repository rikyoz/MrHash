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

TARGET =  mrhash$${ARCH_SUFFIX}

VPATH += ./src/          \
         ./include/      \
         ./lib/          \
         ./res/          \
         ./ui/

INCLUDEPATH += ./include/ \
               ./lib/ \
               ./third_party/boost-array/include/ \
               ./third_party/boost-assert/include/ \
               ./third_party/boost-config/include/ \
               ./third_party/boost-core/include/ \
               ./third_party/boost-crc/include/ \
               ./third_party/boost-integer/include/ \
               ./third_party/boost-static_assert/include/ \
               ./third_party/boost-throw_exception/include/ \
               ./third_party/boost-type_traits/include/

SOURCES += \
           src/main.cpp               \
           src/mainwindow.cpp         \
           src/about.cpp              \
           src/haval.cpp              \
           src/qextrahash.cpp         \
           src/fileinfowidget.cpp     \
           src/filehashcalculator.cpp \
           src/util.cpp               \
           src/crc.cpp                \
           src/fileiconwidget.cpp     \
           src/base64dialog.cpp       \
           src/filebase64calculator.cpp


HEADERS  += \
            include/mainwindow.hpp         \
            include/about.hpp              \
            include/haval.hpp              \
            include/qextrahash.hpp         \
            include/hashalgorithm.hpp      \
            include/fileinfowidget.hpp     \
            include/filehashcalculator.hpp \
            include/util.hpp               \
            include/crc.hpp                \
            include/fileiconwidget.hpp     \
            include/base64dialog.hpp       \
            include/filebase64calculator.hpp

FORMS    += mainwindow.ui about.ui fileinfowidget.ui fileiconwidget.ui base64dialog.ui

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
PATCH_VER  = 2
VERSION    = $${MAJOR_VER}.$${MINOR_VER}.$${PATCH_VER}
DEFINES   += "MAJOR_VER=$${MAJOR_VER}" "MINOR_VER=$${MINOR_VER}" "PATCH_VER=$${PATCH_VER}"

LIBS += -L$$PWD/lib/qtcryptohash/bin/$${PLATFORM}/$${BUILD}/ -lQtCryptoHash$${ARCH_SUFFIX}
DEFINES += QTCRYPTOHASH_STATIC

######################## OS DEPENDENT OPTIONS ########################
win32 {
    # CONTENT OF THE RC FILE #
    QMAKE_TARGET_PRODUCT = MrHash
    QMAKE_TARGET_COMPANY = rikyoz
    QMAKE_TARGET_COPYRIGHT = Copyright (c) 2016 $${QMAKE_TARGET_COMPANY}
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
