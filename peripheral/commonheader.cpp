#include "commonheader.h"
#include "electronicscale_cas.h"
#include "electronicscale_dahua.h"
#include "electronicscale_ysheng.h"
#include "electronicscale_djian.h"
#include "electronicscale_kaifeng.h"
#include "trans_u2g.h"
#include "qextserialport.h"
#include <QSettings>
#include "printer_virtual.h"
#include "printer_postek.h"
#include "printer_argox_pplb.h"
#include "printer_argox_ppla.h"
#include "printer_tsc_ttp_244_plus.h"
#include "printer_gainscha.h"
#include "printer_pos_serial.h"
#include "printer_gainscha_2120tf.h"
#include "printer_pos_vidpid.h"
#include "printer_pos_net.h"

QStringList CommonHeader::normal_printtypes;
QStringList CommonHeader::normal_printpathtypes;

QStringList CommonHeader::pricecout_scaletypes;

QStringList CommonHeader::clientdisplay_ledtypes;

QStringList CommonHeader::printtypes;
QStringList CommonHeader::printpathtypes;

QStringList CommonHeader::barcode_scaletypes;
QString CommonHeader::com_ini_filepath;
QString CommonHeader::com_root_dirpath;
QString CommonHeader::barcode_scaleflag;

QMap<int, QString> CommonHeader::baudrates;
QMap<int, QString> CommonHeader::barcode_scale_weigettype;

Qt::LayoutDirection CommonHeader::com_widget_dir=Qt::LeftToRight;
QStandardItemModel CommonHeader::label_vidpid_model;

CommonHeader::CommonHeader()
{
}

CommonHeader::~CommonHeader()
{
}

QStringList CommonHeader::creat_printtypes()
{
    if(printtypes.isEmpty()){
        printtypes  << QObject::tr("POSTEK Q8/200") << QObject::tr("TSC TTP-244 PLUS")
                    << QObject::tr("SC2100(商超专用)") << QObject::tr("ARGOX CP-2140")
                    << QObject::tr("佳博GP-3120TN")
                    << QObject::tr("佳博GP-3150TIN")
                    << QObject::tr("佳博GP-2120TF")
                       ;
    }
    return printtypes;
}

QStandardItemModel &CommonHeader::creat_print_vidpidmodel()
{
    if(label_vidpid_model.rowCount()==0){
        label_vidpid_model.setHorizontalHeaderLabels(QStringList()<<QObject::tr("标签机名称")<<QObject::tr("打印机类型")<<QObject::tr("VIDPID")<<QObject::tr("描述"));
        label_vidpid_model.appendRow(QList<QStandardItem*>()
                                     <<new QStandardItem("p1")
                                     <<new QStandardItem("佳博GP-3120TN")
                                     <<new QStandardItem("0471&0017")
                                     <<new QStandardItem("")
                                     );
    }
    return label_vidpid_model;
}

QStringList CommonHeader::creat_printpathtypes()
{
    /**
USB TO  f_scan_usbPort
串口哦 TO  f_scan_ttyPort
*/
    if(printpathtypes.isEmpty()){
        printpathtypes << QObject::tr("USB") << QObject::tr("串口")<< QObject::tr("网口");
    }
    return printpathtypes;
}

QMap<int, QString> CommonHeader::creat_baudrates()
{
    if(baudrates.isEmpty()){
        baudrates.insert(BAUD110, "110");
        baudrates.insert(BAUD300, "300");
        baudrates.insert(BAUD600, "600");
        baudrates.insert(BAUD1200, "1200");
        baudrates.insert(BAUD2400, "2400");
        baudrates.insert(BAUD4800, "4800");
        baudrates.insert(BAUD9600, "9600");
        baudrates.insert(BAUD19200, "19200");
        baudrates.insert(BAUD38400, "38400");
        baudrates.insert(BAUD57600, "57600");
        baudrates.insert(BAUD115200, "115200");
    }
    return baudrates;
}

