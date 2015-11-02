#include "labelprint_design_dialog.h"
#include "ui_labelprint_design_dialog.h"
#include "ui_labelprint_design_save_dialog.h"
#include "commonheader.h"
#include <QDir>
#include<QMessageBox>
#include "ui_labelprint_design_import_dialog.h"
#include"commonheader.h"
#include<QSettings>

#include "printer_postek.h"
#include "printer_argox_pplb.h"
#include "printer_argox_ppla.h"
#include "printer_tsc_ttp_244_plus.h"
#include "printer_gainscha_2120tf.h"
#include "printer_pos_serial.h"
#include "uinormal_message.h"
#include "printer_pos_net.h"
#include "s_mainwindow.h"
#include "labelprint_design_import_dialog.h"

labelprint_design_dialog::labelprint_design_dialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::labelprint_design_dialog)
{
    ui->setupUi(this);setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);this->setLayoutDirection(CommonHeader::com_widget_dir);
    this->setWindowTitle(tr("设计"));
    design=static_cast<S_MainWindow*>(ui->stackedWidget->widget(0));
    setdialog=static_cast<labelprint_set_tagbarcode_dialog*>(ui->stackedWidget->widget(1));

    ui->stackedWidget->setCurrentIndex(0);
    ui->pushButton_switch->setText(tr("-->预览设置"));

    connect(ui->pushButton_switch,SIGNAL(clicked()),this,SLOT(toswitch()));
    connect(ui->pushButton_defbill,SIGNAL(clicked()),this,SLOT(todefbill()));
    connect(ui->pushButton_importbill,SIGNAL(clicked()),this,SLOT(toimportbill()));
    connect(ui->pushButton_savebill,SIGNAL(clicked()),this,SLOT(tosavebill()));
    connect(ui->pushButton_test,SIGNAL(clicked()),this,SLOT(totestprint()));
    toload();
}

labelprint_design_dialog::~labelprint_design_dialog()
{
    delete ui;
}

void labelprint_design_dialog::tosave()
{
    SSRPairMap ssrmap=design->printMap();
    QByteArray ba;
    QDataStream in(&ba, QIODevice::WriteOnly);    // read the data serialized from the file
    QStringList names = ssrmap.keys();
    foreach(QString name, names){
        in << name <<  ssrmap[name].value(0).toInt() <<  ssrmap[name].value(1).toRectF() <<  ssrmap[name].value(2).toString()
           <<ssrmap[name].value(3).toBool();
    }

    QFile file(CommonHeader::com_root_dirpath+"/def.txt");
    if(!file.open(QFile::WriteOnly)){
        uinormal_message::warning(this, tr("标价签:"), tr("标价签错误:")+tr("保存失败")+file.errorString());
        return;
    }
    file.write(ba);
    file.close();
}

void labelprint_design_dialog::toload()
{
    QFile file(CommonHeader::com_root_dirpath+"/def.txt");
    !file.open(QFile::ReadOnly);

    QByteArray ba = file.readAll();
    QDataStream in(&ba, QIODevice::ReadOnly);    // read the data serialized from the file
    SSRPairMap ssrmap;
    while(!in.atEnd()){
        QString name;
        int t;
        QRectF r;
        QString fonts;
        bool istext;
        in >> name >> t >> r >> fonts>>istext;
        ssrmap.insert(name, QVariantList() << t << r << fonts<<istext);
    }
    file.close();
    design->initDocument(ssrmap);
}

void labelprint_design_dialog::toswitch()
{
    switch(ui->stackedWidget->currentIndex()){
    case 0:{
        ui->stackedWidget->setCurrentIndex(1);
        ui->pushButton_switch->setText(tr("<--预览"));
    }
        break;
    case 1:
        ui->stackedWidget->setCurrentIndex(0);
        ui->pushButton_switch->setText(tr("-->预览设置"));

        //需要[保存]然后重新[导出]
        setdialog_save();
        break;
    }
}

