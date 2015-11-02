#include "electroniccountprice.h"
#include "trans_u2g.h"
#include "commonheader.h"
#include "electroniccountprice_kaifeng.h"
#include "printer_pos_serial.h"
#include "qextserialport.h"
#include "myhelper.h"

ElectronicCountPrice::ElectronicCountPrice(QObject *parent, const QString &type, const QString &baudrate) :
    QObject(parent), _baudrate(baudrate)
{
    kaifeng = new ElectronicCountPrice_kaifeng(this);
    connect(kaifeng,SIGNAL(receiveData(QByteArray)),this,SLOT(toreceive(QByteArray)));
    setType(type);
}

ElectronicCountPrice::~ElectronicCountPrice()
{
    delete kaifeng;
}

bool ElectronicCountPrice::isOpen()
{
    return kaifeng->printer->isOpen();
}

bool ElectronicCountPrice::tryStart_qs(const QString &path, long timeout)
{
    return kaifeng->tryStart_qs(path,_baudrate, timeout);
}

bool ElectronicCountPrice::tryStart_s(const QString &path, long timeout)
{
    return kaifeng->tryStart_qs(path,_baudrate, timeout);
}

void ElectronicCountPrice::stop()
{
    kaifeng->stop();
}

QString ElectronicCountPrice::lastError()
{
    return kaifeng->lastError();
}

void ElectronicCountPrice::setType(const QString &type)
{
    if(_type != type){
        _type = type;
        switch(CommonHeader::creat_pricecount_scaletypes().indexOf(_type)){
        case 0://凯丰
            setSendData("S");//'S'=QByteArray::fromHex("53")
            break;
        case 1://凯士
            setSendData("");
            break;
        case 2://KF-PC秤
            setSendData("");
            break;
        case 3://顶尖PBX
            setSendData("!0R");
            break;
        case 4://大华ASC-A
            setSendData("P");
            break;
        case 5://顶尖PS1X
            setSendData("W");
            kaifeng->printer->psettings.DataBits=DATA_7;
            kaifeng->printer->psettings.Parity=PAR_EVEN;
            kaifeng->printer->psettings.StopBits=STOP_1;
            break;
        case 6://风华电子称
            setSendData("S");//'S'=QByteArray::fromHex("53")
            break;
        case 7://友中衡计价秤
            setSendData(QByteArray::fromHex("0157300D"));
            break;
        }

    }
}

void ElectronicCountPrice::setBaudrate(const QString &baudrate)
{
    kaifeng->setBaudrate((_baudrate = baudrate));
}

void ElectronicCountPrice::setSendData(const QByteArray &data)
{
    kaifeng->setSendData(data);
}

QString ElectronicCountPrice::jieshi(uchar a)
{
    QString ret;
    switch(a){
    case 0xee:
        ret="0";
        break;
    case 0xfe:
        ret="0.";
        break;
    case 0x0a:
        ret="1";
        break;
    case 0x1a:
        ret="1.";
        break;
    case 0x67:
        ret="2";
        break;
    case 0x77:
        ret="2.";
        break;
    case 0x4f:
        ret="3";
        break;
    case 0x5f:
        ret="3.";
        break;
    case 0x8b:
        ret="4";
        break;
    case 0x9b:
        ret="4.";
        break;
    case 0xcd:
        ret="5";
        break;
    case 0xdd:
        ret="5.";
        break;
    case 0xed:
        ret="6";
        break;
    case 0xfd:
        ret="6.";
        break;
    case 0x0e:
        ret="7";
        break;
    case 0x1e:
        ret="7.";
        break;
    case 0xef:
        ret="8";
        break;
    case 0xff:
        ret="8.";
        break;
    case 0xcf:
        ret="9";
        break;
    case 0xdf:
        ret="9.";
        break;
    case 0xaf:
        ret="A";
        break;
    case 0xe9:
        ret="B";
        break;
    case 0xe4:
        ret="C";
        break;
    case 0x6b:
        ret="D";
        break;
    case 0xe5:
        ret="E";
        break;
    case 0xa5:
        ret="F";
        break;
    case 0x68:
        ret="U";
        break;
    case 0x69:
        ret="O";
        break;
    case 0x01:
        ret="-";
        break;
    case 0x11:
        ret="-.";
        break;
    case 0x29:
        ret="N";
        break;
    case 0xe0:
        ret="L";
        break;
    case 0x41:
        ret="=";
        break;
    case 0x00:
        ret=" ";
        break;
    case 0x10:
        ret=" .";
        break;
    default:
        ret="X";
        break;
    }
    return ret;
}

