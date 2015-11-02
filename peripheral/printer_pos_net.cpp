#include "printer_pos_net.h"
#include <QTcpSocket>
#include <QStringList>
#include <QHostAddress>
#include "trans_u2g.h"
#include <QDebug>
Printer_POS_NET::Printer_POS_NET(QObject *parent):
    Printer_POS_Virtual(parent)
{
    tcpSocket = new QTcpSocket(this);
}

Printer_POS_NET::~Printer_POS_NET()
{
    closeconnect();
    delete tcpSocket;
}

bool Printer_POS_NET::tryconnect(const QString &hostaddress, quint16 port, int msecs, bool *manual_break)
{
    ErrString.clear();
    if(f_scan_dev_ups() == "1"){
        ErrString=(tr("检测到使用ups,供电暂时不打印小票!"));
        return false;
    }

    //manual_break优先级最高
    if(manual_break && *manual_break) return false;

    //连接服务器
    int step = 1000;
    int loopcount = msecs / step;
    qDebug() << "tryconnect" << hostaddress << port;
    while(loopcount --){
        if(manual_break && *manual_break) return false;
        tcpSocket->connectToHost(QHostAddress(hostaddress),port);
        if(tcpSocket->state() == QAbstractSocket::ConnectedState
                || tcpSocket->waitForConnected(step)){

            qDebug() << "connect success!" <<  tcpSocket->peerAddress().toString();
            return true;
        }
    }
    qDebug() << "connect failure!" << tcpSocket->errorString();
    return false;
}

bool Printer_POS_NET::closeconnect(int msecs, bool *manual_break)
{
    //manual_break优先级最高
    if(manual_break && *manual_break) return false;
    //若未连接
    if(tcpSocket->state() == QAbstractSocket::UnconnectedState)
        return true;
    //断开服务器
    int step = 1000;
    int loopcount = msecs / step;
    while(loopcount --){
        if(manual_break && *manual_break) return false;
        tcpSocket->disconnectFromHost();
        if (tcpSocket->state() == QAbstractSocket::UnconnectedState
                || tcpSocket->waitForDisconnected(step)){
            qDebug() << "disconnect success!";
            return true;
        }
    }
    qDebug() << tcpSocket->errorString();
    return false;
}

bool Printer_POS_NET::waitForReadyRead(int msecs, bool *manual_break)
{
    //manual_break优先级最高
    if(manual_break && *manual_break) return false;

    int step = 1000;
    int loopcount = msecs / step;
    while(loopcount --){
        if(manual_break && *manual_break) return false;
        if(tcpSocket->waitForReadyRead(step)){
            return true;
        }
    }
    qDebug() << "time out "<<tcpSocket->peerAddress() << tcpSocket->errorString();
    return false;
}

bool Printer_POS_NET::isOpen()
{
    return tcpSocket->state() == QAbstractSocket::ConnectedState;
}

QString Printer_POS_NET::errorString()
{
    return ErrString.isEmpty()?tcpSocket->errorString():ErrString;
}

QString Printer_POS_NET::getipport()
{
    return _ipport;
}

qint64 Printer_POS_NET::writedirect(const QByteArray &byte)
{
    return tcpSocket->write(byte);
}

qint64 Printer_POS_NET::write(const QString &byte)
{
    return writedirect(trans_u2g(byte));
}

qint64 Printer_POS_NET::writeHex(const QString &byte)
{
    return writedirect(QByteArray::fromHex(byte.toLocal8Bit()));
}

bool Printer_POS_NET::tryOpen(const QString &ipport)
{
    _ipport=ipport;
    QStringList pport=ipport.split(":");
    return tryconnect(pport.value(0), pport.value(1).toInt());
}

void Printer_POS_NET::close()
{
    tcpSocket->waitForBytesWritten(5000);
    closeconnect();
}

QString Printer_POS_NET::lastError()
{
    return errorString();
}

void Printer_POS_NET::resetSocket(Printer_POS_NET::TYPE _type)
{
    delete tcpSocket;
    tcpSocket = 0;
    switch(_type){
    case TCP:
        tcpSocket = new QTcpSocket(this);
        break;
    case UDP:
        tcpSocket = new QUdpSocket(this);
        break;
    }
}