void labelprint_design_dialog::todefbill()
{
    SSRPairMap restore_ssrmap;
    restore_ssrmap.insert(tr("商品名称"),QVariantList() << (int)Shape::Rectangle << QRect(400,300,100,30) << QString("1"));//
    restore_ssrmap.insert(tr("条码1"),QVariantList() << (int)Shape::Rectangle << QRect(650,250,100,30) << QString("1"));
    restore_ssrmap.insert(tr("规格"),QVariantList() << (int)Shape::Rectangle << QRect(400,250,100,30) << QString("1"));
    restore_ssrmap.insert(tr("单位"),QVariantList() << (int)Shape::Rectangle << QRect(400,100,100,30) << QString("1"));

    restore_ssrmap.insert(tr("现售价"),QVariantList() << (int)Shape::Rectangle << QRect(250,180,100,30)<< QString("1"));
    restore_ssrmap.insert(tr("产地"),QVariantList() << (int)Shape::Rectangle << QRect(400,200,100,30) << QString("1"));
    restore_ssrmap.insert(tr("条码2"),QVariantList() << (int)Shape::OneBarCode << QRect(650,200,100,30) << QString("1"));

    design->initDocument(restore_ssrmap);
}

void labelprint_design_dialog::toimportbill()
{
    labelprint_design_import_dialog dialog(this);
    dialog.updatecom(CommonHeader::com_root_dirpath + "/localdata/labelprinterform");

    //! ========================
    if(QDialog::Accepted!=dialog.exec()) return;

    if(dialog.getfilename().isEmpty()){
        uinormal_message::warning(this, tr("标价签:"), tr("没有找到文件"));
        return;
    }

    QFile file(dialog.getfilename());
    if(!file.open(QFile::ReadOnly)){
        uinormal_message::warning(this, tr("标价签:"), tr("标价签错误:")+tr("导入失败")+file.errorString());
        return;
    }

    QByteArray ba = file.readAll();
    QDataStream in(&ba, QIODevice::ReadOnly);    // read the data serialized from the file
    SSRPairMap ssrmap;
    while(!in.atEnd()){
        QString name;
        int t;
        QRect r;
        QString fonts;
        in >> name >> t >> r >> fonts;
        ssrmap.insert(name, QVariantList() << t << r << fonts);
    }
    file.close();
    design->initDocument(ssrmap);
}

void labelprint_design_dialog::tosavebill()
{
    Ui_labelprint_design_save_dialog sui;
    QDialog dialog(this);
    dialog.setWindowTitle(tr("保存表单"));
    sui.setupUi(&dialog);dialog.setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);dialog.setLayoutDirection(CommonHeader::com_widget_dir);
    //! ========================
    if(QDialog::Accepted!=dialog.exec()) return;

    if(sui.lineEdit_filename->text().isEmpty()){
        uinormal_message::warning(this, tr("标价签:"), tr("标价签错误:")+tr("文件名为空"));
        return;
    }
    QDir dir(CommonHeader::com_root_dirpath + "/localdata/labelprinterform");
    if(!dir.exists()){
        dir.mkpath(dir.path());
    }
    QString formname=dir.path()+"/"+sui.lineEdit_filename->text();
    QFileInfoList fileinfos=dir.entryInfoList();
    QStringList filenams;
    foreach(QFileInfo info, fileinfos){
        if(info.fileName()=="."
                ||info.fileName()=="..")continue;
        filenams.append(info.fileName());
    }

    if( (filenams.contains(sui.lineEdit_filename->text()) && uinormal_message::warning(this, tr("标价签:"), tr("标价签:")+tr("有同名文件,是否覆盖?"), tr("确定"), tr("取消"))==0)
            || !filenams.contains(sui.lineEdit_filename->text())
            ){
        SSRPairMap ssrmap=design->printMap();
        QByteArray ba;
        QDataStream in(&ba, QIODevice::WriteOnly);    // read the data serialized from the file
        QStringList names = ssrmap.keys();
        foreach(QString name, names){
            in << name <<  ssrmap[name].value(0).toInt() <<  ssrmap[name].value(1).toRect() <<  ssrmap[name].value(2).toString();
        }

        QFile file(formname);
        if(!file.open(QFile::WriteOnly)){
            uinormal_message::warning(this, tr("标价签:"), tr("标价签错误:")+tr("保存失败")+file.errorString());
            return;
        }
        file.write(ba);
        file.close();
        uinormal_message::warning(this, tr("标价签:"), tr("标价签:")+tr("保存成功"));
    }
}

void labelprint_design_dialog::totestprint()
{
    toprint(true);
}