void ElectronicCountPrice::toreceive(const QByteArray &data)
{
    static bool iscapture = false;
    if(iscapture) return;

    QString ret="0.0000";
    switch(CommonHeader::creat_pricecount_scaletypes().indexOf(_type)){//switch
    case 0: //凯丰
    {//
        float f = data.toFloat() / 1000.0;//千克 小数点后四位
        emit this->weighComming(QString().sprintf("%.4f",f));
        qDebug() << f;
    }
        break;
    case 1: //凯士
    {
        float f = data.mid(2,6).trimmed().toFloat();//千克 小数点后四位
        emit this->weighComming(QString().sprintf("%.4f",f));
    }
        break;
    case 2: //KF-PC秤
    {

        // myHelper::Sleep(100);//延时100毫秒保证接收到的是一条完整的数据,而不是脱节的
        QByteArray temp=data;
        if (temp.size()>=24){
            if (temp[0]==0x04 && temp[1]==0x02 && temp[2]==0x08 && temp[3]==0x20 && temp[20]==0x06&& temp[21]==0x82&& temp[22]==0x48&& temp[23]==0x28 ){

                QString s;
                QString zhongliang;
                QString danjia;
                QString heji;
                for(int i = 3; i < 20; i++){
                    s.sprintf("%02x", (unsigned char)temp.at(i));
                    switch(i){
                    case 4:
                        zhongliang=jieshi(myHelper::StrHexToDecimal(s));
                        if (zhongliang.indexOf(".")>0)
                        {
                            zhongliang=zhongliang.left(1);
                            //  ui->led_qupi->turnOn();
                            emit qupiComming(true);
                        }
                        else
                        {
                            //  ui->led_qupi->turnOff();
                            emit qupiComming(false);
                        }
                        break;
                    case 5:

                        zhongliang+=jieshi(myHelper::StrHexToDecimal(s));
                        break;
                    case 6:

                        zhongliang+=jieshi(myHelper::StrHexToDecimal(s));
                        break;
                    case 7:

                        zhongliang+=jieshi(myHelper::StrHexToDecimal(s));
                        break;
                    case 8:
                        if (jieshi(myHelper::StrHexToDecimal(s)).indexOf(".")>0)
                        {

                            //ui->led_lingwei->turnOn();
                            emit lingweiComming(true);
                            zhongliang+=jieshi(myHelper::StrHexToDecimal(s)).left(1);
                        }
                        else
                        {
                            //ui->led_lingwei->turnOff();
                            emit lingweiComming(false);
                            zhongliang+=jieshi(myHelper::StrHexToDecimal(s));
                        }

                        //ui->label_10->setText(zhongliang);
                        emit weighComming(zhongliang);
                        break;

                    default:
                        break;
                    }
                }
            }
        }


        //        QString cret;
        //        char cc[] = {0x04, 0x02, 0x08, 0x20};//重量标志
        //        if(data.length() == 8
        //                && data.startsWith(QByteArray(cc))){
        //            iscapture = true;
        //            QByteArray call = data.right(4);
        //            foreach(uchar c, call){
        //                cret += KF_PC_TRANS(c);
        //            }
        //            ret=cret;
        //        }
        //        /*if(!cret.isEmpty()) */
        //        emit this->receiveData(ret);
    }
        break;
    case 3://顶尖PBX
    {
        int index0, index1;//head1 索引， head2索引
        char head1,head2,flag1,sign,checksum,tail1,tail2,flag2;
        QByteArray weight,unit;
        head1=0x01;
        head2=0x02;
        tail1=0x03;
        tail2=0x04;
        if((index0=data.indexOf(QByteArray()+head1+head2))>-1){//0x0102固定为0x0102 开头
            if((index1=data.indexOf(QByteArray()+tail1+tail2))>-1){//0x0304固定为0x0304 结束
                QByteArray inter =data.mid(index0, index1-index0 + 3);
                if(inter.count()==16){
                    flag1=inter[2];
                    sign=inter[3];
                    weight=QByteArray()+inter[4]+inter[5]+inter[6]+inter[7]+inter[8]+inter[9];
                    unit=QByteArray()+inter[10] + inter[11];
                    checksum=inter[12];
                    flag2=inter[15];
                    if(flag1=='F'){
                        //重量溢出，或没开机
                        //qDebug() << "重量溢出，或没开机";
                    } else if(flag1=='U'){
                        //重量不稳定
                        //qDebug() << "重量不稳定";
                    } else if(flag1=='S'){
                        //重量稳定
                        //qDebug() << "重量稳定";
                    }
                    if(flag2&0x10){//0001 1100
                        //【1重量为零，重量非零】
                        //qDebug() << "重量为零";
                    } else {
                        //qDebug() << "重量非零";
                    }
                    if(flag2&0x08){
                        //【1去皮模式，非去皮模式】
                        //qDebug() << "去皮模式";
                    }else {
                        //qDebug() << "非去皮模式";
                    }
                    if(flag2&0x04){
                        //【1重量溢出或没开机，0重量正常或已经开机】
                        //qDebug() << "重量溢出或没开机";
                    }else {
                        //qDebug() << "重量正常或已经开机";
                    }
                    if(_2xor(inter.left(12))!=checksum){
                        //奇偶校验失败
                        //qDebug() << "奇偶校验失败";
                    }
                    //qDebug() << weight;
                    if(sign!='-'){
                        float f = weight.toFloat();//千克 小数点后四位
                        ret=QString().sprintf("%.4f",f);
                    }
                }
            }
        }
        emit weighComming(ret);
    }
        break;
    case 4://大华ASC-A
    {
        int index0, index1;
        char head1,head2,head3,tail1,tail2,tail3;
        head1=0x02;//头
        head2=0x20;//称重数据定位
        head3=0x20;//称重数据定位
        tail1=0x20;
        tail2=0x4b;
        tail3=0x47;
        if((index0=data.indexOf(QByteArray()+head1+head2+head3))>-1){//0x0102固定为0x0102 开头
            if((index1=data.indexOf(QByteArray()+tail1+tail2+tail3))>-1){//0x0304固定为0x0304 结束
                QByteArray inter =data.mid(index0, index1-index0);
                inter=inter.mid(3);
                if(inter.count()==5){
                    float f = inter.toFloat();//千克 小数点后四位
                    ret=QString().sprintf("%.4f",f);
                }
            }
        }
        emit weighComming(ret);
    }
        break;
    case 5://顶尖PS1X
    {
        //0.105
        if(data.toHex().startsWith("02")//开头
                &&data.toHex().endsWith("0d")//结尾
                &&data.length() == 7//长度
                ){
            if(data.toHex()=="0230303030300d"){//小于0
            } else {
                qDebug() << "else顶尖PS1X" << data.mid(1, 5).toInt(0, 16);
                float f=data.mid(1, 5).toInt()*1.0/1000.0;
                ret=QString().sprintf("%.4f",f);
                emit weighComming(ret);
            }
        }
    }
    case 6://风华电子称
    {
        //0 1 2     3 4 5
        if(data.length()==6){
            if(data[5]=='+'){
                QByteArray num=data.mid(0, 5);
                float f= num.toInt()*1.0/1000.0;
                ret=QString().sprintf("%.4f",f);
                emit weighComming(ret);
            }
        }
    }
        break;
    case 7://友中衡计价秤
    {
        if(data.endsWith(QByteArray::fromHex("0D"))){
            bool ok=false;
            float f= data.right(11).left(8).toFloat(&ok);
            if(ok){
                ret=QString().sprintf("%.4f",f);
                emit weighComming(ret);
            }
        }
    }
        break;
    }//switch
    //千克 小数点后四位
    iscapture = false;
}

