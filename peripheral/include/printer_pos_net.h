#ifndef PRINTER_POS_NET_H
#define PRINTER_POS_NET_H

#include <QTcpSocket>
#include <QUdpSocket>
#include <QDebug>
#include "printer_pos_virtual.h"

#ifdef PRINTER_POS_NET
#define PRINTER_POS_NET_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define PRINTER_POS_NET_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif

/**
example:
        Printer_POS_NET printer;
        if(printer.tryconnect("192.168.1.201", 9100)){
            foreach(pieceIns ins, printIns){
                printer.writedirect(QByteArray::fromHex(ins.first));
                printer.writedirect(trans_u2g(ins.second));
            }
        } else {
            qDebug() << printer.errorString();
        }
        printer.closeconnect();
*/
class PRINTER_POS_NET_COMMON_DLLSPEC Printer_POS_NET : public Printer_POS_Virtual
{
    Q_OBJECT
public:
    enum TYPE{
        TCP,
        UDP
    };
    Printer_POS_NET(QObject *parent = 0);
    ~Printer_POS_NET();

    bool isOpen();
    QString errorString();
    QString getipport();

    qint64 writedirect(const QByteArray & byte);
    qint64 write(const QString & byte);
    qint64 writeHex(const QString & byte);
    bool tryOpen(const QString &ipport);
    void close();
    QString lastError();

    QAbstractSocket *tcpSocket;
    void resetSocket(TYPE _type);
public slots:
    bool tryconnect(const QString &hostaddress, quint16 port, int msecs = 5000, bool *manual_break = 0);

    bool closeconnect(int msecs = 5000, bool *manual_break = 0);

    bool waitForReadyRead(int msecs = 5000, bool *manual_break = 0);
private:
    QString ErrString;
    QString _ipport;
};

#endif // PRINTER_POS_NET_H
