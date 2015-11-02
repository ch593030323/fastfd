#ifndef PRINTER_TSC_TTP_244_PLUS_H
#define PRINTER_TSC_TTP_244_PLUS_H

#include <QObject>
#include <QMap>
#include <QSize>
#include "printer_virtual.h"
#ifdef PRINTER_TSC_TTP_244_PLUS
#define PRINTER_TSC_TTP_244_PLUS_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define PRINTER_TSC_TTP_244_PLUS_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif

//打印 单位mm
class PRINTER_TSC_TTP_244_PLUS_COMMON_DLLSPEC printer_TSC_TTP_244_PLUS:public Printer_Virtual
{
public:
    enum ROTATE{
        ROTATE_0   = 0,
        ROTATE_90  = 1,
        ROTATE_180 = 2,
        ROTATE_270 = 3
    };

    static QMap<QString, QString> oneBarType_of_TSC();
    static QMap<QString, QString> twoBarType_of_TSC();
    static QMap<QString, QString> textfont_of_TSC();
public:
    explicit printer_TSC_TTP_244_PLUS();

    QString toPrint(const QString &in, int piece = 1, const QString &height = "40", int gap = 0, const QString &width = "10");//在拼接成的指令的基础上，补充并打印

    QString drawText(
            const QString &data = "BC 1"                //打印内容
            , int x = 20
            , int y = 20
            , const QString &font = "FONT001"           //打印字体

            , const QSize &size = QSize(1,1)            //倍高倍宽
            );

    QString drawOneCode(
            const QString &data = "0123456789"          //打印内容
            , int x = 20
            , int y = 20
            , const QString &bartype = "128M"           //一维码类型
            , int height = 41
            , printer_TSC_TTP_244_PLUS::ROTATE rotate = ROTATE_0     //[1]orientation,1 represents for portrait

            , int widebarwidth = 0                      //宽边宽
            , int narrowbarwidth = 0                    //窄边宽

            );
    QString drawTwoCode(
            const QString &data = "0123456789"          //打印内容
            , int x = 20
            , int y = 20
            );
};

#endif // PRINTER_TSC_TTP_244_PLUS_H
