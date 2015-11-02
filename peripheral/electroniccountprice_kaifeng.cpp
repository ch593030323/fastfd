#include "electroniccountprice_kaifeng.h"
#include "commonheader.h"
#include "printer_pos_serial.h"
#include <QEventLoop>
#include <QTimer>

ElectronicCountPrice_kaifeng::ElectronicCountPrice_kaifeng(QObject *parent) :
    QObject(parent)
{
    printer = new Printer_POS_SERIAL(this);
    t = new Printer_POS_SERIAL_Thread_scale(printer,this);

    connect(t,SIGNAL(receiveData(QByteArray)),this,SIGNAL(receiveData(QByteArray)));

    QObject o;
    o.moveToThread(t);
}

ElectronicCountPrice_kaifeng::~ElectronicCountPrice_kaifeng()
{
    if(t->isRunning()){
        t->isbreak = true;
        t->wait();
    }
    delete t;

    delete printer;
}

void ElectronicCountPrice_kaifeng::setSendData(const QByteArray &data)
{
    t->senddata = data;
}

QByteArray ElectronicCountPrice_kaifeng::sendData()
{
    return t->senddata;
}

bool ElectronicCountPrice_kaifeng::tryStart_qs(const QString &path, const QString &baudrate, long timeout)
{
    if(path == "NONE") return true;//表示没有启用
    if(!printer->tryOpen(path)) return false;
    printer->setBaudRate(CommonHeader::creat_baudrates().key(baudrate));//默认波特率时115200 ，这儿需要9600
    printer->setTimeout(timeout);

    t->torestart();

    return true;
}

bool ElectronicCountPrice_kaifeng::tryStart_s(const QString &path, const QString &baudrate, long timeout)
{
    if(path == "NONE") return true;//表示没有启用
    if(!printer->tryOpen(path)) return false;
    printer->setBaudRate(CommonHeader::creat_baudrates().key(baudrate));//默认波特率时115200 ，这儿需要9600
    printer->setTimeout(timeout);

    t->torestart();

    return true;
}

void ElectronicCountPrice_kaifeng::stop()
{
    t->tostop();

    printer->close();
}

void ElectronicCountPrice_kaifeng::restart()
{
}

QString ElectronicCountPrice_kaifeng::lastError()
{
    return printer->lastError();
}

void ElectronicCountPrice_kaifeng::setBaudrate(const QString &baudrate)
{
    printer->setBaudRate(baudrate.toInt());
}

//===============================================

void Printer_POS_SERIAL_Thread_scale::run()
{
    _printer->writedirect(senddata);
    int waits=0;
    QEventLoop eventloop;
    forever
    {
        if(isbreak) //手动停止
            break;
        if(_printer->bytesAvailable()>0){
            QByteArray b=_printer->readAll();
            emit this->receiveData(b);
            _printer->writedirect(senddata);
        } else {
            if((waits++)>10){
                waits=0;
                _printer->writedirect(senddata);
            }
        }
        QTimer::singleShot(100, &eventloop, SLOT(quit()));
        eventloop.exec();
    }
}

//一次开始，但是假如后来打senddata 更新的话，会线程失效
void Printer_POS_SERIAL_Thread_scale::tostart()
{
    isbreak = false;
    start();
    //If the thread is already running, this function does nothing.
}

void Printer_POS_SERIAL_Thread_scale::torestart()
{
    tostop();
    tostart();
}

void Printer_POS_SERIAL_Thread_scale::tostop()
{
    isbreak = true;
    wait();
    //This function will return true if the thread has finished.
    //It also returns true if the thread has not been started yet.
    //If time is ULONG_MAX (the default), then the wait will never timeout
}




