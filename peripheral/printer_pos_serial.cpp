#include "printer_pos_serial.h"
#include "qextserialport.h"
#include "trans_u2g.h"
#include <QDebug>

Printer_POS_SERIAL::Printer_POS_SERIAL(QObject *parent) :
    _type(SERIAL), Printer_POS_Virtual(parent)
{
    myCom= new QextSerialPort;
    //定义串口对象，指定串口名和查询模式，这里使用Polling
    myCom->setQueryMode(QextSerialPort::Polling);

    psettings.BaudRate=BAUD115200;
    psettings.DataBits=DATA_8;
    psettings.FlowControl=FLOW_OFF;
    psettings.Parity=PAR_NONE;
    psettings.StopBits=STOP_1;
    psettings.Timeout_Millisec=500;
}

Printer_POS_SERIAL::~Printer_POS_SERIAL()
{
    if(myCom->isOpen())
        myCom->close();
    delete myCom;
}


qint64 Printer_POS_SERIAL::writedirect(const QByteArray &byte)
{
    qDebug() << byte;
    return myCom->write(byte);
}

//in 你好
qint64 Printer_POS_SERIAL::write(const QString &in)
{
    return writedirect(trans_u2g(in));
}

//in ef 02
qint64 Printer_POS_SERIAL::writeHex(const QString &in)
{
    return writedirect(QByteArray::fromHex(QByteArray().append(in)));
}

QString Printer_POS_SERIAL::lastError()
{
    return ErrString;
}

void Printer_POS_SERIAL::close()
{
    myCom->close();
}

bool Printer_POS_SERIAL::devOpen(const QString &in)//open and new
{
    if(myCom->isOpen()) myCom->close();
    //设置端口号
    myCom->setPortName(in);
    //以读写方式打开串口
    if(!myCom ->open(QIODevice::ReadWrite|QIODevice::Unbuffered)){
        ErrString = tr("设备打开失败.没有权限!");
        return false;
    }
    switch(_type){
    case SERIAL:
        //波特率设置
        myCom->setBaudRate(psettings.BaudRate);

        //奇偶校验设置
        myCom->setParity(psettings.Parity);

        //数据位设置
        myCom->setDataBits(psettings.DataBits);

        //停止位设置
        myCom->setStopBits(psettings.StopBits);

        //数据流控制设置
        myCom->setFlowControl(psettings.FlowControl);

        //延时设置
        myCom->setTimeout(psettings.Timeout_Millisec);
        break;
    case USB:break;
    }

    return true;
}

bool Printer_POS_SERIAL::tryOpen(const QString &port)
{
    ErrString.clear();
    if(f_scan_dev_ups() == "1"){
        ErrString=(tr("检测到使用ups,供电暂时不打印小票!"));
        return false;
    }
    return devOpen(port);
}

void Printer_POS_SERIAL::setBaudRate(int type)
{
    myCom->setBaudRate(BaudRateType(type));
}

void Printer_POS_SERIAL::setTimeout(long timeout)
{
    //    For Unix:

    //    Sets the read and write timeouts for the port to \a millisec milliseconds.
    //    Note that this is a per-character timeout, i.e. the port will wait this long for each
    //    individual character, not for the whole read operation.
    myCom->setTimeout(timeout);
}

void Printer_POS_SERIAL::setType(PORT_TYPE type)
{
    _type = type;
}

QStringList Printer_POS_SERIAL::fixedPorts()
{
    return QStringList()
        #ifdef Q_OS_LINUX
            <<("/dev/usb/lp0")
           <<("/dev/usb/lp1")
          <<("/dev/usb/lp2")
         <<("/dev/usb/lp3")
        <<("/dev/ttyS0")
       <<("/dev/ttyS1")
      <<("/dev/ttyS2")
     <<("/dev/ttyS3")
    <<("/dev/ttyS4")
    <<("/dev/ttyS5")
    <<("/dev/ttyS6");
#elif defined (Q_OS_WIN)
            <<("COM0")
           <<("COM1")
          <<("COM2")
         <<("COM3")
        <<("COM4")
       <<("COM5")
      <<("COM6")
     <<("COM7")
    <<("COM8")
    <<("COM9")
    <<("COM10")
    <<("COM11")
    <<("COM12")
    <<("COM13")
    <<("COM14")
    <<("COM15")

  #endif
      ;
}
bool Printer_POS_SERIAL::isOpen()
{return myCom->isOpen();}
QByteArray Printer_POS_SERIAL::readAll()
{return myCom->readAll();}
qint64 Printer_POS_SERIAL::bytesAvailable()
{return myCom->bytesAvailable();}
void Printer_POS_SERIAL::flush()
{myCom->flush();}
qint64 Printer_POS_SERIAL::read(char * data, qint64 maxSize)
{return myCom->read(data, maxSize);}
