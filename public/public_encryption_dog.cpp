#include "public_encryption_dog.h"
#include <QApplication>
#include <QtDebug>

public_encryption_dog::public_encryption_dog(QWidget *parent) : QWidget(parent)
{

}

bool public_encryption_dog::conductlock()
{
    errsrring.clear();
    //1
#ifdef Q_OS_WIN
    //windows下加密狗
    long nAppID = 1650534242; //应用程序ID, 4字节,通过设号工具设置  注意保密
    long Keys[8]= {0};
    long nKeyNum = 0;

    //QLibrary mylib("NoxApp64.dll");   //声明所用到的dll文件 我的是64位编译器 所以使用64 位dll
    QLibrary mylib(QCoreApplication::applicationDirPath() + "/NoxApp.dll");   //声明所用到的dll文件

    if(mylib.load())
    {
        NoxFind my_NoxFind=(NoxFind)mylib.resolve("NoxFind");
        NoxOpen my_NoxOpen=(NoxOpen)mylib.resolve("NoxOpen");
        NoxGetLastError my_NoxGetLastError=(NoxGetLastError)mylib.resolve("NoxGetLastError");
        NoxReadStorage  my_NoxReadStorage=(NoxReadStorage)mylib.resolve("NoxReadStorage");
        NoxClose my_NoxClose=(NoxClose)mylib.resolve("NoxClose");

        if(!my_NoxFind || !my_NoxOpen || !my_NoxGetLastError || !my_NoxReadStorage || !my_NoxClose)   //保证每个函数导入成功
        {
            errsrring=tr("加密狗错误，导入库错误");
            return false;
            //            return SOFTDOG_ERR_WRONGLIB;
        }

        if(0 !=my_NoxFind(nAppID,Keys,&nKeyNum))
        {
            errsrring=tr("加密狗错误，没有找到加密狗");
            return false;
            //            return SOFTDOG_ERR_NODOG;
        }
        qDebug()<<"There are" <<nKeyNum<<" NOX keys.\n";

        //打开第一只加密锁
        char * userPin = "8c751b20059acb5a"; //用户密码，通过设号工具设置 注意保密
        if( 0 != my_NoxOpen(Keys[0],userPin))
        {
            errsrring=tr("加密狗错误，导入库错误");
            return false;
            //            return SOFTDOG_ERR_WRONGDOG;
        }
        qDebug()<<"Open Key Successfuflly!\n";

        //存储区读(16字节)
        unsigned char readStorage[16] = {0};
        if( 0 != my_NoxReadStorage(Keys[0],readStorage))
        {
            errsrring=tr("加密狗错误，导入库错误");
            return false;
            //            return SOFTDOG_ERR_WRONGDOG;
        }

        //关闭加密锁
        my_NoxClose(Keys[0]);
    }
    else
    {
        errsrring=tr("加密狗错误，没有找到加密狗库文件");
        return false;
        //        return SOFTDOG_ERR_NOLIB;
    }
#endif
    return true;
    //    return SOFTDOG_NOERR;
    //2
}

QString public_encryption_dog::errorString()
{
    return errsrring;
}

