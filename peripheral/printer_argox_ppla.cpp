
#include "printer_argox_ppla.h"


/**
1.支持的一维码格式
Code 39、
UPC-A、
UPC-E、
Code 128 subset
A/B/C、
EAN-13、
EAN-8、
HBIC、
Codabar、
Plessey、
UPC2、
UPC5、
Code 93、
Postnet、
UCC/EAN-128、
UCC/EAN-128 K-MART、
UCC/EAN-128 Random
weight、
Telepen、
FIM、
Interleaved 2 of 5
(Standard/with modulo 10 checksum/ with human
readable check digit/ with modulo 10 checksum &
shipping bearer bars) 、
GS1 Data bar (RSS)
*/

/**
2.支持的二维码格式
MaxiCode、PDF417、Data Matrix (ECC 200
only) 、QR code、Composite Codes、
Aztec Barcode 、Micro PDF417
*/

/**
3.软件字型
需要通过font utility 下载字型
*/

/**
4.通信
Argox CP
打印机系列提供
9
针电子工业协会
(EIA)
标准的
RS-232
接口、
USB
接口、并口和以太网口。不同的接口选项适合
不同的用途。

CP-2140, CP-2140Z, CP-3140L
：并口埠
, USB
口和串口
CP-2140E, CP-3140E, CP-3140LE
：以太网口
, USB
口和串口
*/

/**
5.
*/
static QMap<QString, QString> onebartype;
static QMap<QString, QString> twobartype;
static QMap<QString, QString> textfont;

static QMap<QString, QString> _oneBarType_of_PPLA()
{
    if(onebartype.isEmpty()){
        onebartype.insert("3 OF 9", "A");
        onebartype.insert("UPC-A", "B");
        onebartype.insert("UPC-E", "C");
        onebartype.insert("INTERLEADVED 2 OF 5", "D");
        onebartype.insert("CODE 128", "E");

        onebartype.insert("EAN-13", "F");
        onebartype.insert("EAN-8", "G");
        onebartype.insert("HBIC", "H");
        onebartype.insert("CODA BAR", "I");
        onebartype.insert("WITH CHECKSUM", "J");

        onebartype.insert("PLESSEY", "K");
        onebartype.insert("125 WITH CHECKSUM & BEARER", "L");
        onebartype.insert("UPC2", "M");
        onebartype.insert("UPC5", "N");
        onebartype.insert("CODE 93", "O");

        onebartype.insert("POSTNET", "P");
        onebartype.insert("UCC/EAN 128", "Q");
        onebartype.insert("UCC/EAN 128K", "R");
        onebartype.insert("UCC/EAN CODE 128 RANDOM WEIGHT", "S");
        onebartype.insert("TELEPEN", "T");

        onebartype.insert("FIM", "V");

    }
    return onebartype;
}
static QMap<QString, QString> _twoBarType_of_PPLA()
{
    if(twobartype.isEmpty()){
        twobartype.insert("MAXICODE", "U");
        twobartype.insert("DATAMATRIX", "W");
        twobartype.insert("QR<AUTO FORMAT>", "W1d");
        twobartype.insert("QR<MANUAL FORMAT>", "W1D");

        twobartype.insert("RSS", "W1k");
        twobartype.insert("AZTEC BARCODE(variabel length)", "W1f");
        twobartype.insert("AZTEC BARCODE(specified length/byte count)", "W1F");
        twobartype.insert("MIRCO PDF417", "W1z");

        twobartype.insert("PDF-417", "z");
    }
    return twobartype;
}
static QMap<QString, QString> _textfont_of_PPLA()
{
    if(textfont.isEmpty()){
        textfont.insert("font0", "0");
        textfont.insert("font1", "1");
        textfont.insert("font2", "2");
        textfont.insert("font3", "3");

        textfont.insert("font4", "4");
        textfont.insert("font6", "6");
        textfont.insert("font7", "7");
        textfont.insert("font8", "8");

    }
    return textfont;
}

static QByteArray SOH = QByteArray::fromHex(QByteArray().append("01"));
static QByteArray STX = QByteArray::fromHex(QByteArray().append("02"));
static QByteArray CR = QByteArray::fromHex(QByteArray().append("0a"));
static QByteArray FILENAME = QByteArray::fromHex(QByteArray().append("3132330d0a"));

Printer_ARGOX_PPLA::Printer_ARGOX_PPLA()
{
}

Printer_ARGOX_PPLA::~Printer_ARGOX_PPLA()
{
}

QString Printer_ARGOX_PPLA::toPrint(const QString &in, int piece, int widthpixel, int heightpixel)
{
    QString b = STX + "L\n";    //Enter label formatting state
    b += QString("D%1%2\n")     //[1][1]sets width and height pixel
            .arg(widthpixel % 10)
            .arg(heightpixel % 10);
    b += in;
    b += "E\n";                 //ends the job and exit from label formatting mode

    while(--piece){
        b += b;
    }
    return b;
}

QString Printer_ARGOX_PPLA::drawOneCode(const QString &data, int x, int y, const QString &bartype, int height, Printer_ARGOX_PPLA::ROTATE orientation, int widebarwidth, int narrowbarwidth)
{
    return QString("%1%2%3%4%5%6%7%8\n")
            .arg(orientation)
            .arg(bartype)
            .arg(widebarwidth % 10)
            .arg(narrowbarwidth % 10)

            .arg(QString().sprintf("%03d",height % 1000))
            .arg(QString().sprintf("%04d",y % 10000))
            .arg(QString().sprintf("%04d",x % 10000))
            .arg(data);
}

QString Printer_ARGOX_PPLA::drawText(const QString &data, int x, int y, const QString &font, Printer_ARGOX_PPLA::ROTATE orientation, int widthmutiplier, int heightmultiplier, int barcodeheight)
{
    return QString("%1%2%3%4%5%6%7%8\n")
            .arg(orientation)
            .arg(font)
            .arg(widthmutiplier % 10)
            .arg(heightmultiplier % 10)

            .arg(QString().sprintf("%03d",barcodeheight % 1000))
            .arg(QString().sprintf("%04d",y % 10000))
            .arg(QString().sprintf("%04d",x % 10000))
            .arg(data)
            ;
}

QString Printer_ARGOX_PPLA::drawTwoCode(const QString &data, int x, int y, const QString &bartype, int value, Printer_ARGOX_PPLA::ROTATE orientation, int verticalmultiplier, int horizontalmultiplier)
{
    return QString("%1%2%3%4%5%6%7%8\n")
            .arg(orientation)
            .arg(bartype)
            .arg(horizontalmultiplier % 10)
            .arg(verticalmultiplier % 10)

            .arg(QString().sprintf("%03d",value % 1000))
            .arg(QString().sprintf("%04d",y % 10000))
            .arg(QString().sprintf("%04d",x % 10000))
            .arg(data)

            ;
}

QMap<QString, QString> Printer_ARGOX_PPLA::oneBarType_of_PPLA()
{
    return _oneBarType_of_PPLA();
}

QMap<QString, QString> Printer_ARGOX_PPLA::twoBarType_of_PPLA()
{
    return _twoBarType_of_PPLA();
}

QMap<QString, QString> Printer_ARGOX_PPLA::textfont_of_PPLA()
{
    return _textfont_of_PPLA();
}