void labelprint_design_dialog::toprint(bool istest, bool pop)
{
    setdialog_save();

    if(design->printmodel->rowCount() <= 0){
        if(pop)uinormal_message::warning(this, tr("标价签:"), tr("标价签错误:")+(istest?tr("测试至少有一条数据"):tr("没有需要打印的数据")));
        return;
    }
    QSettings conf(CommonHeader::com_ini_filepath,QSettings::IniFormat);

    //! 获取打印接口类型； 并且打开打印机
    QString printpathtype = conf.value(QString("label_print/printpathtype"), "USB").toString();
    QString printpath = conf.value(QString("label_print/printpath"), "/dev/usb/lp0").toString();
    QString baudrate_value = conf.value(QString("label_print/baudrate"),"9600").toString();
    Printer_POS_Virtual *toprinter=0;
    toprinter=CommonHeader::get_variantprinter(printpath,CommonHeader::creat_baudrates().key(baudrate_value));
    if(toprinter==0){
        uinormal_message::warning(this, tr("标价签:"), tr("标价签错误:")+"no printer is selected!");
        return;
    }
    if(!toprinter->tryOpen(printpath)){
        uinormal_message::warning(this, tr("标价签:"), tr("标价签错误:")+toprinter->lastError());
        return;
    }


    //! 执行打印
    bool istag = conf.value(QString("label_print/istag"),true).toBool();
    int _gap = conf.value(QString("label_print/gap"), "0").toInt();
    float _height = conf.value(QString("label_print/height"), "320").toFloat();
    float _width = conf.value(QString("label_print/width"),"720").toFloat();
    int column = conf.value(QString("label_print/column"),"2").toInt();
    int print_count = /*conf.value(QString("label_print/printcount"), "1").toInt()*/1;

    float width = valueTranslate(_width,"dots","200DPI");
    float gap = valueTranslate(_gap,"dots","200DPI");
    float height = valueTranslate(_height,"dots","200DPI");

    int piece_width = width / column;
    if(istag){
        column = 1;
    }

    QString printtype = conf.value(QString("label_print/printtype"), "TSC TTP-244 PLUS").toString();
    switch(CommonHeader::creat_printtypes().indexOf(printtype)){
    case 0://POSTEK Q8/200
    {
        Printer_POSTEK printer;
        QVector<QVector<QString> > vvs;
        for(int rr = 0, rr_cout = design->printmodel->rowCount(),actual_row = 0; rr < rr_cout; rr++){
            for(int actual_r = 0, actual_count = design->printmodel->item(rr, 0)->text().toInt()
                ; actual_r < actual_count; actual_r++, actual_row++){
                int cc=actual_row%column;
                int rrr=actual_row/column;
                if(cc==0)vvs.append(QVector<QString>(column));
                vvs[rrr][cc]= print_piece_POSTEK(printer, rr,piece_width * (actual_row % column));
            }
            if(istest) break;//若是测试则打印一次后，直接跳出
        }

        foreach(QVector<QString> vs, vvs){
            QString rowins;
            foreach(QString s, vs){
                rowins+=s;
            }
            toprinter->write(printer.toPrint(rowins, print_count,height, gap));
        }
    }
        break;
    case 1://TSC TTP-244 PLUS"
    {
        width = valueTranslate(_width,"mm","200DPI");
        gap = valueTranslate(_gap,"mm","200DPI");
        height = valueTranslate(_height,"mm","200DPI");

        printer_TSC_TTP_244_PLUS printer;
        QVector<QVector<QString> > vvs;
        for(int rr = 0, rr_cout = design->printmodel->rowCount(),actual_row = 0; rr < rr_cout; rr++){
            for(int actual_r = 0, actual_count = design->printmodel->item(rr, 0)->text().toInt()
                ; actual_r < actual_count; actual_r++, actual_row++){
                int cc=actual_row%column;
                int rrr=actual_row/column;
                if(cc==0)vvs.append(QVector<QString>(column));
                vvs[rrr][cc]= print_piece_TSC(printer, rr,piece_width * (actual_row % column));

            }
            if(istest) break;//若是测试则打印一次后，直接跳出
        }
        foreach(QVector<QString> vs, vvs){
            QString rowins;
            foreach(QString s, vs){
                rowins+=s;
            }
            toprinter->write(printer.toPrint(rowins, print_count,QString::number(height), gap,QString::number(width)));
        }
    }
        break;
    case 2://SC2100(商超专用)
    {
        /*由于flash大小有限，需要外置sd卡才可以打印中文，这儿暂时不做*/
        Printer_ARGOX_PPLA printer;
        QVector<QVector<QString> > vvs;
        for(int rr = 0, rr_cout = design->printmodel->rowCount(),actual_row = 0; rr < rr_cout; rr++){
            for(int actual_r = 0, actual_count = design->printmodel->item(rr, 0)->text().toInt()
                ; actual_r < actual_count; actual_r++, actual_row++){
                int cc=actual_row%column;
                int rrr=actual_row/column;
                if(cc==0)vvs.append(QVector<QString>(column));
                vvs[rrr][cc]= print_piece_ARGOX_PPLA(printer, rr,piece_width * (actual_row % column));
            }
            if(istest) break;//若是测试则打印一次后，直接跳出
        }
        QString ins;
        foreach(QVector<QString> vs, vvs){
            QString rowins;
            foreach(QString s, vs){
                rowins+=s;
            }
            toprinter->write(printer.toPrint(rowins, print_count));
        }
    }
        break;
    case 3://ARGOX CP-2140
    {
        Printer_ARGOX_PPLB printer;
        QVector<QVector<QString> > vvs;
        for(int rr = 0, rr_cout = design->printmodel->rowCount(),actual_row = 0; rr < rr_cout; rr++){
            for(int actual_r = 0, actual_count = design->printmodel->item(rr, 0)->text().toInt()
                ; actual_r < actual_count; actual_r++, actual_row++){
                int cc=actual_row%column;
                int rrr=actual_row/column;
                if(cc==0)vvs.append(QVector<QString>(column));
                vvs[rrr][cc]= print_piece_ARGOX_PPLB(printer, rr,piece_width * (actual_row % column));
            }
            if(istest) break;//若是测试则打印一次后，直接跳出
        }
        QString ins;
        foreach(QVector<QString> vs, vvs){
            QString rowins;
            foreach(QString s, vs){
                rowins+=s;
            }
            toprinter->write(printer.toPrint(rowins, print_count));
        }
    }
        break;
    case 4://佳博GP-3120TN
    case 5://佳博GP-3150TIN
    {
        width = valueTranslate(_width,"mm","200DPI");
        gap = valueTranslate(_gap,"mm","200DPI");
        height = valueTranslate(_height,"mm","200DPI");

        Printer_GAINSCHA printer;
        QVector<QVector<QString> > vvs;

        for(int rr = 0, rr_cout = design->printmodel->rowCount(),actual_row = 0; rr < rr_cout; rr++){
            for(int actual_r = 0, actual_count = design->printmodel->item(rr, 0)->text().toInt()
                ; actual_r < actual_count; actual_r++, actual_row++){
                int cc=actual_row%column;
                int rrr=actual_row/column;
                if(cc==0)vvs.append(QVector<QString>(column));
                vvs[rrr][cc]= print_piece_GAINSCHA(printer, rr,piece_width * (actual_row % column));

            }
            if(istest) break;//若是测试则打印一次后，直接跳出
        }
        QString ins;
        foreach(QVector<QString> vs, vvs){
            QString rowins;
            foreach(QString s, vs){
                rowins+=s;
            }
            toprinter->write(printer.toPrint(rowins, print_count,QString::number(height), gap,QString::number(width)));
        }
    }
        break;
    case 6://佳博GP-2120TF
    {
        width = valueTranslate(_width,"mm","200DPI");
        gap = valueTranslate(_gap,"mm","200DPI");
        height = valueTranslate(_height,"mm","200DPI");

        height=(height*1.0/25.4);
        width=width*1.0/25.4;

        Printer_GAINSCHA_2120TF printer;
        QVector<QVector<QString> > vvs;
        for(int rr = 0, rr_cout = design->printmodel->rowCount(),actual_row = 0; rr < rr_cout; rr++){
            for(int actual_r = 0, actual_count = design->printmodel->item(rr, 0)->text().toInt()
                ; actual_r < actual_count; actual_r++, actual_row++){
                int cc=actual_row%column;
                int rrr=actual_row/column;
                if(cc==0)vvs.append(QVector<QString>(column));
                vvs[rrr][cc]= print_piece_GAINSCHA(printer, rr,piece_width * (actual_row % column));

            }
            if(istest) break;//若是测试则打印一次后，直接跳出
        }
        QString ins;
        foreach(QVector<QString> vs, vvs){
            QString rowins;
            foreach(QString s, vs){
                rowins+=s;
            }
            toprinter->write(printer.toPrint(rowins, print_count,height, gap,width));
        }
    }
        break;
    default :break;
    }
    toprinter->close();
    if(pop)uinormal_message::warning(this, tr("标价签:"),tr("标价签:")+tr("打印成功"));
}


