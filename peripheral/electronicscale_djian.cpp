#include "electronicscale_djian.h"
#include "trans_u2g.h"

QMap<int, QString> ElectronicScale_DJIAN::_wtype;
QStringList ElectronicScale_DJIAN::_btype;
QStringList ElectronicScale_DJIAN::_name;

ElectronicScale_DJIAN::ElectronicScale_DJIAN()
{
}

QStringList ElectronicScale_DJIAN::scaleName()
{
    if(_name.isEmpty()){
        _name << QObject::tr("厦门顶尖LS2CX");
    }
    return _name;
}

QMap<int, QString> ElectronicScale_DJIAN::WeigetType()
{
    if(_wtype.isEmpty()){

    }
    return _wtype;
}

QStringList ElectronicScale_DJIAN::BarType()
{
    if(_btype.isEmpty()){
        //13 D部门码 I货号 W重量C校验和P总价
        _btype.append("DDIIIIIPPPPPC");
        _btype.append("DDIIIIIWW.WWWC");//99.999kg
    }
    return _btype;
}
bool ElectronicScale_DJIAN::barcode_scale_resolve(QString &barcode, QString &price, QString &weight, QString &amount, const QString &code, const QString &type)
{
    bool ret = false;
    int index = BarType().indexOf(type);
    switch(index){
    case 0:
        barcode = code.mid(2, 5);
        amount = code.mid(7, 5);
        ret = true;
        break;
    case 1:
        barcode = code.mid(2, 5);
        weight = code.mid(7, 5);
        ret = true;
        break;
    }
    return ret;
}

QString ElectronicScale_DJIAN::barcode_default_port()
{
    QByteArray ins;
    return ins;
}
QByteArray ElectronicScale_DJIAN::clearPLU()
{
    QByteArray ins;
    return ins;
}

QByteArray ElectronicScale_DJIAN::clearPLU_back()
{
    QByteArray ins;
    return ins;
}

QByteArray ElectronicScale_DJIAN::downloadPLUINFO(const QString &plu, const QString &goodsname, const QString &barcode, const QString &price, const QString &wtype, const QString &validate)
{

    //! 生成data
    QByteArray b;
    char start[]={0xCF};
    /*1*/b+=QByteArray(start, sizeof(start))/*起始符*/;
    /*3*/b+=_alignRight_fixedLen_int2bcd(barcode, 3, char(0))/*生鲜号3位*/;
    /*41*/b+=_alignLeft_fixedLen_byte2byte(trans_u2g(goodsname), 40, char(0))/*品名40位*/+char(0)/*1位结束符*/;
    /*4*/b+=_alignRight_fixedLen_int2bcd(QString::number(int(price.toFloat())), 4, char(0))/*单价4位：BCD*/;
    /*1*/b+=char(0x04)/*单位kg*/;
    /*78*/b+=_alignLeft_fixedLen_byte2byte(char(0), 78, char(0));
    qDebug() << b.toHex();
    return b;
}

QByteArray ElectronicScale_DJIAN::downloadPLUINFO_back(const QString &barcode)
{
    char ret[]={0xCF};
    qDebug() << (QByteArray(ret, sizeof(ret))+_alignRight_fixedLen_int2bcd(barcode, 3, char(0))).toHex();
    return QByteArray(ret, sizeof(ret))+_alignRight_fixedLen_int2bcd(barcode, 3, char(0))/*生鲜号3位*/;;
}

QByteArray ElectronicScale_DJIAN::setShopName(const QString &shop_name)
{
    QByteArray ins;
    return ins;
}

QByteArray ElectronicScale_DJIAN::setShopName_back()
{
    QByteArray ins;
    return ins;
}

QByteArray ElectronicScale_DJIAN::setShopPrefix(const QString &pre, const QByteArray &sysparam)
{
    QByteArray ins;
    return ins;
}

QByteArray ElectronicScale_DJIAN::setShopPrefix_back()
{
    QByteArray ins;
    return ins;
}

QByteArray ElectronicScale_DJIAN::getSysParam()
{
    QByteArray ins;
    return ins;
}

QByteArray ElectronicScale_DJIAN::getSysParam_back()
{
    QByteArray ins;
    return ins;
}

QByteArray ElectronicScale_DJIAN::test()
{
    QByteArray ins;
    return ins;
}
