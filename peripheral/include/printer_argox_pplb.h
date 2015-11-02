#ifndef PRINTER_ARGOX_PPLB_H
#define PRINTER_ARGOX_PPLB_H

//【0】 字体的倍高倍宽支持字母 如0-9； A(10)-O(24)
//标签打印例子：100 39 0 2 mm 200dpi
#include <QMap>
#include <QString>
#include <QSize>
#include <stdio.h>
#include "printer_virtual.h"

#ifdef PRINTER_ARGOX_PPLB
#define PRINTER_ARGOX_PPLB_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define PRINTER_ARGOX_PPLB_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif
class PRINTER_ARGOX_PPLB_COMMON_DLLSPEC Printer_ARGOX_PPLB: public Printer_Virtual
{
public:
    enum ROTATE{
        ROTATE_0   = 0,
        ROTATE_90  = 1,
        ROTATE_180 = 2,
        ROTATE_270 = 3
    };
    static QMap<QString, QString> oneBarType_of_PPLB();
    static QMap<QString, QString> twoBarType_of_PPLB();
    static QMap<QString, QString> textfont_of_PPLB();
    static quint16 get_vid();
    static quint16 get_pid();

    Printer_ARGOX_PPLB();
    ~Printer_ARGOX_PPLB();
    //    N\n
    //    B20,20,0,E80,3,3,41,B,"0123459"\n
    //    P1\n

    QString toPrint(const QString &data, int pieces = 1);                //打印内容， 打印次数
    QString drawOneCode(
            const QString &data = "0123459"
            , int x = 20
            , int y = 20
            , const QString &bartype = "E80"                              //一维码类型
            , int height = 41                                             //条码高度

            , Printer_ARGOX_PPLB::ROTATE orientation = ROTATE_0           //方向指示
            , int narrow_width =3                                         //窄边宽
            , int wide_width = 3                                          //宽边宽
            , const QString &isprint = "B"                                //其实就是是否打印：N is not text is printed;B is readable is printed
            );
    QString drawText(
            const QString &data = "BC 1"
            , int x = 20
            , int y = 20
            , const QString &font = "966"                                 // 1 - 5表示字体的大小；a-z表示需要自己下载字体；966是通过软件下载好的字体

            , Printer_ARGOX_PPLB::ROTATE orientation = ROTATE_0           //方向指示
            , int horizontalscale = 1                                     //水平放大倍数
            , int verticalscale = 1                                       //垂直放大倍数
            , const QString &texttype = "N"                               //文本是否反相，N表示不反相；R表示反相。
            );
    QString drawTwoCode(
            const QString &data = "BC 1"
            , int x = 20
            , int y = 20
            , const QString &bartype = "Q"                                //这儿暂时只支持Q
            );

};
#endif // PRINTER_ARGOX_PPLB_H