QString labelprint_design_dialog::print_piece_POSTEK(Printer_POSTEK &printer, int rr, int start_x)
{
    if(rr >= design->printmodel->rowCount()) return QByteArray();//当前行数超过总行数

    //获取 打印机设置属性 ； 根据属性将ssrmap的内容转换成需要打格式:[dots 、200dpi]
    SSRPairMap map = ssrmapTranslate("dots", "200DPI", LEFTTOP);
    QStringList names = map.keys();
    QString ins;
    SSMap ssmap;
    Shape::Type type;
    SSMap::iterator itr;
    QString text;
    QString key;
    QPoint p;
    int size;
    int h;
    bool istext;
    QSettings conf(CommonHeader::com_ini_filepath,QSettings::IniFormat);
    int wide_width = conf.value(QString("label_print/wide_width"),"2").toInt();
    int narrow_width = conf.value(QString("label_print/narrow_width"),"2").toInt();
    for(int c = 0, cout = design->printmodel->columnCount() ; c < cout; c++){
        ssmap.insert(design->printmodel->horizontalHeaderItem(c)->text(), design->printmodel->item(rr, c)->text());
    }

    QString onebartype_key = conf.value(QString("label_print/onebartype"), "Code 128 AUTO").toString();
    QString font_key = conf.value(QString("label_print/font"), "FONT001").toString();

    QString onebartype = Printer_POSTEK::oneBarType_of_POSTEK().value(onebartype_key);
    QString font = Printer_POSTEK::textfont_of_POSTEK().value(font_key);
    if(font.isEmpty()) font = font_key; //若是没有在原字体里找到，则是新定义字体

    foreach(QString name, names){
        if(!ssmap.contains(name)) continue;//包含关键字
        type = (Shape::Type)map[name].value(0).toInt();//三种类别的一种
        itr=ssmap.find(name);
        text = itr.value();
        key = itr.key();
        p = map[name].value(1).toRect().topLeft();//坐标
        size = map[name].value(2).toInt();//字体放大倍数
        h = map[name].value(1).toRect().height();//高度
        istext=map[name].value(3).toBool();
        switch(type){
        case Shape::Rectangle:
            ins += printer.drawText(
                        (istext?(key+":"):"")+text
                        , p.x() + start_x
                        , p.y()
                        , font
                        , Printer_POSTEK::ROTATE_0
                        , size
                        , size
                        );
            break;
        case Shape::TwoBarCode:
            ins += printer.drawTwoCode( text
                                        , p.x() + start_x
                                        , p.y()
                                        , "QR"
                                        , 0
                                        , 0
                                        , Printer_POSTEK::ROTATE_0);
            break;
        case Shape::OneBarCode:
            ins += printer.drawOneCode(text
                                       , p.x() + start_x
                                       , p.y()
                                       , onebartype
                                       , h

                                       , Printer_POSTEK::ROTATE_0
                                       , wide_width
                                       , narrow_width
                                       );//ONEBARCODE
            break;
        default:break;
        }
    }

    return ins;

}

