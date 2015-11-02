#ifndef PRINTER_POS_VIDPID_H
#define PRINTER_POS_VIDPID_H

#include <QObject>
#include "printer_pos_virtual.h"
#ifdef Q_OS_UNIX
#include <sys/types.h>
#include "libusb-1.0/libusb.h"
#include <linux/fs.h>
#include <stdio.h>
#endif
#include<QStringList>
#ifdef PRINTER_POS_VIDPID
#define PRINTER_POS_VIDPID_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define PRINTER_POS_VIDPID_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif
class PRINTER_POS_VIDPID_COMMON_DLLSPEC Printer_POS_VIDPID : public Printer_POS_Virtual
{
    Q_OBJECT
public:
    explicit Printer_POS_VIDPID(QObject *parent = 0);
    ~Printer_POS_VIDPID();
    virtual qint64 writedirect(const QByteArray & byte);
    virtual qint64 write(const QString &in);//if n have write then n is return; if n == -1 then error
    virtual qint64 writeHex(const QString &in);//if n have write then n is return; if n == -1 then error
    virtual bool tryOpen(const QString &port/*VID&PID*/);
    virtual void close();
    virtual QString lastError();
private:
#ifdef Q_OS_UNIX
    libusb_device_handle *Curdev;//usb
    libusb_device **devs;//usb list
    libusb_context *ctx;//usb context
#endif
    QString otherError;
    bool isopen;

};

#endif // PRINTER_POS_VIDPID_H
