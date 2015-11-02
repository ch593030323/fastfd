#-------------------------------------------------
#
# Project created by QtCreator 2015-09-09T17:58:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = peripheral
TEMPLATE = app
TRANSLATIONS += peripheral.ts


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

include(peripheral.pri)

RESOURCES += \
    peripheral_res.qrc

