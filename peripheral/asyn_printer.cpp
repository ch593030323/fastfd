#include "asyn_printer.h"
#include "printer_pos_net.h"
#include "printer_pos_serial.h"
#include "qextserialport.h"
#include "commonheader.h"
#include "printer_pos_vidpid.h"
#include <QSettings>
#include "printer_normal_t2us.h"
#include "printer_normal_58.h"
#include "printer_normal_80.h"
#include "printer_normal_kf58.h"
#include "printer_pos_virtual.h"
#include <QEventLoop>
#include <QTimer>
#include <QtCore>
#include "commonheader.h"
#if QT_VERSION > QT_VERSION_CHECK(5,0,0)
#include <QtConcurrent/QtConcurrent>
#endif

QMutex print_thread::mutex;
QMutex asyn_Printer::mutex;
void asyn_Printer_print(const QString &path, const QString &baudrate, int loop, const blockIns &ins)
{
    asyn_Printer::mutex.lock();
    Printer_POS_Virtual *printer=CommonHeader::get_variantprinter(path, CommonHeader::creat_baudrates().key(baudrate));
    if(printer){
        printer->tryOpen(path);
        while(loop--){
            foreach(pieceIns pins,ins){
                printer->writeHex(pins.first);
                printer->write(pins.second);
            }
        }
        printer->close();
        delete printer;
    }
    asyn_Printer::mutex.unlock();
}
asyn_Printer::asyn_Printer(QObject *parent) :
    QObject(parent)
{}

asyn_Printer::~asyn_Printer()
{
}

bool asyn_Printer::toprint(const QString &printtype, const QString &path, const QString &baudrate, const blockIns &ins, int times, bool isblock)
{
    qDebug() << "================"<<printtype;
    blockIns tmpins=ins;
    switch(CommonHeader::creat_normal_printtypes().indexOf(printtype)){
    case 0://58mm热敏
        tmpins=Printer_Normal_58::tran(tmpins);
        break;
    case 1://80mm热敏
        tmpins=Printer_Normal_80::tran(tmpins);
        break;
    case 2://KF58
        tmpins=Printer_Normal_KF58::tran(tmpins);
        break;
    case 3://MINI58
        *(QString *)&path=Printer_Normal_T2US().vidpid;
    }

    //    if(CommonHeader::com_widget_dir==Qt::RightToLeft){
    //        for(int index=0,indexcount=tmpins.count();index<indexcount;index++){
    //            tmpins[index].first="1B6102"+tmpins[index].first;
    //        }
    //    }

    if(path.contains("tty")//串口
            || path.contains("usb")//u口
            || path.contains("&")//vidpid
            || path.contains(":")//网口
            ){
        normal_print(path, baudrate, ins, times,isblock);
    } else {
        ErrString = tr("路径错误!");
    }
    return ErrString.isEmpty();
}

bool asyn_Printer::normal_print(const QString path, const QString &baudrate, const blockIns &ins, int times, bool isblock)
{
    ErrString.clear();
    print_thread *t = new print_thread;
    t->_ins = ins;
    t->_times = times;
    t->_path = path;
    t->_baudrate = baudrate;
    t->printer=0;
    t->printer=CommonHeader::get_variantprinter(path, CommonHeader::creat_baudrates().key(baudrate));

    if(t->printer==0){ErrString="no printer is selected!";return ErrString.isEmpty(); }
    if(isblock){//阻塞
        if(!t->printer->tryOpen(path)){ErrString=t->printer->lastError();return ErrString.isEmpty(); }
        int loop = times;
        while(loop--){
            foreach(pieceIns pins,ins){
                t->printer->writeHex(pins.first);
                t->printer->write(pins.second);
            }
        }
        t->printer->close();
        delete t;
    } else{
        QFuture<void> future = QtConcurrent::run(asyn_Printer_print,
                                                 path,
                                                 baudrate,
                                                 times,
                                                 ins);
        future.waitForFinished();
        //        connect(t,SIGNAL(finished()),t,SLOT(deleteLater()));
        //        QObject().moveToThread(t);
        //        t->start();
    }
    return true;
}

print_thread::~print_thread()
{
    delete printer;
}

void print_thread::run()
{
    //    QEventLoop eloop;
    //    QTimer::singleShot(100, &eloop,SLOT(quit()));
    //    eloop.exec();
    print_thread::mutex.lock();
    if(printer->tryOpen(_path)){
        int loop = _times;
        while(loop--){
            foreach(pieceIns pins,_ins){
                printer->writeHex(pins.first);
                printer->write(pins.second);
            }
        }
        printer->close();
    }
    print_thread::mutex.unlock();
}
