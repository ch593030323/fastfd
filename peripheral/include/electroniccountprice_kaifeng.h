#ifndef ELECTRONICCOUNTPRICE_KAIFENG_H
#define ELECTRONICCOUNTPRICE_KAIFENG_H

//【0】 取重范围：0～15000g
#include <QThread>
class Printer_POS_SERIAL;
#ifdef ELECTRONICCOUNTPRICE_KAIFENG
#define ELECTRONICCOUNTPRICE_KAIFENG_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define ELECTRONICCOUNTPRICE_KAIFENG_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif

class Printer_POS_SERIAL_Thread_scale;
class ELECTRONICCOUNTPRICE_KAIFENG_COMMON_DLLSPEC ElectronicCountPrice_kaifeng : public QObject
{
    Q_OBJECT
public:
    explicit ElectronicCountPrice_kaifeng(QObject *parent = 0);
    ~ElectronicCountPrice_kaifeng();
    void setSendData(const QByteArray &data);
    QByteArray sendData();
    bool tryStart_qs(const QString &path, const QString &baudrate, long timeout = 100); //尝试计价秤开始读数据，动态模式question answer【应答型】
    bool tryStart_s(const QString &path, const QString &baudrate, long timeout = 100);//暂时和应答模式一致

    void stop();//停止，1.会暂停线程，2.和计时器，3.打开打串口也会关闭
    void restart();
    QString lastError();

    void setBaudrate(const QString &baudrate);
signals:
    void receiveData(const QByteArray &data);
public:
    Printer_POS_SERIAL *printer;
    Printer_POS_SERIAL_Thread_scale *t;
    
};
class  Printer_POS_SERIAL_Thread_scale:public QThread
{
    Q_OBJECT
public:
    enum MODEL{
        QUESTION_ANSWER,
        ANSWER
    };

    Printer_POS_SERIAL_Thread_scale(Printer_POS_SERIAL *printer, QObject *parent = 0)
        : _printer(printer), QThread(parent), isbreak(false){}
    bool isbreak;
    QByteArray senddata;

    void tostart();
    void torestart();
    void tostop();
signals:
    void receiveData(const QByteArray &data);
protected:
    virtual void run();
private:
    Printer_POS_SERIAL *_printer;
};
#endif // ELECTRONICCOUNTPRICE_KAIFENG_H
