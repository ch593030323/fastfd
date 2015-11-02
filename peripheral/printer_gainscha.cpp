#include "printer_gainscha.h"

//SOUND 3,200 蜂鸣 3次，每次间隔200ms
/**
Barcode type
128
128M
EAN128
25
25C

39
39C
93
EAN13
EAN13+2

EAN13+5
EAN8
EAN8+2
EAN8+5
CODA

POST
UPCA
UPCA+2
UPCA+5
UPCE

UPCE+2
UPCE+5
CPOST
MSI
MSIC

PLESSY
ITF14
EAN14
*/
static QMap<QString, QString> textfont;
static QMap<QString, QString> onebartype;
static QMap<QString, QString> twobartype;

Printer_GAINSCHA::Printer_GAINSCHA()
{
    //        speical_PRINTSIZE.insert("40x30", QSize(42.48, 32.48));
}

QMap<QString, QSizeF> Printer_GAINSCHA::get_speical_PRINTSIZE()
{
    QMap<QString, QSizeF> printsize;
    printsize.insert("40x30", QSizeF(42.48, 32.48));
    return  printsize;
}

QMap<QString, QString> Printer_GAINSCHA::oneBarType_of_GAINSCHA()
{
    if(onebartype.isEmpty()){
        onebartype.insert("Code 128 AUTO", "128");
        onebartype.insert("Code 128 MANUAL", "128M");
        onebartype.insert("Code 128 AUTO", "EAN128");
        onebartype.insert("Interleaved 2 of 5", "25");
        onebartype.insert("Interleaved 2 of 5 with check digital", "25C");

        onebartype.insert("Code 39", "39");
        onebartype.insert("Code 39 with check digital", "39C");
        onebartype.insert("93", "93");
        onebartype.insert("EAN 13", "EAN13");
        onebartype.insert("EAN 13 with 2 digits add-on", "EAN13+2");

        onebartype.insert("EAN 13 with 5 digits add-on", "EAN13+5");
        onebartype.insert("EAN 8", "EAN8");
        onebartype.insert("EAN 8 with 2 digits add-on", "EAN8+2");
        onebartype.insert("EAN 8 with 5 digits add-on", "EAN8+5");
        onebartype.insert("Codabar", "CODA");

        onebartype.insert("Postnet", "CODA");
        onebartype.insert("UPC-A", "UPCA");
        onebartype.insert("UPC-A with 2 digits add-on", "UPCA+2");
        onebartype.insert("UPC-A with 5 digits add-on", "UPCA+5");
        onebartype.insert("UPC-E", "UPCE");

        onebartype.insert("UPC-E with 2 digits add-on", "UPCE+2");
        onebartype.insert("UPC-E with 5 digits add-on", "UPCE+5");
        onebartype.insert("China post code", "CPOST");
        onebartype.insert("MSI code", "MSI");
        onebartype.insert("MSI code with check digital", "MSIC");

        onebartype.insert("PLESSEY code", "PLESSY");
        onebartype.insert("ITF 14 code", "ITF14");
        onebartype.insert("EAN 14 code", "EAN14");

    }
    return onebartype;
}

QMap<QString, QString> Printer_GAINSCHA::twoBarType_of_GAINSCHA()
{
    if(twobartype.isEmpty()){
        /*二维码 类型文档中没有使用，这儿暂时不添加*/
    }
    return twobartype;
}

QMap<QString, QString> Printer_GAINSCHA::textfont_of_GAINSCHA()
{
    if(textfont.isEmpty()){
        textfont.insert("TSS24.BF2", "TSS24.BF2");
    }
    return textfont;
}


QString Printer_GAINSCHA::toPrint(const QString &in, int piece, QString height, int gap, QString width, const QString &unit)
{
    return QString(
                "SIZE %1%6,%2%6\n"
                "GAP %3mm,0mm\n"
                "DENSITY %4\n"
                "SPEED %5\n"
                "REFERENCE 1,1\n"
                "DIRECTION 1\n"
                "CLS\n"//清楚image的缓存
                "%7PRINT %8,1\n"
                )
            .arg(width).arg(height)
            .arg(gap)
            .arg(15)//density
            .arg(4) //speed
            .arg(unit)
            .arg(in)
            .arg(piece)
            ;
}


QString Printer_GAINSCHA::drawText(const QString &data, int x, int y, const QString &font, const QSize &size)
{
    return QString("TEXT %1,%2,\"%3\",%4,%5,%6,\"%7\"\n")
            .arg(x)
            .arg(y)
            .arg(font)
            .arg(0)//旋转角度
            .arg(size.width())
            .arg(size.height())//水平，垂直放大陪数
            .arg(data);//打印内容

}

QString Printer_GAINSCHA::drawOneCode(const QString &data, int x, int y, const QString &bartype, int height, Printer_GAINSCHA::ROTATE rotate, int widebarwidth, int narrowbarwidth)
{
    return QString("BARCODE %1,%2,\"%3\",%4,%5,%6,%7,%8,\"%9\"\n")
            .arg(x)
            .arg(y)
            .arg(bartype)//对于的条形码的种类，这儿只写一种，其他的请查阅文档
            .arg(height)//条形码的高度
            .arg(1)//条形码的可视属性，1为可见，0为不可见
            .arg(rotate)//旋转，这儿默认旋转为不旋转
            .arg(widebarwidth).arg(narrowbarwidth)//宽窄比例
            .arg(data);//内容
    //"BARCODE 0,60,\"128M\",20,1,0,2,2,\"!104!096ABCD!101EFGH\"\n"//条形码ok
}

QString Printer_GAINSCHA::drawTwoCode(const QString &data, int x, int y)
{
    return QString("QRCODE %1,%2,%3,%4,%5,%6,%7,%8,\"%9\"\n")
            .arg(x)
            .arg(y)
            .arg("L")//错误纠错能力等级
            .arg(7)//cell width

            .arg("M")//手动生成编码
            .arg(0)//不选转
            .arg("M1")//扩大版本
            .arg("S0")//mask 0

            .arg(QString("B0012%1").arg(data));//B0012是字节模式
    //QRCODE 100,10,L,7,M,0,M1,S1,"ATHE FIRMWARE HAS BEEN UPDATED"

}
