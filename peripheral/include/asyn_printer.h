#ifndef ASYN_PRINTER_H
#define ASYN_PRINTER_H

#include <QObject>
#include <QMutex>
#include <QPair>
#include <QQueue>
#include <QThread>
#include <QFile>
#include <QStringList>
#include "commonheader.h"


class Printer_POS_SERIAL;
class Printer_POS_Virtual;
#ifdef ASYN_PRINTER
#define ASYN_PRINTER_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define ASYN_PRINTER_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif
//异步打印
class ASYN_PRINTER_COMMON_DLLSPEC asyn_Printer : public QObject
{
    Q_OBJECT
public:
    explicit asyn_Printer(QObject *parent = 0);
    ~asyn_Printer();
    bool toprint(const QString &printtype,
                 const QString &path/*/dev/ttyS0*/,
                 const QString &baudrate/*115200*/,
                 const blockIns &ins/*("132", "456")*/,
                 int times = 1/*次数*/,
                 bool isblock=false);
    inline QString lastError(){return ErrString;}
    static QMutex mutex;
private:
    bool normal_print(const QString path, const QString &baudrate, const blockIns &ins, int times, bool isblock=false);
    QString ErrString;
};

class print_thread:public QThread
{
    Q_OBJECT

public:
    print_thread(QObject *parent = 0) : QThread(parent){}
    ~print_thread();
    blockIns _ins;
    int _times;
    QString _path;
    QString _baudrate;
    Printer_POS_Virtual *printer;
    static QMutex mutex;
private:
protected:
    virtual void run();
};
#endif // ASYN_PRINTER_H
