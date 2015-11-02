#ifndef PRINTER_GAINSCHA_H
#define PRINTER_GAINSCHA_H

//佳博标签打印

//toPrint(const QString &in,int piece=1, int page=1,const QSize &s=QSize(40,30),int density=15,int speed=4);
//指定打印的份数，指定每份打印的张数，设置卷纸的长宽，浓度，速度
//【0】 支持usb和串口
//【1】 默认单位为 mm
//【2】 串口波特率为9600
//【3】 中文支持需要下载字库到打印机 这儿窗口里需要加字体打名称，方便在软件里添加

#include <QMap>
#include <QSize>
#include <QRect>
#include <QString>
#include <QObject>
#include "printer_virtual.h"

#ifdef PRINTER_GAINSCHA
#define PRINTER_GAINSCHA_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define PRINTER_GAINSCHA_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif
//打印 单位mm
class PRINTER_GAINSCHA_COMMON_DLLSPEC Printer_GAINSCHA:public Printer_Virtual
{
public:
    enum ROTATE{
        ROTATE_0   = 0,
        ROTATE_90  = 1,
        ROTATE_180 = 2,
        ROTATE_270 = 3
    };

    static QMap<QString, QString> oneBarType_of_GAINSCHA();
    static QMap<QString, QString> twoBarType_of_GAINSCHA();
    static QMap<QString, QString> textfont_of_GAINSCHA();

public:
    explicit Printer_GAINSCHA();
    virtual QMap<QString/*表格特例*/, QSizeF/*宽高*/> get_speical_PRINTSIZE();

    virtual QString toPrint(const QString &in,
                            int piece = 1,
                            QString height = "32.48",
                            int gap = 0,
                            QString width = "10",
                            const QString &unit="mm");//在拼接成的指令的基础上，补充并打印

    QString drawText(
            const QString &data = "BC 1"                //打印内容
            , int x = 20
            , int y = 20
            , const QString &font = "TSS24.BF2"           //打印字体

            , const QSize &size = QSize(1,1)            //倍高倍宽
            );

    QString drawOneCode(
            const QString &data = "0123456789"          //打印内容
            , int x = 20
            , int y = 20
            , const QString &bartype = "128M"           //一维码类型
            , int height = 41
            , Printer_GAINSCHA::ROTATE rotate = ROTATE_0     //[1]orientation,1 represents for portrait

            , int widebarwidth = 0                      //宽边宽
            , int narrowbarwidth = 0                    //窄边宽

            );

    QString drawTwoCode(
            const QString &data = "0123456789"          //打印内容
            , int x = 20
            , int y = 20
            );
};

#endif // PRINTER_GAINSCHA_H
