#-------------------------------------------------
#
# Project created by QtCreator 2015-02-09T15:19:03
#
#-------------------------------------------------

QT       += core gui sql network xml widgets concurrent

TRANSLATIONS += fastfd.ts

TARGET = fastfd

TEMPLATE = app

DESTDIR = .

include(qt/qt.pri)

include(back/back.pri)

include(public/public.pri)

include(peripheral/peripheral.pri)

include(qt_restaurant/qt_restaurant.pri)

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc \

UI_DIR += .

RC_FILE = fastfd.rc
