#include "printer_postek.h"
//博思得标签打印机
/*

*/
static QByteArray SOH = QByteArray::fromHex(QByteArray().append("01"));
static QByteArray STX = QByteArray::fromHex(QByteArray().append("02"));
static QByteArray CR = QByteArray::fromHex(QByteArray().append("0D"));
static QByteArray LF = QByteArray::fromHex(QByteArray().append("0A"));
static QByteArray FILENAME = "\"standalone\"";

static QMap<QString, QString> textfont;
static QMap<QString, QString> onebartype;
static QMap<QString, QString> twobartype;
Printer_POSTEK::Printer_POSTEK()
{
}

Printer_POSTEK::~Printer_POSTEK()
{
}

QString Printer_POSTEK::toPrint(const QString &in, int times, int height, int gap)
{
    QByteArray b;
    b += "N"+CR+LF;
    b += "ZB"+CR+LF;//set print direction
    b += "FS" + FILENAME+CR+LF;
    b += in;
    b += "FE"+CR+LF;
    b += QString("Q%1,%2").arg(height).arg(gap)+CR+LF;
    b += "FR" + FILENAME+CR+LF;
    b += QString("W%1").arg(times)+CR+LF;
    b += "FK" + FILENAME+CR+LF;

    return QString(b);
}


QString Printer_POSTEK::drawText(const QString &data, int x, int y, const QString &font, Printer_POSTEK::ROTATE rotate, int xbiger, int ybiger, const QString &textprinttype)
{
    return QString("T%1,%2,%3,%4,%5,%6,%7,\"%8\"")
            .arg(x)
            .arg(y)
            .arg(rotate)
            .arg(font)

            .arg((xbiger%25) > 0 ? xbiger % 25 : 1)
            .arg((ybiger%25) > 0 ? ybiger % 25 : 1)
            .arg(textprinttype)
            .arg(data)+CR+LF;
}


QString Printer_POSTEK::drawOneCode(const QString &data, int x, int y, const QString &onebartype, int height, Printer_POSTEK::ROTATE rotate, int wide_width , int narrow_width, const QString &onebarprinttype)
{
    return QString("B%1,%2,%3,%4,%5,%6,%7,%8,\"%9\"")
            .arg(x)
            .arg(y)
            .arg(rotate)
            .arg(onebartype)

            .arg(narrow_width)
            .arg(wide_width)
            .arg(height)
            .arg(onebarprinttype)

            .arg(data)+CR+LF
            ;
}

QString Printer_POSTEK::drawTwoCode(const QString &data, int x, int y, const QString &bartype, int maxwidth, int maxheight, Printer_POSTEK::ROTATE rotate, int bigger, Printer_POSTEK::MODEL twobarmodel, Printer_POSTEK::RECOVERY recovery, Printer_POSTEK::MASK mask)
{
        return QString("b%1,%2,%3,%4,%5,o%6,r%7,m%8,g%9,s%10,\"%11\"")
                .arg(x)
                .arg(y)
                .arg(bartype)
                .arg(maxwidth)
                .arg(maxheight)

                .arg(rotate)
                .arg(bigger)
                .arg(twobarmodel)
                .arg(recovery)

                .arg(mask)
                .arg(data)+CR+LF
                ;
}

QString Printer_POSTEK::drawLineXOR(const QPoint &postion
                                    ,int width
                                    ,int height)
{
    //LE50,30,100,10 CR LF
    return QString("LE%1,%2,%3,%4")
            .arg(postion.x())// 直线 x
            .arg(postion.y())// 直线 y
            .arg(width)//直线宽
            .arg(height) + CR + LF;//直线高
}

QString Printer_POSTEK::drawLineOR(const QPoint &postion
                                   ,int width
                                   ,int height)
{
    //LO50,30,100,10 CR LF
    return QString("LO%1,%2,%3,%4")
            .arg(postion.x())// 直线 x
            .arg(postion.y())// 直线 y
            .arg(width)//直线宽
            .arg(height) + CR + LF;//直线高
}

QString Printer_POSTEK::drawLineSlash(const QPoint &beginpos
                                      , int bold
                                      , const QPoint &endpos)
{
    //LS50,30,10,100,80 CR LF
    return QString("LS%1,%2,%3,%4,%5")
            .arg(beginpos.x())// 直线 x
            .arg(beginpos.y())// 直线 y
            .arg(bold)//直线宽
            .arg(endpos.x())// 直线 x
            .arg(endpos.y()) + CR + LF;
}