Printer_Virtual *CommonHeader::getlabelprint_of_type(const QString &printtype)
{
    Printer_Virtual *printer=0;
    switch(creat_printtypes().indexOf(printtype)){
    case 0:printer=new Printer_POSTEK;break;
    case 1:printer=new printer_TSC_TTP_244_PLUS;break;
    case 2:printer=new Printer_ARGOX_PPLA;break;
    case 3:printer=new Printer_ARGOX_PPLB;break;
    case 4:
    case 5:printer=new Printer_GAINSCHA;break;
    case 6:printer=new Printer_GAINSCHA_2120TF;break;
    }
    return printer;
}


QStringList CommonHeader::creat_normal_printtypes()
{
    if(normal_printtypes.isEmpty()){
        normal_printtypes << QObject::tr("58mm热敏") << QObject::tr("80mm热敏") << QObject::tr("KF58") << QObject::tr("MINI58");
        //T2US 说明使用pidvid进行打印; pidvid需要调用Printer_Normal_T2US().vidpid获得
        //路径选择vidpid后,直接使用asyn_Printer即可
    }
    return normal_printtypes;
}

QStringList CommonHeader::creat_pricecount_scaletypes()
{
    if(pricecout_scaletypes.isEmpty()){
        pricecout_scaletypes << QObject::tr("凯丰(商超专用)")
                             << QObject::tr("凯士PR PLUS")
                             << QObject::tr("KF-PC秤")
                             << QObject::tr("顶尖PBX")
                             << QObject::tr("大华ASC-A")
                             << QObject::tr("顶尖PS1X")
                             << QObject::tr("风华电子称")
                             << QObject::tr("友中衡计价秤")

                                ;

    }
    return pricecout_scaletypes;
}

QStringList CommonHeader::creat_clientdisplay_ledtypes()
{
    if(clientdisplay_ledtypes.isEmpty()){
        clientdisplay_ledtypes << "LED-8"<<"VFD220";
    }
    return clientdisplay_ledtypes;
}

QStringList CommonHeader::creat_normal_printpathtypes()
{

    if(normal_printpathtypes.isEmpty()){
        normal_printpathtypes << QObject::tr("USB") << QObject::tr("串口") << QObject::tr("网口");
    }
    return normal_printpathtypes;

}

Printer_POS_Virtual *CommonHeader::get_variantprinter(const QString &path, int ttybaudrate)
{
    Printer_POS_Virtual *printer=0;
    if(path.contains("tty")){
        printer=new Printer_POS_SERIAL;
        static_cast<Printer_POS_SERIAL *>(printer)->psettings.BaudRate=BaudRateType(ttybaudrate);
    } else
    if(path.contains("usb")){
        printer=new Printer_POS_SERIAL;
        static_cast<Printer_POS_SERIAL *>(printer)->setType(Printer_POS_SERIAL::USB);
    } else
    if(path.contains("&")){
        printer=new Printer_POS_VIDPID;
    }
    if(path.contains(":")){
        printer=new Printer_POS_NET;
    }

    return printer;
}

QStringList CommonHeader::creat_barcode_scaletypes()
{
    if(barcode_scaletypes.isEmpty()){
        barcode_scaletypes << QObject::tr("上海大华 TM") << QObject::tr("上海凯士 CL5200")
                           << QObject::tr("上海精涵友声 TM") << QObject::tr("厦门顶尖LS2CX")
                           << QObject::tr("凯丰条码秤");
    }
    return barcode_scaletypes;
}


