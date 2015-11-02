#include "printer_argox_pplb.h"
#include <QtDebug>

/**
1.支持的一维码格式
0       Code 128 UCC (shipping container code)
1       Code 128 auto
1A      Code 128 subset A
1B      Code 128 subset B
1C      Code 128 subset C
1E      UCC/EAN
2       Interleaved 2 of 5
2C      Interleaved 2 of 5 with check sum digit
2D      Interleaved 2 of 5 with human readable checkdigit
2G      German Postcode
2M      Matrix 2 of 5
2U      UPC Interleaved 2 of 5
3       Code 3 of 9
3C      Code 3 of 9 with check sum digit
9       Code 93
E30     EAN-13
E32     EAN-13 2 digit add-on
E35     EAN-13 5 digit add-on
E80     EAN-8
E82     EAN-8 2 digit add-on
E85     EAN-8 5 digit add-on
K       Codabar
P       Postnet
UA0     UPC-A
UA2     UPC-A 2 digit add-on
UA5     UPC-A 5 digit add-on
UE0     UPC-E
UE2     UPC-E 2 digit add-on
UE5     UPC-E 5 digit add-on
*/

/**
2.支持的二维码格式
A       Aztec-Specific Options
M       Maxi Code
P       PDF-417
D       Data Matrix
Q or QR QR Code
*/
static QMap<QString, QString> onebartype;
static QMap<QString, QString> twobartype;
static QMap<QString, QString> textfont;

static QMap<QString, QString> _oneBarType_of_PPLB()
{
    if(onebartype.isEmpty()){
        onebartype.insert("Code 128 UCC (shipping container code)", "0");
        onebartype.insert("Code 128 auto)", "1");
        onebartype.insert("Code 128 subset A", "1A");
        onebartype.insert("Code 128 subset B", "1B");
        onebartype.insert("Code 128 subset C", "1C");

        onebartype.insert("UCC/EAN", "1E");
        onebartype.insert("Interleaved 2 of 5", "2");
        onebartype.insert("Interleaved 2 of 5 with check sum digit", "2C");
        onebartype.insert("Interleaved 2 of 5 with human readable check digit", "2D");
        onebartype.insert("German Postcode", "2G");

        onebartype.insert("Matrix 2 of 5", "2M");
        onebartype.insert("UPC Interleaved 2 of 5", "2U");
        onebartype.insert("Code 3 of 9", "3");
        onebartype.insert("Code 3 of 9 with check sum digit", "3C");
        onebartype.insert("Code 93", "9");

        onebartype.insert("EAN-13", "E30");              //EAN码的结构为12+p、7+p、p为校验位；长度不能小于12，7：小于不打印 大于截取
        onebartype.insert("EAN-13 2 digit add-on", "E32");
        onebartype.insert("EAN-13 5 digit add-on", "E35");
        onebartype.insert("EAN-8", "E80");
        onebartype.insert("EAN-8 2 digit add-on", "E82");

        onebartype.insert("EAN-8 5 digit add-on", "E85");
        onebartype.insert("Codabar", "K");
        onebartype.insert("Postnet", "P");
        onebartype.insert("UPC-A", "UA0");
        onebartype.insert("UPC-A 2 digit add-on", "UA2");

        onebartype.insert("UPC-A 5 digit add-on", "UA5");
        onebartype.insert("UPC-E", "UE0");
        onebartype.insert("UPC-E 2 digit add-on", "UE2");
        onebartype.insert("UPC-E 5 digit add-on", "UE5");

    }
    return onebartype;
}
static QMap<QString, QString> _twoBarType_of_PPLB()
{
    if(twobartype.isEmpty()){
        twobartype.insert("Aztec-Specific Options", "A");
        twobartype.insert("Maxi Code", "M");
        twobartype.insert("PDF-417", "P");
        twobartype.insert("Data Matrix", "D");

        twobartype.insert("QR Code", "Q"/*Q or QR*/);
    }
    return twobartype;
}
static QMap<QString, QString> _textfont_of_PPLB()
{
    if(textfont.isEmpty()){
        textfont.insert("font1", "1");
        textfont.insert("font2", "2");
        textfont.insert("font3", "3");
        textfont.insert("font4", "4");
        textfont.insert("font5", "5");
    }
    return textfont;
}
Printer_ARGOX_PPLB::Printer_ARGOX_PPLB()
{
}

Printer_ARGOX_PPLB::~Printer_ARGOX_PPLB()
{
}

QString Printer_ARGOX_PPLB::toPrint(const QString &data, int pieces)
{
    qDebug() << QString("N\n%1\n%2P%3\n")
                .arg("ZB")//set print direction
                .arg(data)
                .arg(pieces);

    return QString("N\n%1\n%2P%3\n")
            .arg("ZB")//set print direction
            .arg(data)
            .arg(pieces)
            ;
}

QString Printer_ARGOX_PPLB::drawOneCode(const QString &data, int x, int y, const QString &bartype, int height, Printer_ARGOX_PPLB::ROTATE orientation, int narrow_width, int wide_width, const QString &isprint)
{
    return QString("B%1,%2,%3,%4,%5,%6,%7,%8,\"%9\"\n")
            .arg(x)
            .arg(y)
            .arg(orientation)
            .arg(bartype)

            .arg(narrow_width)
            .arg(wide_width)
            .arg(height)
            .arg(isprint)

            .arg(data)

            ;
}

QString Printer_ARGOX_PPLB::drawText(const QString &data, int x, int y, const QString &font, Printer_ARGOX_PPLB::ROTATE orientation, int horizontalscale, int verticalscale, const QString &texttype)
{
    return QString("A%1,%2,%3,%4,%5,%6,%7,\"%8\"\n")
            .arg(x)
            .arg(y)
            .arg(orientation)
            .arg(font)

            .arg(horizontalscale)
            .arg(verticalscale)
            .arg(texttype)
            .arg(data)
            ;
}

QString Printer_ARGOX_PPLB::drawTwoCode(const QString &data, int x, int y, const QString &bartype)
{
    return QString("b%1,%2,%3,\"%4\"\n")
            .arg(x)
            .arg(y)
            .arg(bartype)
            .arg(data)
            ;
}

QMap<QString, QString> Printer_ARGOX_PPLB::oneBarType_of_PPLB()
{
    return _oneBarType_of_PPLB();
}

QMap<QString, QString> Printer_ARGOX_PPLB::twoBarType_of_PPLB()
{
    return _twoBarType_of_PPLB();
}

QMap<QString, QString> Printer_ARGOX_PPLB::textfont_of_PPLB()
{
    return _textfont_of_PPLB();
}

quint16 Printer_ARGOX_PPLB::get_vid()
{
    return 0x1664;
}

quint16 Printer_ARGOX_PPLB::get_pid()
{
    return 0x081b;
}
