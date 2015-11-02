#include "printer_argox.h"
//立象 标签打印机
static QByteArray SOH = QByteArray::fromHex(QByteArray().append("01"));
static QByteArray STX = QByteArray::fromHex(QByteArray().append("02"));
static QByteArray CR = QByteArray::fromHex(QByteArray().append("0a"));
static QByteArray FILENAME = QByteArray::fromHex(QByteArray().append("3132330d0a"));

Printer_ARGOX::Printer_ARGOX()
{
    bartype_map.insert("3 OF 9", "A");
    bartype_map.insert("UPC-A", "B");
    bartype_map.insert("UPC-E", "C");
    bartype_map.insert("INTERLEADVED 2 OF 5", "D");
    bartype_map.insert("CODE 128", "E");

    bartype_map.insert("EAN-13", "F");
    bartype_map.insert("EAN-8", "G");
    bartype_map.insert("HBIC", "H");
    bartype_map.insert("CODA BAR", "I");
    bartype_map.insert("WITH CHECKSUM", "J");

    bartype_map.insert("PLESSEY", "K");
    bartype_map.insert("125 WITH CHECKSUM & BEARER", "L");
    bartype_map.insert("UPC2", "M");
    bartype_map.insert("UPC5", "N");
    bartype_map.insert("CODE 93", "O");

    bartype_map.insert("POSTNET", "P");
    bartype_map.insert("UCC/EAN 128", "Q");
    bartype_map.insert("UCC/EAN 128K", "R");
    bartype_map.insert("UCC/EAN CODE 128 RANDOM WEIGHT", "S");
    bartype_map.insert("TELEPEN", "T");

    bartype_map.insert("FIM", "V");
    bartype_map.insert("MAXICODE", "U");
    bartype_map.insert("DATAMATRIX", "W");
    bartype_map.insert("QR<AUTO FORMAT>", "W1d");
    bartype_map.insert("QR<MANUAL FORMAT>", "W1D");

    bartype_map.insert("AZTEC BARCODE", "W1f");
    bartype_map.insert("RSS", "W1K");
    bartype_map.insert("MIRCO PDF41", "W1z");
    bartype_map.insert("PDF-417", "Z");

//    凯士条码秤
}


Printer_ARGOX::~Printer_ARGOX()
{
}

QString Printer_ARGOX::DrawBar(QString data
                               , Printer_ARGOX_ROTATE orientation
                               , QString bartype, int widebarwidth
                               , int narrowbarwidth
                               , int barcodeheight
                               , int y
                               , int x
                               )
{
    return QString("%1%2%3%4%5%6%7%8\n")
            .arg(orientation)
            .arg(bartype)
            .arg(widebarwidth % 10)
            .arg(narrowbarwidth % 10)

            .arg(QString().sprintf("%03d",barcodeheight % 1000))
            .arg(QString().sprintf("%04d",y % 10000))
            .arg(QString().sprintf("%04d",x % 10000))
            .arg(data);
}

QByteArray Printer_ARGOX::toprint(const QString &in, int times, int widthpixel , int heightpixel)
{
    QByteArray b = STX + "L\n"; //Enter label formatting state
    b += QByteArray().append(QString("D%1%2\n")  //[1][1]sets width and height pixel
            .arg(widthpixel % 10)
            .arg(heightpixel % 10)
                           );
    b += QByteArray().append(in);
    b += "E\n";//ends the job and exit from label formatting mode

    return b;
}
