#include "clientdisplay_no.h"
#include "asyn_printer.h"
#include "commonheader.h"
#include "printer_pos_serial.h"

static char send_content[] = {0x1b, 0x51, 0x41};
static char send_sqr[] = {0x1b,0x73};
ClientDisplay_NO::ClientDisplay_NO()
{
    printer = new Printer_POS_SERIAL;
}

ClientDisplay_NO::~ClientDisplay_NO()
{
    delete printer;
}



bool ClientDisplay_NO::writeCom(int sqr, const QString &i, const QString &devpath,const QString &baudrate)
{
    if(!printer->tryOpen(devpath)){
        return false;
    }
    printer->setBaudRate(CommonHeader::creat_baudrates().key(baudrate)); //波特率必须时2400 才可以通信
    QByteArray in;
    QByteArray s1,s2;
    in.append(i);
    //1 2 3 4
    //小于零的按零算 ，大于4的按五算 ，其他为1 2 3 4 ，0 5 为清空，及没有灯点亮
    sqr = qMax(0,qMin(5,sqr));//sqr < 0? 0 : sqr > 4 ?  5 : sqr;
    char m[] = {0x1b, 0x51, 0x41};
    s1 = QByteArray(m, sizeof(m)) + in + (char)0x0d;
    //5. ESC  Q  A  d1d2d3…dn  CR送显示数据命令
    printer->writedirect(s1);
    //6. ESC  s  n设置 “单价”、“总计”、“收款”、“找零”字符显示状态命令
    char m1[] = {0x1b,0x73};
    s2 = QByteArray(m1, sizeof(m1)) + (char)(0x30 + sqr);
    printer->writedirect(s2);
    return true;
}

bool ClientDisplay_NO::cleanCom()
{
    char m[] = { 0x1b, 0x40 };
    printer->writedirect(QByteArray(m, sizeof(m)));
    return true;
}
//bool ClientDisplay_NO::writeCom(int sqr, const QString &i, const QString &devpath,const QString &baudrate)
//{
//    //5. ESC  Q  A  d1d2d3…dn  CR送显示数据命令
//    //6. ESC  s  n设置 “单价”、“总计”、“收款”、“找零”字符显示状态命令

//    QByteArray s_content = QByteArray(send_content, sizeof(send_content))
//            + QByteArray().append(i) + char(0x0d);
//    QByteArray s_sqr = QByteArray(send_sqr, sizeof(send_sqr))
//            + char(0x30 + sqr);
//    blockIns ins;
//    ins << pieceIns("", s_content);
//    ins << pieceIns("", s_sqr);

//    return printer.toprint(devpath, baudrate, ins);
//}

//bool ClientDisplay_NO::cleanCom(const QString &devpath,const QString &baudrate)
//{
//    char m[] = { 0x1b, 0x40 };
//    blockIns ins;
//    ins << pieceIns("", QByteArray(m, sizeof(m)));

//    return printer.toprint(devpath, baudrate, ins);
//}

QString ClientDisplay_NO::lastError()
{
    return printer->lastError();
}
