#ifndef CLIENTDISPLAY_NO_H
#define CLIENTDISPLAY_NO_H

//客显 型号未知
//【0】tryOpen(/dev/ttyS0)
//【1】ClientDisplay_NO().writeCom(2, "123.6");
#include <QStringList>
#include <QObject>
class Printer_POS_SERIAL;
#ifdef CLIENTDISPLAY_NO
#define CLIENTDISPLAY_NO_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define CLIENTDISPLAY_NO_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif
class CLIENTDISPLAY_NO_COMMON_DLLSPEC ClientDisplay_NO
{
public:
    explicit ClientDisplay_NO();
    ~ClientDisplay_NO();
    //写串口
    bool writeCom(int sqr, const QString &in, const QString &devpath,const QString &baudrate);
    bool cleanCom(/*const QString &devpath, const QString &baudrate*/);
    QString lastError();
public:
    Printer_POS_SERIAL *printer;
};
#endif // CLIENTDISPLAY_NO_H
