INCLUDEPATH += $$PWD\
     $$PWD/mySerialPortTools\
     $$PWD/include

DEPENDPATH += $$PWD\
    $$PWD/include

QT += network widgets sql

HEADERS += \
    $$PWD/include/trans_u2g.h \
    $$PWD/include/reader_rf.h \
    $$PWD/include/printer_tsc_ttp_244_plus.h \
    $$PWD/include/printer_postek.h \
    $$PWD/include/printer_pos_serial.h \
    $$PWD/include/printer_pos_net.h \
    $$PWD/include/printer_gainscha.h \
    $$PWD/include/printer_argox_pplb.h \
    $$PWD/include/printer_argox_ppla.h \
    $$PWD/include/printer_argox.h \
    $$PWD/include/frmmain.h \
    $$PWD/include/electronicscale_ysheng.h \
    $$PWD/include/electronicscale_djian.h \
    $$PWD/include/electronicscale_dahua.h \
    $$PWD/include/electronicscale_cas.h \
    $$PWD/include/electroniccountprice_kaifeng.h \
    $$PWD/include/electroniccountprice.h \
    $$PWD/include/commonheader.h \
    $$PWD/include/clientdisplay_no.h \
    $$PWD/include/clientdisplay.h \
    $$PWD/include/bmp_info.h \
    $$PWD/include/asyn_printer.h \
    $$PWD/include/printer_normal_kf58.h \
    $$PWD/include/printer_normal_58.h \
    $$PWD/include/printer_normal_80.h \
    $$PWD/include/printer_gainscha_2120tf.h \
    $$PWD/include/barscale_net_set_dialog.h \
    $$PWD/include/barscale_main_dialog.h \
    $$PWD/include/barscale_downdialog.h \
    $$PWD/include/s_mainwindow.h \
    $$PWD/include/s_document.h \
    $$PWD/include/s_commands.h \
    $$PWD/include/labelprint_set_tagbarcode_dialog.h \
    $$PWD/include/labelprint_set_dialog.h \
    $$PWD/include/labelprint_main_dialog.h \
    $$PWD/include/labelprint_goodinfo_select_dialog.h \
    $$PWD/include/labelprint_goodinfo_dialog.h \
    $$PWD/include/labelprint_design_dialog.h \
    $$PWD/include/uinormal_button.h \
    $$PWD/include/uinormal_lineedit.h \
    $$PWD/include/uinormal_commbox.h \
    $$PWD/include/uinormal_message.h \
    $$PWD/include/printer_pos_virtual.h \
    $$PWD/include/printer_pos_vidpid.h \
    $$PWD/include/printer_normal_t2us.h \
    $$PWD/include/electronicscale_kaifeng.h \
    $$PWD/include/printer_virtual.h \
    $$PWD/include/labelprint_goodinfo_add_dialog.h \
    $$PWD/include/labelprint_set_vidpid_dialog.h \
    peripheral/labelprint_design_import_dialog.h


SOURCES += \
    $$PWD/reader_rf.cpp \
    $$PWD/printer_tsc_ttp_244_plus.cpp \
    $$PWD/printer_postek.cpp \
    $$PWD/printer_pos_serial.cpp \
    $$PWD/printer_pos_net.cpp \
    $$PWD/printer_gainscha.cpp \
    $$PWD/printer_argox_pplb.cpp \
    $$PWD/printer_argox_ppla.cpp \
    $$PWD/printer_argox.cpp \
    $$PWD/frmmain.cpp \
    $$PWD/electronicscale_ysheng.cpp \
    $$PWD/electronicscale_djian.cpp \
    $$PWD/electronicscale_dahua.cpp \
    $$PWD/electronicscale_cas.cpp \
    $$PWD/electroniccountprice_kaifeng.cpp \
    $$PWD/electroniccountprice.cpp \
    $$PWD/commonheader.cpp \
    $$PWD/clientdisplay_no.cpp \
    $$PWD/clientdisplay.cpp \
    $$PWD/bmp_info.cpp \
    $$PWD/asyn_printer.cpp \
    $$PWD/printer_normal_kf58.cpp \
    $$PWD/printer_normal_58.cpp \
    $$PWD/printer_normal_80.cpp \
    $$PWD/printer_gainscha_2120tf.cpp \
    $$PWD/barscale_net_set_dialog.cpp \
    $$PWD/barscale_main_dialog.cpp \
    $$PWD/barscale_downdialog.cpp \
    $$PWD/s_mainwindow.cpp \
    $$PWD/s_document.cpp \
    $$PWD/s_commands.cpp \
    $$PWD/labelprint_set_tagbarcode_dialog.cpp \
    $$PWD/labelprint_set_dialog.cpp \
    $$PWD/labelprint_main_dialog.cpp \
    $$PWD/labelprint_goodinfo_select_dialog.cpp \
    $$PWD/labelprint_goodinfo_dialog.cpp \
    $$PWD/labelprint_design_dialog.cpp \
    $$PWD/uinormal_button.cpp \
    $$PWD/uinormal_lineedit.cpp \
    $$PWD/uinormal_commbox.cpp \
    $$PWD/uinormal_message.cpp \
    $$PWD/printer_pos_virtual.cpp \
    $$PWD/printer_pos_vidpid.cpp \
    $$PWD/printer_normal_t2us.cpp \
    $$PWD/electronicscale_kaifeng.cpp \
    $$PWD/printer_virtual.cpp \
    $$PWD/labelprint_goodinfo_add_dialog.cpp \
    $$PWD/labelprint_set_vidpid_dialog.cpp \
    peripheral/labelprint_design_import_dialog.cpp


