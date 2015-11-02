#ifndef GFROMUNICODE_H
#define GFROMUNICODE_H
#include <QByteArray>
#include <QString>
#include <QDebug>
#include <QStringList>
#include <QFile>
#ifdef Q_OS_UNIX
#include <stdio.h>
#include <iconv.h>
#endif
#include <QProcess>
#include <QTextCodec>
#include <QApplication>
#include "qextserialenumerator.h"

//求字符串的gbk的长度，汉字为3->2，字母为1->1
static int  size_g_of_u(const QString &in){
    int sum = 0;
    foreach(QChar c,in){
        if(QByteArray().append(c).size() == 1)
            sum +=1;
        else
            sum += 2;
    }
    return sum;
}

static int  code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen,
                         char *outbuf, size_t outlen) {
#ifdef Q_OS_UNIX

    iconv_t cd;
    char **pin = &inbuf;
    char **pout = &outbuf;

    cd = iconv_open(to_charset,from_charset);
    if (cd==0) return -1;
    memset(outbuf,0,outlen);
    if (iconv(cd,pin,&inlen,pout,&outlen)==-1){iconv_close(cd);return -1;}
    iconv_close(cd);
#endif
    return 0;
}

static int  u2g(char *inbuf, size_t inlen, char *outbuf, size_t outlen) //UNICODE码转为GB2312码
{
    return code_convert("utf-8", "gb2312", inbuf, inlen, outbuf, outlen);
}

//utf-8 to gbk
static QByteArray  trans_u2g(const QString &in)
{
    QByteArray b;
    //获取app的编码
    QByteArray pre_codec=QTextCodec::codecForLocale()->name();
    //设置为打印机可以识别的编码
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gb2312"));
    //转换
    b.append(in.toLocal8Bit());
    //还原
    QTextCodec::setCodecForLocale(QTextCodec::codecForName(pre_codec));
    return b;
}
static QString f_scan_dev_ups()
{
    qDebug() << "f_scan_dev_ups1";
    //获取
    QString ret;
    QProcess p;
    p.start("cat /dev/ups");
    if(p.waitForReadyRead()){
        ret= p.readAll().trimmed();
    }
    p.close();
    return ret;
}

static QStringList  f_scan_usbPort()
{
    QStringList proclist,outlist;

#ifdef Q_OS_UNIX

    if(!QFile::exists("/dev/usb")) return QStringList();

    //获取
    char buf_1[255] = {0};
    FILE *fd = popen("ls /dev/usb/*","r");
    if(fd < 0)return QStringList();
    fread(buf_1, sizeof(buf_1) - 1, 1,  fd);
    buf_1[sizeof(buf_1)] = '\0';
    pclose(fd);

    //过滤
    proclist = QString(buf_1).split("\n", QString::SkipEmptyParts);
    foreach(QString str,proclist){
        if(str.contains("/dev/usb/lp"))
            outlist<<str;
    }
#endif
    return outlist;
}

static QStringList  f_scan_ttyPort()
{
    QList<QextPortInfo> ports=QextSerialEnumerator::getPorts();
    QStringList portlist;
    foreach(QextPortInfo port, ports)portlist.append(port.physName);
    return portlist;
}

//汉字转区位码 : utf-8 to 区位码
//只支持汉字不包括 ：阿拉伯数字和字母
//字母 只包括ascii下的 ！到 } 之间打字符，包括 数字，大小写字母，以及大部分符号
static QByteArray  hanzi2quweima(const QString &hanzi)
{
    QByteArray b0;
    foreach(QChar c, hanzi){
        if(QByteArray().append(c).size() >= 3){
            QByteArray b = trans_u2g(c);
            //开始转换
            uint bb = b.toHex().toUInt(0, 16);//b.toHex();
            uint bb1 = 0x2020;//国标码
            uint bb2 = 0x8080;//机内码
            bb = bb - bb1 - bb2;

            unsigned p1 = (bb & 0xff00) >> 8;
            unsigned p2 = bb & 0x00ff;

            b0 +=  QString().sprintf("%02d%02d", p1, p2);//QByteArray::number(p1) + QByteArray::number(p2);
        } else {
            QByteArray b = QByteArray().append(c);
            if(QByteArray("!") <= b && b <= QByteArray("}")){
                b0 += QByteArray("0") + QByteArray::number(b.toHex().toInt(0,16) - 0x21 + 301);
            }
        }
    }
    return b0;
}

static char  _2xor(const QByteArray &data)
{
    char c0 = 0;
    foreach(char c, data){
        c0 ^= c;
    }
    return c0;
}

static QByteArray  _alignRight_fixedLen_int2bcd(const QString &data/*123*/, int len, char c){
    //将data直接反转换为16进制，右对齐，取len长度，不够补c（一个字节0x00之类的）
    return QByteArray::fromHex(QByteArray().append(data)).rightJustified(len, c,true);
}
static QByteArray  _alignRight_fixedLen_int2hex(const QString &data/*123*/, int len, char c){
    //将data转成16进制，右对齐，取len长度，不够补c（一个字节0x00之类的）
    return QByteArray::fromHex(QByteArray::number(data.toInt(),16)).rightJustified(len, c, true);
}
static QByteArray  _alignLeft_fixedLen_byte2byte(const QByteArray &data, int len, char c){
    //将data左对齐，取len长度，不够补c（一个字节0x00之类的）
    return data.leftJustified(len,c,true);
}
static QByteArray  _alignRight_fixedLen_byte2byte(const QByteArray &data, int len, char c){
    //将data左对齐，取len长度，不够补c（一个字节0x00之类的）
    return data.rightJustified(len,c,true);
}

static QString _fromlast_cutbetween(const QString &content, const QString &cmpvalue){
    //s=1000100001001abc10000
    //_fromlast_cutbetween(s,"1")  //return abc
    //_fromlast_cutbetween(s,"c")  //return 1000100001001ab
    int endindex= content.lastIndexOf(cmpvalue);
    int beginindex= content.lastIndexOf(cmpvalue, endindex-1);

    return content.mid(beginindex+cmpvalue.count(), endindex-beginindex-cmpvalue.count());
}

#endif // GFROMUNICODE_H
