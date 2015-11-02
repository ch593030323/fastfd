#ifndef PRINTER_POS_SERIAL_H
#define PRINTER_POS_SERIAL_H


//=====================================================================
//【0】这里再次说明，Polling方式是不能使用readyRead()信号的，所以我们需要自己设置定时器，来不断地读取缓冲区的数据。


#include <QObject>
class QextSerialPort;
#include "qextserialport.h"
#include "printer_pos_virtual.h"

#ifdef PRINTER_POS_SERIAL
#define PRINTER_POS_SERIAL_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define PRINTER_POS_SERIAL_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif
/**
exmaple:
            QString path="/dev/usb/lp0";
            Printer_POS_SERIAL printer;
            if(path.contains("usb"))printer.setType(Printer_POS_SERIAL::USB);
            if(printer.tryOpen(path)){//打开USB口
                foreach(pieceIns ins, printIns){
                    printer.writedirect(QByteArray::fromHex(ins.first));
                    printer.writedirect(trans_u2g(ins.second));
                }
                printer.close();
            } else {
                qDebug() << (error=path+printer.lastError());
            }
*/
class PRINTER_POS_SERIAL_COMMON_DLLSPEC Printer_POS_SERIAL:public Printer_POS_Virtual
{
    Q_OBJECT
public:
    enum PORT_TYPE{
        SERIAL,
        USB
    };
    PortSettings psettings;

protected:
    QextSerialPort *myCom;
    PORT_TYPE _type;
    QString ErrString;
    QByteArray receive_data;
    bool flag;

protected:
    bool devOpen(const QString &in);

public:
    Printer_POS_SERIAL(QObject*parent = 0);
    ~Printer_POS_SERIAL();

    bool isOpen();
    /*手动关闭*/

    /*gbk,和十六进制 的读写*/
    virtual qint64 writedirect(const QByteArray & byte);
    virtual qint64 write(const QString &in);//if n have write then n is return; if n == -1 then error
    virtual qint64 writeHex(const QString &in);//if n have write then n is return; if n == -1 then error
    virtual bool tryOpen(const QString &port);
    virtual void close();
    virtual QString lastError();
    QByteArray readAll();


    /*设置波特率，可在接口打开后设置*/
    void setBaudRate(int type);

    /*设置串口读写超时，可在接口打开后设置*/
    void setTimeout(long timeout);

    /*设置打印类型, 在tryopen前设置有效*/
    void setType( PORT_TYPE type);

    /*返回可以访问打数据字节数*/
    qint64 bytesAvailable();
    void flush();
    qint64 read(char * data, qint64 maxSize);

    QStringList fixedPorts();
};
#endif // PRINTER_POS_SERIAL_H