QMap<int, QString> CommonHeader::creat_barcode_scale_weigettype(const QString &type)
{
    barcode_scale_weigettype.clear();
    switch(creat_barcode_scaletypes().indexOf(type)){
    case 0://上海大华 TM-15H
        barcode_scale_weigettype = ElectronicScale_DAHUA::WeigetType();
        break;
    case 1://凯士 CL5200
        barcode_scale_weigettype = ElectronicScale_CAS::WeigetType();
        break;
    case 2://友声
        barcode_scale_weigettype = ElectronicScale_YSHENG::WeigetType();
        break;
    case 3://厦门顶尖LS2CX
        barcode_scale_weigettype = ElectronicScale_DJIAN::WeigetType();
        break;
    case 4://凯丰条码秤
        barcode_scale_weigettype = ElectronicScale_KAIFENG::WeigetType();
        break;
    }
    return barcode_scale_weigettype;
}

QString CommonHeader::creat_barcode_scale_weigettype_init(const QString &type)
{
    int value = 0;
    QList<int > int_keys;
    switch(creat_barcode_scaletypes().indexOf(type)){
    case 0://上海大华 TM-15H
        int_keys = ElectronicScale_DAHUA::WeigetType().keys();
        break;
    case 1://凯士 CL5200
        int_keys = ElectronicScale_CAS::WeigetType().keys();
        break;
    case 2://友声
        int_keys = ElectronicScale_YSHENG::WeigetType().keys();
        break;
    case 3://厦门顶尖LS2CX
        int_keys = ElectronicScale_DJIAN::WeigetType().keys();
        break;
    case 4://凯丰条码秤
        int_keys = ElectronicScale_KAIFENG::WeigetType().keys();
        break;
    }
    if(!int_keys.isEmpty()){
        value = int_keys.value(0);
    }
    return QString::number(value);
}

QString CommonHeader::creat_barcode_scale_netport_init(const QString &type)
{
    QString value;
    switch(creat_barcode_scaletypes().indexOf(type)){
    case 0://上海大华 TM-15H
        value = "4001";
        break;
    case 1://凯士 CL5200
        value = "20304";
        break;
    case 2://友声
        value = "33581";
        break;
    case 3://厦门顶尖LS2CX
        value="6275";
        break;
    case 4://凯丰条码秤
        value = "5100";
        break;
    }
    return value;
}

QStringList CommonHeader::creat_barcode_scalebarcodetype(const QString &type)
{
    QStringList value;
    switch(creat_barcode_scaletypes().indexOf(type)){
    case 0://上海大华 TM-15H
        value = ElectronicScale_DAHUA::BarType();
        break;
    case 1://凯士 CL5200
        value = ElectronicScale_CAS::BarType();
        break;
    case 2://友声
        value = ElectronicScale_YSHENG::BarType();
        break;
    case 3://厦门顶尖LS2CX
        value = ElectronicScale_DJIAN::BarType();
        break;
    case 4://凯丰条码秤
        value = ElectronicScale_KAIFENG::BarType();
        break;
    }
    return value;
}

bool CommonHeader::barcode_scale_resolve(QString &barcode, QString &price, QString &weight, QString &amount, const QString &code)
{
    QSettings conf(com_ini_filepath, QSettings::IniFormat);
    QString flag=conf.value("barscale/scaleflag").toString();
    QString type=conf.value("barscale/scalebartype").toString();
    return code.startsWith(flag)&&
            (ElectronicScale_DAHUA::barcode_scale_resolve(barcode,price,weight,amount,code,type)
             || ElectronicScale_CAS::barcode_scale_resolve(barcode,price,weight,amount,code,type)
             || ElectronicScale_YSHENG::barcode_scale_resolve(barcode,price,weight,amount,code,type)
             || ElectronicScale_DJIAN::barcode_scale_resolve(barcode,price,weight,amount,code,type)
             || ElectronicScale_KAIFENG::barcode_scale_resolve(barcode,price,weight,amount,code,type))
            ;
}

QString CommonHeader::get_barcode_scaleflag()
{
    if(barcode_scaleflag.isEmpty()){
        QSettings conf(com_ini_filepath, QSettings::IniFormat);
        QString flag=conf.value("barscale/scaleflag").toString();
        barcode_scaleflag=flag;
    }
    return barcode_scaleflag;
}
