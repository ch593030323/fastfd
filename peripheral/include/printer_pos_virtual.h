#ifndef PRINTER_POS_VIRTUAL_H
#define PRINTER_POS_VIRTUAL_H

#include <QObject>
#ifdef PRINTER_POS_VIRTUAL
#define PRINTER_POS_VIRTUAL_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define PRINTER_POS_VIRTUAL_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif
class PRINTER_POS_VIRTUAL_COMMON_DLLSPEC Printer_POS_Virtual:public QObject
{
        Q_OBJECT
public:
    explicit Printer_POS_Virtual(QObject *parent = 0);

    virtual qint64 writedirect(const QByteArray & byte)=0;
    virtual qint64 write(const QString & byte)=0;

    virtual qint64 writeHex(const QString & byte)=0;

    virtual bool tryOpen(const QString &port)=0;

    virtual void close()=0;

    virtual QString lastError()=0;

};

#endif // PRINTER_POS_VIRTUAL_H
