#ifndef PRINTER_ARGOX_H
#define PRINTER_ARGOX_H
//立象标签打印机
//【0】drawOneBar（） 这儿打y值不能为过小（如0；1） 否则会进纸多行
//【1】支持中文的话，同样需要依靠软件来实现下载
#include <QMap>
#include <QString>
#include "printer_virtual.h"
#ifdef PRINTER_ARGOX
#define PRINTER_ARGOX_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define PRINTER_ARGOX_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif

class PRINTER_ARGOX_COMMON_DLLSPEC Printer_ARGOX:public Printer_Virtual
{
public:
    enum Printer_ARGOX_ROTATE{
        ROTATE_0   = 1,
        ROTATE_90  = 2,
        ROTATE_180 = 3,
        ROTATE_270 = 4
    };
public:
    Printer_ARGOX();
    ~Printer_ARGOX();
    QByteArray toprint(const QString &in, int times, int widthpixel, int heightpixel); //[1][1]Sets width and height pixel.
    inline QString drawOneBar(QString data, int x, int y, QString bartype){
        return DrawBar(data
                       , ROTATE_0
                       , bartype
                       , 0, 0
                       , 0
                       , y
                       , x);
    }

    QMap<QString, QString> bartype_map;

    //protected:
    /*打印条码，内容只有条码和条码内容*/
    QString DrawBar(QString data = "BC 1"                              //[nn..]data
            , Printer_ARGOX_ROTATE orientation = ROTATE_0              //[1]orientation,1 represents for portrait
            , QString bartype = "A"                                 //[A]bar code type
            , int widebarwidth = 0, int narrowbarwidth = 0          //[5][2]wide bar width;narrow bar width
            , int barcodeheight = 0                                 //[000]bar code height ,000 stands for default height.
            , int y = 20                                            //[0020]y coord.
            , int x = 0                                             //[0020]x coord.

            );
};

#endif // PRINTER_ARGOX_H