void ElectronicCountPrice::qupi()
{
    switch(CommonHeader::creat_pricecount_scaletypes().indexOf(_type)){//switch
    case 0: //凯丰
        break;
    case 1://凯士PR PLUS
        break;
    case 2://KF-PC秤
        kaifeng->printer->writeHex("8C");
        break;
    case 3://顶尖PBX
        break;
    case 4://大华ASC-A
        break;
    case 5://顶尖PS1X
        break;
    case 6://风华电子称
        kaifeng->printer->writeHex("5A");
        break;
    case 7://友中衡计价秤
        kaifeng->printer->writeHex("0125EF0D");
        break;
    }
}

void ElectronicCountPrice::zhiling()
{
    switch(CommonHeader::creat_pricecount_scaletypes().indexOf(_type)){//switch
    case 0: //凯丰
        break;
    case 1://凯士PR PLUS
        break;
    case 2://KF-PC秤
        kaifeng->printer->writeHex("8D");
        break;
    case 3://顶尖PBX
        break;
    case 4://大华ASC-A
        break;
    case 5://顶尖PS1X
        break;
    case 6://风华电子称
        kaifeng->printer->writeHex("59");
        break;
    case 7://友中衡计价秤
        kaifeng->printer->writeHex("0125FF0D");
        break;
    }
}


