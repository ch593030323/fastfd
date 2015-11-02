#include "public_mainprinter.h"
#include "printer_pos_serial.h"
#include "trans_u2g.h"
#include "qextserialenumerator.h"
#include "public_sql.h"
#include "public_logindialog.h"
#include "printer_pos_vidpid.h"
#include "printer_normal_t2us.h"
#include "printer_normal_58.h"
#include "printer_normal_80.h"
#include "printer_normal_kf58.h"
#include "asyn_printer.h"

public_mainprinter::public_mainprinter(QObject *parent) :
    QObject(parent)
{}

public_mainprinter::~public_mainprinter()
{}

pieceIns public_mainprinter::n_filldata_n(const QByteArray &hex, char c, int textwidth)
{
    QByteArray ret;
    while(textwidth--){
        ret+=c;
    }
    return pieceIns(hex, "\n"+ret+"\n");
}

pieceIns public_mainprinter::filldata_n(const QByteArray &hex, char c, int textwidth)
{
    QByteArray ret;
    while(textwidth--){
        ret+=c;
    }
    return pieceIns(hex, ret+"\n");
}

pieceIns public_mainprinter::drawText(const QByteArray &hex, const QString &text, Qt::Alignment textflag, int textwidth)
{
    //    pieceIns pret;
    QByteArray ret;
    //    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gbk"));
    //【0】    文本长度过长会破坏格式
    ret=text.toLocal8Bit();
    int actualtextwidth=size_g_of_u(text);//ret.length();
    char blank=' ';
    if(actualtextwidth < textwidth){//文本实际宽度，小于文本预宽度
        switch(textflag){
        case Qt::AlignCenter://居中
            ret=QByteArray((textwidth-actualtextwidth)/2, blank) + ret + QByteArray((textwidth-actualtextwidth)/2, blank);
            break;
        case Qt::AlignRight://右对齐
            ret=QByteArray((textwidth-actualtextwidth), blank) + ret;
            break;
        case Qt::AlignLeft://左对齐
            ret=ret + QByteArray((textwidth-actualtextwidth), blank);
            break;
        }
        //最后统一按左对齐处理
    }
    //    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    //    ret=QByteArray::fromHex(hex)+ret;
    return pieceIns(hex, ret);
}

blockIns public_mainprinter::drawText(const QByteArray &hex, const QString &text, Qt::Alignment textflag, int textwidth,  int blanks, const QString &text2)
{
    blockIns ins;
    ins.append(drawText(hex, text,textflag,textwidth));
    ins.append(drawText("", QString(blanks,QChar(' '))+text2));
    return ins;
}

pieceIns public_mainprinter::drawLogo(const QByteArray &hex, const QString &logopath)
{
    QByteArray b0;
    Bmp_info bmpmaker;
    bmpmaker.Pic2printIns(logopath);
    bmpmaker.makeIns(b0);
    return pieceIns(hex+b0, "");
}

pieceIns public_mainprinter::drawQR_Code(const QByteArray &hex ,const QString &code)
{
    QByteArray b0;
    QString hexString = stringToHex(code);
    QString quyuString = QString::number((hexString.length()/2 +3)%256,16);
    if(quyuString.length() == 1)
        quyuString = "0" + quyuString;
    QString chushuString = QString::number((hexString.length()/2 +3)/256,16);
    if(chushuString.length() == 1)
        chushuString = "0" + chushuString;

    b0+=("1B6101");
    b0+=("1D286B0300314308");
    b0+=("1d286b");
    b0+=(QByteArray().append(quyuString));
    b0+=(QByteArray().append(chushuString));
    b0+=("315030");
    b0+=(QByteArray().append(hexString));
    b0+=("1D286B0300315130");

    return pieceIns(hex+b0, "");
}

pieceIns public_mainprinter::drawoneBarcode(const QByteArray &hex, const QString &code)
{
    QByteArray b0;
    QByteArray b1=QByteArray::number(stringToHex(code).length()/2,16);
    b0.append("1D6B45");
    b0.append(b1.count()==1?"0"+b1:b1);
    b0.append(stringToHex(code));
    b0.append("0A");

    return pieceIns(hex+b0, "");
}


blockIns public_mainprinter::setPrintCount(const blockIns &text, int printcount)
{
    blockIns b;
    while(printcount--){
        b+=text;
    }
    return b;
}

bool public_mainprinter::asynchPrint(const blockIns &printIns, const QString &path, bool isblock)
{
    QString error;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    asyn_Printer printer;
    if(!printer.toprint(conf.value("system_setting/dayinjixinghao").toString(),
                        path,
                        QString("115200"),
                        printIns,
                        1,
                        isblock
                        )){
        error=printer.lastError();
    }
    if(!error.isEmpty()) sendError(error, tr("打印信息"));
    return error.isEmpty();
}

bool public_mainprinter::synchPrint(const blockIns &printIns, const QString &path)
{
    return asynchPrint(printIns, path, true);
}

void public_mainprinter::sendError(const QString &err, const QString &title)
{
    emit errorString(err, title);
}

QString public_mainprinter::stringToHex(QString str)
{
    QByteArray arr;
    QString s;

    //获取app的编码
    QByteArray pre_codec=QTextCodec::codecForLocale()->name();
    //设置为打印机可以识别的编码
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gb2312"));

    arr.append(str);

    QString bitString;
    for(int i = 0; i < str.count(); ++i)
    {
        bitString = QString(str.at(i)).toLocal8Bit().toHex();

        if(bitString.length() == 1)
            s += "0" + bitString;
        else
            s += bitString;
    }

    //还原
    QTextCodec::setCodecForLocale(QTextCodec::codecForName(pre_codec));
    return s;
}

