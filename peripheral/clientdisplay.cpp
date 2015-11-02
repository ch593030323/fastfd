#include "clientdisplay.h"
#include "commonheader.h"
#include "clientdisplay_no.h"
#include "printer_pos_serial.h"
ClientDisplay::ClientDisplay(const QString &type, const QString & baudrate) :
    _type(type),_baudrate(baudrate)
{
    display = new ClientDisplay_NO;
}

ClientDisplay::~ClientDisplay()
{
    delete display;
}

bool ClientDisplay::writeCom(int sqr, const QString &in, const QString &devpath, const QString &name)
{    
    bool f=false;
    switch(CommonHeader::creat_clientdisplay_ledtypes().indexOf(_type)){
    case 0://LED-8
        f=display->writeCom(sqr,in,devpath,"2400"/*_baudrate*/);
        break;
    case 1://VFD220
    {
        Printer_POS_SERIAL printer;
        if((f=printer.tryOpen(devpath))){
            printer.setBaudRate(9600/*CommonHeader::creat_baudrates().key(_baudrate)*/); //波特率必须时9600 才可以通信
            printer.writeHex("0C");//22. 清除屏幕的所有字符

            switch(sqr){
            case 1://单价 PRICE
                printer.writedirect(QByteArray().append(name));
                printer.writeHex("1F240102");//12.  x  y光标移到指定的位置 [1FH][24H]x  y;1H<=x<=14H，01H<=y<=02H
                printer.writedirect(QByteArray("PRICE:").append(QString().sprintf("%.02f",in.toDouble())));
                break;
            case 2://总计 TATOL
                printer.writedirect(QByteArray().append(name));
                printer.writeHex("1F240102");//12.  x  y光标移到指定的位置 [1FH][24H]x  y;1H<=x<=14H，01H<=y<=02H
                printer.writedirect(QByteArray("TATOL:").append(QString().sprintf("%.02f",in.toDouble())));
                break;
            case 3://收款 CASH
                printer.writedirect(QByteArray().append(name));
                printer.writeHex("1F240102");//12.  x  y光标移到指定的位置 [1FH][24H]x  y;1H<=x<=14H，01H<=y<=02H
                printer.writedirect(QByteArray("CASH:").append(QString().sprintf("%.02f",in.toDouble())));
                break;
            case 4://找零 CHANGE
                printer.writedirect(QByteArray().append(name));
                printer.writeHex("1F240102");//12.  x  y光标移到指定的位置 [1FH][24H]x  y;1H<=x<=14H，01H<=y<=02H
                printer.writedirect(QByteArray("CHANGE:").append(QString().sprintf("%.02f",in.toDouble())));
                break;
            }
        }
    }
        break;
    }

    return f;
}

bool ClientDisplay::cleanCom(const QString &devpath, const QString &baudrate)
{
    bool f=false;
    switch(CommonHeader::creat_clientdisplay_ledtypes().indexOf(_type)){
    case 0://LED-8
        f=display->cleanCom();

        break;
    case 1://VFD220
    {
        Printer_POS_SERIAL printer;
        if((f=printer.tryOpen(devpath))){
            printer.setBaudRate(CommonHeader::creat_baudrates().key(_baudrate)); //波特率必须时9600 才可以通信
            printer.writeHex("0C");//22. 清除屏幕的所有字符
        }
    }

        break;
    }

    return f;
}

bool ClientDisplay::startShowinfo(const QString &text, const QString &devpath)
{
    bool f=false;
    switch(CommonHeader::creat_clientdisplay_ledtypes().indexOf(_type)){
    case 0://LED-8
        f=true;
        break;
    case 1://VFD220
    {
        Printer_POS_SERIAL printer;
        if((f=printer.tryOpen(devpath))){
            printer.setBaudRate(9600/*CommonHeader::creat_baudrates().key(_baudrate)*/); //波特率必须时9600 才可以通信
            printer.writeHex("0C");//22. 清除屏幕的所有字符
            printer.writedirect(QByteArray().append(text));
        }
    }
        break;
    }

    return f;
}

QString ClientDisplay::lastError()
{
    return display->lastError();
}
