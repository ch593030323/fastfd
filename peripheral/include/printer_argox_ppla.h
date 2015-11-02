#ifndef PRINTER_ARGOX_PPLA_H
#define PRINTER_ARGOX_PPLA_H
//立象标签打印机
//【0】drawOneBar（） 这儿打y值不能为过小（如0；1） 否则会进纸多行
//【1】支持中文的话，同样需要依靠软件来实现下载
//【2】    //暂时不能用 QString drawTwoCode(

//打印机设置
//条码EXAMPLE   400      12
//              0
//              2
//              dots
//              200dpi
#include <QMap>
#include <QString>
#include <QMap>
#include <QSize>
#include "printer_virtual.h"

#ifdef PRINTER_ARGOX_PPLA
#define PRINTER_ARGOX_PPLA_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define PRINTER_ARGOX_PPLA_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif

class PRINTER_ARGOX_PPLA_COMMON_DLLSPEC Printer_ARGOX_PPLA:public Printer_Virtual
{
public:

    enum ROTATE{
        ROTATE_0   = 1,
        ROTATE_90  = 2,
        ROTATE_180 = 3,
        ROTATE_270 = 4
    };
    static QMap<QString, QString> oneBarType_of_PPLA();
    static QMap<QString, QString> twoBarType_of_PPLA();
    static QMap<QString, QString> textfont_of_PPLA();
public:
    Printer_ARGOX_PPLA();
    ~Printer_ARGOX_PPLA();

    QString toPrint(
            const QString &in
            , int piece = 1
            , int widthpixel = 1, int heightpixel = 1               //[1][1]Sets width and height pixel.
            );
    QString drawOneCode(
            const QString &data = "BC 1"                            //[nn..]data
            , int x = 0                                             //[0000]x coord.
            , int y = 20                                            //[0020]y coord.
            , const QString &bartype = "A"                          //[A]bar code type
            , int height = 0                                 //[000]bar code height ,000 stands for default height.

            , Printer_ARGOX_PPLA::ROTATE orientation = ROTATE_0     //[1]orientation,1 represents for portrait
            , int widebarwidth = 0, int narrowbarwidth = 0          //[5][2]wide bar width;narrow bar width

            );
    QString drawText(
            const QString &data = "BC 1"                            //[nn..]data
            , int x = 0                                             //[0000]x coord.
            , int y = 20                                            //[0020]y coord.
            , const QString &font = "2"                             //[1]font type

            , Printer_ARGOX_PPLA::ROTATE orientation = ROTATE_0     //[1]orientation,1 represents for portrait
            , int widthmutiplier = 1
            , int heightmultiplier = 1                              //
            , int barcodeheight = 1                                 //[1]bar code height is ignored in case of text

            );
    //暂时不能用
    QString drawTwoCode(
            const QString &data = "BC 1"
            , int x = 0                                             //[0000]x coord.
            , int y = 20                                            //[0020]y coord.
            , const QString &bartype = "W1d"                        //[WID]bar code type

            , int value = 0                                         //[000] ineffective; must bu numeric
            , Printer_ARGOX_PPLA::ROTATE orientation = ROTATE_0     //[1]orientation,1 represents for portrait
            , int verticalmultiplier = 4                            //[4]
            , int horizontalmultiplier = 4                          //[4]
            );
};

#endif // PRINTER_ARGOX_PPLA_H