QString labelprint_design_dialog::print_piece_TSC(printer_TSC_TTP_244_PLUS &printer, int rr, int start_x)
{
    if(rr >= design->printmodel->rowCount()) return QByteArray();//当前行数超过总行数

    //获取 打印机设置属性 ； 根据属性将ssrmap的内容转换成需要打格式:[mm 、200dpi]
    SSRPairMap map = ssrmapTranslate("dots", "200DPI", LEFTTOP);
    QStringList names = map.keys();
    QSettings conf(CommonHeader::com_ini_filepath,QSettings::IniFormat);
    int wide_width = conf.value(QString("label_print/wide_width"),"2").toInt();
    int narrow_width = conf.value(QString("label_print/narrow_width"),"2").toInt();

    QString ins;


    SSMap ssmap;
    for(int c = 0, cout = design->printmodel->columnCount() ; c < cout; c++){
        ssmap.insert(design->printmodel->horizontalHeaderItem(c)->text(), design->printmodel->item(rr, c)->text());
    }

    QString onebartype_key = conf.value(QString("label_print/onebartype"), "Code 128 AUTO").toString();
    QString font_key = conf.value(QString("label_print/font"), "FONT001").toString();

    QString onebartype = printer_TSC_TTP_244_PLUS::oneBarType_of_TSC().value(onebartype_key);
    QString font = printer_TSC_TTP_244_PLUS::textfont_of_TSC().value(font_key);
    if(font.isEmpty()) font = font_key; //若是没有在原字体里找到，则是新定义字体

    foreach(QString name, names){
        if(!ssmap.contains(name)) continue;//包含关键字
        Shape::Type type = (Shape::Type)map[name].value(0).toInt();
        SSMap::iterator itr=ssmap.find(name);
        QString text = itr.value();
        QString key = itr.key();

        QPoint p = map[name].value(1).toRect().topLeft();//坐标

        int size = map[name].value(2).toInt();//字体大小
        int h = map[name].value(1).toRect().height();//高度
        bool istext=map[name].value(3).toBool();

        switch(type){
        case Shape::Rectangle:
            ins += printer.drawText(
                        (istext?(key+":"):"")+text
                        , p.x() + start_x
                        , p.y()
                        , font

                        , QSize(size, size)
                        );
            break;
        case Shape::TwoBarCode:
            ins += printer.drawTwoCode(text
                                       , p.x() + start_x
                                       , p.y()

                                       );
            break;
        case Shape::OneBarCode:
            ins += printer.drawOneCode(text
                                       , p.x() + start_x
                                       , p.y()
                                       , onebartype
                                       , h
                                       , printer_TSC_TTP_244_PLUS::ROTATE_0
                                       , wide_width
                                       , narrow_width
                                       );//ONEBARCODE
            break;
        default:break;
        }
    }
    return ins;
}

