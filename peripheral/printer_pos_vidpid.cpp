#include "printer_pos_vidpid.h"
#include "trans_u2g.h"

Printer_POS_VIDPID::Printer_POS_VIDPID(QObject *parent) :
    Printer_POS_Virtual(parent),
    isopen(false)
{
}

Printer_POS_VIDPID::~Printer_POS_VIDPID()
{
    close();
}

qint64 Printer_POS_VIDPID::writedirect(const QByteArray &byte)
{
    if(!isopen)return 0;
    otherError.clear();
    int r;
    int actual;
    QByteArray data=byte;
#ifdef Q_OS_UNIX
    r = libusb_bulk_transfer(Curdev, (0x02 | LIBUSB_ENDPOINT_OUT), (uchar*)data.data(), data.count(), &actual, 200);
    if(r == 0 && actual == data.count()){
        return actual;
    }
    otherError = libusb_error_name(r);
#endif
    return 0;
}

qint64 Printer_POS_VIDPID::write(const QString &in)
{
    return writedirect(trans_u2g(in));
}

qint64 Printer_POS_VIDPID::writeHex(const QString &in)
{
    return writedirect(QByteArray::fromHex(QByteArray().append(in)));
}

bool Printer_POS_VIDPID::tryOpen(const QString &port)
{
    isopen=false;
    otherError.clear();
    if(f_scan_dev_ups() == "1"){
        otherError=(tr("检测到使用ups,供电暂时不打印小票!"));
        return false;
    }
    QStringList vidpid=port.split("&");
    if(vidpid.value(0).isEmpty()
            ||vidpid.value(1).isEmpty()){
        otherError=tr("路径错误");
        return false;
    }
#ifdef Q_OS_UNIX
    Curdev = NULL;
    devs =  NULL;
    ctx=NULL;
    int r;

    //初始化
    if ((r = libusb_init(&ctx)) < 0) {
        otherError="libusb_init failure!";
        return false;
    }
    //启动调试
    libusb_set_debug(ctx, 3);    //确保应用程序不关闭文件描述符 stderr输出。
    //按vid,pid打开
    if((Curdev = libusb_open_device_with_vid_pid(ctx,qint16(vidpid.value(0).toInt(0, 16)),qint16(vidpid.value(1).toInt(0, 16)))) == NULL){//打开失败则 退出 Curdev == NULL
        //qDebug( "libusb_open_device_with_vid_pid = %d",Curdev);                            //该函数 包含了libusb_get_device_list的判断
        otherError="libusb_open_device_with_vid_pid failure!";
        return false;
    }
    //声明前判断
    r = libusb_kernel_driver_active(Curdev, 0);           //判断 该接口是否已经被内核声明
    if (r == 1) {                                                                 //若声明一个已经被内核声明过的接口，则脱离该接口的内核驱动程序
        r = libusb_detach_kernel_driver(Curdev, 0);     //同一次 只会进入一次，及 只会脱离一次
        //qDebug("libusb_detach_kernel_driver = %d\n", r);
    }
    //声明
    if((r=libusb_claim_interface(Curdev,0)) < 0){
        otherError="libusb_claim_interface failure!";
        return false;
    }

    isopen=true;
#endif
    return isopen;
}

void Printer_POS_VIDPID::close()
{
    if(!isopen)return;
#ifdef Q_OS_UNIX
    libusb_release_interface(Curdev,0);
    libusb_close(Curdev); //close the device we opened
    libusb_exit(ctx); //needs to be called to end the
#endif
    isopen=false;
}

QString Printer_POS_VIDPID::lastError()
{
    return otherError;
}
