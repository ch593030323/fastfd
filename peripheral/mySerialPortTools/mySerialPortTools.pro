#-------------------------------------------------
#
# Project created by QtCreator 2011-12-22T22:11:03
#
#-------------------------------------------------

QT       += core gui

TARGET = mySerialPortTools
TEMPLATE = app


SOURCES += main.cpp\
        qextserialport.cpp\
        frmmain.cpp \
    hled.cpp

HEADERS += frmmain.h \
        qextserialport_global.h \
        qextserialport.h \
        myhelper.h \
    hled.h

win32 { SOURCES += qextserialport_win.cpp }
unix { SOURCES += qextserialport_unix.cpp }

FORMS += frmmain.ui

RESOURCES += \
    main.qrc



win32:RC_FILE=main.rc

CONFIG += warn_off      #关闭警告