QString labelprint_design_dialog::print_piece_ARGOX_PPLA(Printer_ARGOX_PPLA &printer, int rr, int start_x)
{
    if(rr >= design->printmodel->rowCount()) return QByteArray();//当前行数超过总行数

    //获取 打印机设置属性 ； 根据属性将ssrmap的内容转换成需要打格式:[dots 、200dpi]
    SSRPairMap map = ssrmapTranslate("dots", "200DPI", LEFTTOP);
    QStringList names = map.keys();
    QSettings conf(CommonHeader::com_ini_filepath,QSettings::IniFormat);
    int wide_width = conf.value(QString("label_print/wide_width"),"2").toInt();
    int narrow_width = conf.value(QString("label_print/narrow_width"),"2").toInt();

    QString ins;


    SSMap ssmap;
    for(int c = 0, cout = design->printmodel->columnCount() ; c < cout; c++){
        ssmap.insert(design->printmodel->horizontalHeaderItem(c)->text(), design->printmodel->item(rr, c)->text());
    }

    QString onebartype_key = conf.value(QString("label_print/onebartype"), "Code 128 AUTO").toString();
    QString font_key = conf.value(QString("label_print/font"), "FONT001").toString();

    QString onebartype = Printer_ARGOX_PPLA::oneBarType_of_PPLA().value(onebartype_key);
    QString font = Printer_ARGOX_PPLA::textfont_of_PPLA().value(font_key);
    if(font.isEmpty()) font = font_key; //若是没有在原字体里找到，则是新定义字体

    foreach(QString name, names){
        if(!ssmap.contains(name)) continue;//包含关键字
        Shape::Type type = (Shape::Type)map[name].value(0).toInt();
        SSMap::iterator itr=ssmap.find(name);
        QString text = itr.value();
        QString key = itr.key();

        QPoint p = map[name].value(1).toRect().topLeft();//坐标
        int size = map[name].value(2).toInt();//字体放大倍数
        int h = map[name].value(1).toRect().height();//高度
        bool istext=map[name].value(3).toBool();

        switch(type){
        case Shape::Rectangle:
            ins += printer.drawText(
                        (istext?(key+":"):"")+text
                        , p.x() + start_x
                        , p.y()
                        , font

                        , Printer_ARGOX_PPLA::ROTATE_0
                        , size
                        , size
                        );
            break;
        case Shape::TwoBarCode:
            ins += printer.drawTwoCode(text
                                       , p.x() + start_x
                                       , p.y()
                                       , "Q"
                                       );
            break;
        case Shape::OneBarCode:
            ins += printer.drawOneCode(text
                                       , p.x() + start_x
                                       , p.y()
                                       , onebartype
                                       , h
                                       , Printer_ARGOX_PPLA::ROTATE_0
                                       , wide_width
                                       , narrow_width
                                       );//ONEBARCODE
            break;
        default:break;
        }
    }

    return ins;
}