QString Printer_POSTEK::drawLineWhite(const QPoint &postion
                                      ,int width
                                      ,int height)
{
    //LW100,20,5,110 CR LF
    return QString("LW%1,%2,%3,%4")
            .arg(postion.x())// 直线 x
            .arg(postion.y())// 直线 y
            .arg(width)//直线宽
            .arg(height) + CR + LF;//直线高
}

QString Printer_POSTEK::drawRectage(const QPoint &beginpos
                                    , int bold
                                    , const QPoint &endpos)
{
    //X50,120,5,250,150  CR LF
    return QString("X%1,%2,%3,%4,%5")
            .arg(beginpos.x())// 直线 x
            .arg(beginpos.y())// 直线 y
            .arg(bold)//直线宽
            .arg(endpos.x())// 直线 x
            .arg(endpos.y()) + CR + LF;
}

QMap<QString, QString> Printer_POSTEK::oneBarType_of_POSTEK()
{
    if(onebartype.isEmpty()){
        onebartype.insert("Code 128 UCC (shipping container code)", "0");
        onebartype.insert("Code 128 AUTO", "1");
        onebartype.insert("Code 128 subset A", "1A");
        onebartype.insert("Code 128 subset B", "1B");
        onebartype.insert("Code 128 subset C", "1C");

        onebartype.insert("UCC/EAN", "1E");
        onebartype.insert("Interleaved 2 of 5", "2");
        onebartype.insert("Interleaved 2 of 5 with check sum digit)", "2C");
        onebartype.insert("Interleaved 2 of 5 with human readable check digit", "2D");
        onebartype.insert("German Postcode", "2G");

        onebartype.insert("Matrix 2 of 5", "2M");
        onebartype.insert("UPC Interleaved 2 of 5", "2U");
        onebartype.insert("Code 3 of 9", "3");
        onebartype.insert("Code 3 of 9 with check sum digit", "3C");
        onebartype.insert("Extended Code 3 of 9", "3E");

        onebartype.insert("Extended Code 3 of 9 with check sum digit", "3F");
        onebartype.insert("Code93", "9");
        onebartype.insert("EAN-13", "E30");
        onebartype.insert("EAN-13 2 digit add-on", "E32");
        onebartype.insert("EAN-13 5 digit add-on", "E35");

        onebartype.insert("EAN-8", "E80");
        onebartype.insert("EAN-8 2 digit add-on", "E82");
        onebartype.insert("EAN-8 5 digit add-on", "E-85");
        onebartype.insert("Codabar", "K");
        onebartype.insert("Postnet", "P");

        onebartype.insert("UPC-A", "UA0");
        onebartype.insert("UPC-A 2 digit add-on", "UA2");
        onebartype.insert("UPC-A 5 digit add-on", "UA5");
        onebartype.insert("UPC-E", "UE0");
        onebartype.insert("UPC-E 2 digit add-on", "UE2");

        onebartype.insert("UPC-E 5 digit add-on ", "UE5");
    }
    return onebartype;
}

QMap<QString, QString> Printer_POSTEK::twoBarType_of_POSTEK()
{
    if(twobartype.isEmpty()){
        twobartype.insert("Maxi Code", "M");
        twobartype.insert("PDF-417", "P");
        twobartype.insert("DataMatrix ECC200", "DX");
        twobartype.insert("DataMatrix ECC000 （*）", "D0");
        twobartype.insert("DataMatrix ECC050 （*）", "D1");

        twobartype.insert("DataMatrix ECC080 （*）", "D2");
        twobartype.insert("DataMatrix ECC100 （*）", "D3");
        twobartype.insert("DataMatrix ECC140 （*）", "D4");
        twobartype.insert("QR", "QR");
        twobartype.insert("汉信码", "HX");
    }
    return twobartype;
}

QMap<QString, QString> Printer_POSTEK::textfont_of_POSTEK()
{
    if(textfont.isEmpty()){
        textfont.insert("西文字体1", "1");
        textfont.insert("西文字体2","2");
        textfont.insert("西文字体3","3");
        textfont.insert("西文字体4","4");
        textfont.insert("西文字体5","5");
        textfont.insert("24点阵中文字体","6");
    }
    return textfont;
}