#串口文件#
include(qextserialport/qextserialport.pri)
#串口文件#

unix {
#Creader#
Debug:INCLUDEPATH += /usr/local/include
LIBS +=-L/usr/local/usb-arm/lib -lusb-1.0
INCLUDEPATH += . /usr/local/usb-arm/include
#Creader#
}

win32 {
DEFINES +=ASYN_PRINTER.H \
    BARSCALE_MAIN_DIALOG.H \
    BMP_INFO.H \
    CLIENTDISPLAY.H \
    CLIENTDISPLAY_NO.H \
    COMMONHEADER.H \
    ELECTRONICCOUNTPRICE.H \
    ELECTRONICCOUNTPRICE_KAIFENG.H \
    ELECTRONICSCALE_CAS.H \
    ELECTRONICSCALE_DAHUA.H \
    ELECTRONICSCALE_DJIAN.H \
    ELECTRONICSCALE_KAIFENG.H \
    ELECTRONICSCALE_YSHENG.H \
    FRMMAIN.H \
    LABELPRINT_MAIN_DIALOG.H \
    PRINTER_ARGOX.H \
    PRINTER_ARGOX_PPLA.H \
    PRINTER_ARGOX_PPLB.H \
    PRINTER_GAINSCHA.H \
    PRINTER_GAINSCHA_2120TF.H \
    PRINTER_NORMAL_58.H \
    PRINTER_NORMAL_80.H \
    PRINTER_NORMAL_KF58.H \
    PRINTER_NORMAL_T2US.H \
    PRINTER_POS_NET.H \
    PRINTER_POS_SERIAL.H \
    PRINTER_POS_VIDPID.H \
    PRINTER_POS_VIRTUAL.H \
    PRINTER_POSTEK.H \
    PRINTER_TSC_TTP_244_PLUS.H \
    READER_RF.H \
    READER_RF.H \
#HEADERS += \
#    $$PWD/win32_printer_argox_pplb.h

#SOURCES += \
#    $$PWD/win32_printer_argox_pplb.cpp

#http://www.libusb.org/wiki/libusb-win32#Installation
#To use libusb-win32 in your own programs include the supplied header file usb.h, and link against the import library (libraries for GCC, BCC, and MSVC 32bit/64bit are available).
#To avoid any version conflicts, DO NOT include the DLL libusb0.dll in your application's directory. The DLL is part of the driver and installed automatically to the Windows system directory.

#window 下面通过libusb来访问usb接口，或者更新的
#1.下载外部包libusb-win32-bin-1.2.6.0.zip
#2.根据不同的系统选择\bin 目录下的那个文件下exe进行安装
#3.安装主要是将系统上usb进行bind，方便libusb进行访问
#3.1安装时，要指定设备对应的pid,vid,以便之后对设备进行访
#4.安装好后，pro里包
#LIBS += -L$$PWD/libusb-win32-bin-1.2.6.0/lib/gcc/ -lusb
#5.软件好后，pro里包含头文件路径
#INCLUDEPATH += $$PWD/libusb-win32-bin-1.2.6.0/include
}

FORMS += \
    $$PWD/barscale_net_set_dialog.ui \
    $$PWD/barscale_net_set_change_dialog.ui \
    $$PWD/barscale_main_dialog.ui \
    $$PWD/barscale_downdialog.ui \
    $$PWD/s_mainwindow.ui \
    $$PWD/labelprint_set_tagbarcode_dialog.ui \
    $$PWD/labelprint_set_dialog.ui \
    $$PWD/labelprint_main_dialog.ui \
    $$PWD/labelprint_goodinfo_select_dialog.ui \
    $$PWD/labelprint_goodinfo_dialog.ui \
    $$PWD/labelprint_design_save_dialog.ui \
    $$PWD/labelprint_design_import_dialog.ui \
    $$PWD/labelprint_design_dialog.ui \
    $$PWD/labelprint_goodinfo_add_dialog.ui \
    $$PWD/popinfo_cannotpop_dialog.ui \
    $$PWD/labelprint_set_vidpid_dialog.ui


RESOURCES += \
    $$PWD/peripheral.qrc