QString labelprint_design_dialog::print_piece_ARGOX_PPLB(Printer_ARGOX_PPLB &printer, int rr, int start_x)
{
    if(rr >= design->printmodel->rowCount()) return QByteArray();//当前行数超过总行数

    //获取 打印机设置属性 ； 根据属性将ssrmap的内容转换成需要打格式:[dots 、200dpi]
    SSRPairMap map = ssrmapTranslate("dots", "200DPI", LEFTTOP);
    QStringList names = map.keys();
    QSettings conf(CommonHeader::com_ini_filepath,QSettings::IniFormat);
    int wide_width = conf.value(QString("label_print/wide_width"),"2").toInt();
    int narrow_width = conf.value(QString("label_print/narrow_width"),"2").toInt();

    QString ins;

    SSMap ssmap;
    for(int c = 0, cout = design->printmodel->columnCount() ; c < cout; c++){
        ssmap.insert(design->printmodel->horizontalHeaderItem(c)->text(), design->printmodel->item(rr, c)->text());
    }

    QString onebartype_key = conf.value(QString("label_print/onebartype"), "Code 128 AUTO").toString();
    QString font_key = conf.value(QString("label_print/font"), "FONT001").toString();

    QString onebartype = Printer_ARGOX_PPLB::oneBarType_of_PPLB().value(onebartype_key);
    QString font = Printer_ARGOX_PPLB::textfont_of_PPLB().value(font_key);
    if(font.isEmpty()) font = font_key; //若是没有在原字体里找到，则是新定义字体

    foreach(QString name, names){
        if(!ssmap.contains(name)) continue;//包含关键字
        Shape::Type type = (Shape::Type)map[name].value(0).toInt();
        SSMap::iterator itr=ssmap.find(name);
        QString text = itr.value();
        QString key = itr.key();

        QPoint p = map[name].value(1).toRect().topLeft();//坐标
        int size = map[name].value(2).toInt();//字体大小
        int h = map[name].value(1).toRect().height();//高度
        bool istext=map[name].value(3).toBool();
        switch(type){
        case Shape::Rectangle:
            ins += printer.drawText(
                        (istext?(key+":"):"")+text
                        , p.x() + start_x
                        , p.y()
                        , font

                        , Printer_ARGOX_PPLB::ROTATE_0
                        , size
                        , size
                        );
            break;
        case Shape::TwoBarCode:
            ins += printer.drawTwoCode(text
                                       , p.x() + start_x
                                       , p.y()
                                       , "Q"
                                       );
            break;
        case Shape::OneBarCode:
            ins += printer.drawOneCode(text
                                       , p.x() + start_x
                                       , p.y()
                                       , onebartype
                                       , h
                                       , Printer_ARGOX_PPLB::ROTATE_0
                                       , wide_width
                                       , narrow_width
                                       );//ONEBARCODE
            break;
        default:break;
        }
    }
    return ins;
}

QString labelprint_design_dialog::print_piece_GAINSCHA(Printer_GAINSCHA &printer, int rr, int start_x)
{
    if(rr >= design->printmodel->rowCount()) return QByteArray();//当前行数超过总行数

    //获取 打印机设置属性 ； 根据属性将ssrmap的内容转换成需要打格式:[mm 、200dpi]
    SSRPairMap map = ssrmapTranslate("dots", "200DPI", LEFTTOP);
    QStringList names = map.keys();
    QSettings conf(CommonHeader::com_ini_filepath,QSettings::IniFormat);
    int wide_width = conf.value(QString("label_print/wide_width"),"2").toInt();
    int narrow_width = conf.value(QString("label_print/narrow_width"),"2").toInt();

    QString ins;


    SSMap ssmap;
    for(int c = 0, cout = design->printmodel->columnCount() ; c < cout; c++){
        ssmap.insert(design->printmodel->horizontalHeaderItem(c)->text(), design->printmodel->item(rr, c)->text());
    }

    QString onebartype_key = conf.value(QString("label_print/onebartype"), "Code 128 AUTO").toString();
    QString font_key = conf.value(QString("label_print/font"), "FONT001").toString();

    QString onebartype = Printer_GAINSCHA::oneBarType_of_GAINSCHA().value(onebartype_key);
    QString font = Printer_GAINSCHA::textfont_of_GAINSCHA().value(font_key);
    if(font.isEmpty()) font = font_key; //若是没有在原字体里找到，则是新定义字体

    foreach(QString name, names){
        if(!ssmap.contains(name)) continue;//包含关键字
        Shape::Type type = (Shape::Type)map[name].value(0).toInt();
        SSMap::iterator itr=ssmap.find(name);
        QString text = itr.value();
        QString key = itr.key();

        QPoint p = map[name].value(1).toRect().topLeft();//坐标

        int size = map[name].value(2).toInt();//字体大小
        int h = map[name].value(1).toRect().height();//高度
        bool istext=map[name].value(3).toBool();

        qDebug() << text << istext;
        switch(type){
        case Shape::Rectangle:
            ins += printer.drawText(
                        (istext?(key+":"):"")+text
                        , p.x() + start_x
                        , p.y()
                        , font

                        , QSize(size, size)
                        );
            break;
        case Shape::TwoBarCode:
            ins += printer.drawTwoCode(text
                                       , p.x() + start_x
                                       , p.y()

                                       );
            break;
        case Shape::OneBarCode:
            ins += printer.drawOneCode(text
                                       , p.x() + start_x
                                       , p.y()
                                       , onebartype
                                       , h
                                       , Printer_GAINSCHA::ROTATE_0
                                       , wide_width
                                       , narrow_width
                                       );//ONEBARCODE
            break;
        default:break;
        }
    }
    return ins;
}

