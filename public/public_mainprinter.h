#ifndef PUBLIC_MAINPRINTER_H
#define PUBLIC_MAINPRINTER_H

#include <QObject>
#include <QStandardItemModel>
#include <QMap>
#include <QMutex>
#include <QDebug>
#include "bmp_info.h"
#include "printer_pos_serial.h"
#include "printer_pos_net.h"
#include "commonheader.h"

/*
    public_mainprinter printer;
    QByteArray b;
    b += printer.drawText("1B40 1B6101 1C5701", "交 班 对 账\n");//清除缓存 居中 2x2x
    b += printer.drawText("1B40 1B6101 1C5700", "交 班 对 账\n");//清除缓存 居中 1x1x
    b += printer.drawText("1B40", "");//清除缓存

    b += printer.drawText("0a", "Left", Qt::AlignLeft, 20);
    b += printer.drawText("", "Left", Qt::AlignLeft, 20);
    b += printer.drawText("0a", "LeftLeft", Qt::AlignLeft, 20);
    b += printer.drawText("", "LeftLeftLeft", Qt::AlignLeft, 20);

    b += printer.drawText("0a", "Center", Qt::AlignCenter, 20);
    b += printer.drawText("", "Center", Qt::AlignCenter, 20);
    b += printer.drawText("0a", "CenterCenter", Qt::AlignCenter, 20);
    b += printer.drawText("", "CenterCenterCenter", Qt::AlignCenter, 20);

    b += printer.drawText("0a", "Right", Qt::AlignRight, 20);
    b += printer.drawText("", "Right", Qt::AlignRight, 20);
    b += printer.drawText("0a", "RightRight", Qt::AlignRight, 20);
    b += printer.drawText("", "RightRightRight", Qt::AlignRight, 20);

    b += printer.drawText("1B6405 1D5601", "");//走纸5x， 切纸

    printer.asynchPrint(printer.setPrintCount(b, 1), "192.168.1.172:9100");
*/

/*
    printer->asynchPrint(printer->setPrintCount(printer->drawLogo("1B40 1B6101", ":/image/logo.png"), 1),"/dev/ttyS0" );//清除缓存 居中 pixmap路径
*/

/*
    printer->asynchPrint(printer->setPrintCount(printer->drawQR_Code("","www.baidu.com"), 1),"/dev/ttyS0" );
*/

/*
    printer->asynchPrint(printer->setPrintCount(printer->drawoneBarcode("","123456789"), 1),"/dev/ttyS0" );

*/
class public_mainprinter : public QObject
{
    Q_OBJECT
public:
    enum LITTLEINS{
        HCENTER=0x1B6101,
        HLEFT=0x1B6100,
        HRIGHT=0x1B6102,
        FONTSIZE11=0x1D2111,
        FONTSIZE10=0x1D2110,
        FONTSIZE01=0x1D2101,
        FONTSIZE00=0x1D2100,
        RESET=0x1B40,
        NUMCUT1=0x1D5601,
        NUMCUT2=0x1D5602,
        NUMCUT3=0x1D5603,
        NUMCUT4=0x1D5604,
        NUMCUT5=0x1D5605
    };

    explicit public_mainprinter(QObject *parent = 0);
    ~public_mainprinter();
    pieceIns n_filldata_n(const QByteArray &hex=QByteArray(""), char c='-', int textwidth=-1);//填充
    pieceIns filldata_n(const QByteArray &hex=QByteArray(""), char c='-', int textwidth=-1);//填充
    pieceIns drawText(const QByteArray &hex=QByteArray(""), const QString &text=QString(""), Qt::Alignment textflag=Qt::AlignLeft, int textwidth=-1);//获取文本，打印指令
    blockIns drawText(const QByteArray &hex, const QString &text, Qt::Alignment textflag, int textwidth, int blanks, const QString &text2);
    pieceIns drawLogo(const QByteArray &hex=QByteArray(""), const QString &logopath=QString(""));//获取logo，打印指令
    pieceIns drawQR_Code(const QByteArray &hex/*=QByteArray("1B40 1B6101")*/, const QString &code);//获取二维码，打印指令
    pieceIns drawoneBarcode(const QByteArray &hex/*=QByteArray("1B40 1B6101")*/, const QString &code);//获取二维码，打印指令

    blockIns setPrintCount(const blockIns &text, int printcount=1);//获取打印次数，打印指令

    bool asynchPrint(const blockIns &printIns, const QString &path, bool isblock=false);//异步打印
    bool synchPrint(const blockIns &printIns, const QString &path);//同步打印 出网口外

    void print_data(const blockIns &printIns);
public slots:
    void sendError(const QString &err, const QString &title);

signals:
    void errorString(const QString &err, const QString &title);
private:
    QString stringToHex(QString str);

};

#endif // PUBLIC_MAINPRINTER_H