float labelprint_design_dialog::valueTranslate(float value, const QString &unit, const QString &rate)
{
    QSettings conf(CommonHeader::com_ini_filepath,QSettings::IniFormat);
    QString danwei = conf.value("label_print/danwei", "dots").toString();
    QString fengbianlv = conf.value("label_print/fengbianlv", "200DPI").toString();
    if(danwei.isEmpty() || fengbianlv.isEmpty()) return 0;

    float f1 = 1.0;
    if(danwei == "mm"){ //to [dots 200dpi]
        f1 = 8.0;
    } else  if(danwei == "dots" && fengbianlv == "200DPI"){
        f1 = 1.0;
    } else  if(danwei == "dots" && fengbianlv == "300DPI"){
        f1 = 2.0 / 3.0;
    }

    float f2 = 1.0;
    if(unit == "mm"){//[dots 200dpi] to [unit rate]
        f2 = 1.0 / 8.0;
    } else if(unit == "dots" && rate == "200DPI"){
        f2 = 1.0;
    } else if(unit == "dots" && rate == "300DPI"){
        f2 = 3.0 / 2.0;
    }

    qDebug() << value*f1*f2<<value<<f1<<f2;
    return value * f1 * f2;
}

SSRPairMap labelprint_design_dialog::ssrmapTranslate(const QString &unit, const QString &rate, DIRECT origin)
{
    QSettings conf(CommonHeader::com_ini_filepath,QSettings::IniFormat);
    SSRPairMap map = design->printMap();
    QStringList names = map.keys();
    QSize refsize(conf.value("label_print/width", "720").toInt(),
                  conf.value("label_print/height", "320").toInt());
    //根据配置文件获取 单位和分辨率；若是空，则返回空； 若和默认的一致，则返回原始
    QString danwei = conf.value("label_print/danwei", "dots").toString();
    QString fengbianlv = conf.value("label_print/fengbianlv", "200DPI").toString();

    if(!refsize.isValid()) return SSRPairMap();
    if(danwei.isEmpty() || fengbianlv.isEmpty()) return SSRPairMap();

    if(danwei == "mm"){//to dots 200dpi
        refsize *= 8;
    } else if(danwei == "dots" && fengbianlv == "200DPI"){
        refsize *= 1.0;
    } else if(danwei == "dots" && fengbianlv == "300DPI"){
        refsize *= 2.0 / 3.0;
    }

    double factor = 1.0;//[dots 200dpi] to [unit rate]
    if(unit == "mm" && rate == "200DPI"){
        factor = 1.0 / 8.0;
    } else if(unit == "mm" && rate == "300DPI"){
        factor = 1.0 / 8.0;
    } else if(unit == "dots" && rate == "200DPI"){
        factor = 1.0;
    } else if(unit == "dots" && rate == "300DPI"){
        factor = 3.0 / 2.0;
    }

    foreach(QString name, names){
        QRect rect(map[name].value(1).toRect());
        rectDiv(rect, factor, origin, refsize);
        map[name][1] = rect;
    }
    return map;
}

void labelprint_design_dialog::rectDiv(QRect &rect, double factor, DIRECT origin, const QSize &p1)
{
    int x = rect.x() * factor;
    int y = rect.y() * factor;
    int w = rect.width() *factor;
    int h = rect.height() * factor;
    QSize p = p1 * factor;

    switch(origin){
    case LEFTTOP:
        x = p.width() - x;
        y = p.height() - y;
        break;
    case RIGHTBOTTOM:

        break;
    }
    rect.setRect(x, y, w, h);
}

void labelprint_design_dialog::setdialog_save()
{
    setdialog->tosave();
    design->initDocument(design->printMap());
}

void labelprint_design_dialog::on_pushButton_importbill_clicked()
{

}
