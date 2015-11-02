#include "labelprint_set_dialog.h"
#include <QSettings>
#include <QMessageBox>
#include"commonheader.h"
#include "printer_postek.h"
#include "printer_argox_pplb.h"
#include "printer_argox_ppla.h"
#include "printer_tsc_ttp_244_plus.h"
#include "printer_gainscha.h"
#include "printer_pos_serial.h"
#include "printer_gainscha_2120tf.h"
#include "trans_u2g.h"
#include "uinormal_message.h"
#include "printer_pos_net.h"
#include "labelprint_set_tagbarcode_dialog.h"
#include "labelprint_set_vidpid_dialog.h"

labelprint_set_dialog::labelprint_set_dialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::labelprint_set_dialog)
{
    ui->setupUi(this);setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);this->setLayoutDirection(CommonHeader::com_widget_dir);
    this->setWindowTitle(tr("标签打印设置"));
    QSettings _Settings(CommonHeader::com_ini_filepath, QSettings::IniFormat);
    //标签打印机 labelprint_
    //标签打印机 labelprint_
    ui->comboBox_labelprint_printtype->addItems(CommonHeader::creat_printtypes());
    ui->comboBox_labelprint_printpathtype->addItems(CommonHeader::creat_printpathtypes());
    ui->comboBox_labelprint_baudrate->addItems(CommonHeader::creat_baudrates().values());

    update_combobox(ui->comboBox_labelprint_baudrate, _Settings.value("label_print/baudrate", "9600").toString());
    update_combobox(ui->comboBox_labelprint_printtype,_Settings.value("label_print/printtype", "TSC TTP-244 PLUS").toString());
    update_labelprint_onebartype(ui->comboBox_labelprint_printtype->currentText());
    update_combobox(ui->comboBox_labelprint_font, _Settings.value("label_print/font", "FONT001").toString());
    update_combobox(ui->comboBox_labelprint_onebartype,_Settings.value("label_print/onebartype", "Code 128 AUTO").toString());
    update_combobox(ui->comboBox_labelprint_printpathtype,_Settings.value("label_print/printpathtype", "USB").toString());
    update_labelprint_path(ui->comboBox_labelprint_printpathtype->currentText());
    update_combobox(ui->comboBox_labelprint_printpath,_Settings.value("label_print/printpath", "/dev/usb/lp0").toString());
    ui->spinBox_wide->setValue(_Settings.value("label_print/wide_width", "2").toInt());
    ui->spinBox_narrow->setValue(_Settings.value("label_print/narrow_width", "2").toInt());

    connect(ui->comboBox_labelprint_printpathtype,SIGNAL(currentIndexChanged(QString)),this,SLOT(update_labelprint_path(QString)));
    connect(ui->comboBox_labelprint_printtype,SIGNAL(currentIndexChanged(QString)),this,SLOT(update_labelprint_onebartype(QString)));
    connect(ui->pushButton_labelprint_test,SIGNAL(clicked()),this,SLOT(test_labelprint()));
    connect(ui->pushButton_labelprint_vidpid,SIGNAL(clicked()),this,SLOT(toportbyvidpid()));
}

labelprint_set_dialog::~labelprint_set_dialog()
{
    delete ui;
}

void labelprint_set_dialog::tosave()
{
    QSettings _Settings(CommonHeader::com_ini_filepath, QSettings::IniFormat);
    //标价签 打印机
    _Settings.setValue("label_print/baudrate", ui->comboBox_labelprint_baudrate->currentText());
    _Settings.setValue("label_print/printtype", ui->comboBox_labelprint_printtype->currentText());
    _Settings.setValue("label_print/font", ui->comboBox_labelprint_font->currentText());
    _Settings.setValue("label_print/onebartype", ui->comboBox_labelprint_onebartype->currentText());
    _Settings.setValue("label_print/printpathtype", ui->comboBox_labelprint_printpathtype->currentText());

    _Settings.setValue("label_print/printpath", ui->comboBox_labelprint_printpath->currentText());

    _Settings.setValue("label_print/wide_width", ui->spinBox_wide->value());

    _Settings.setValue("label_print/narrow_width", ui->spinBox_narrow->value());

    st_dialog->afterload();
}

void labelprint_set_dialog::toexit()
{
}

void labelprint_set_dialog::test_labelprint()
{
    QSettings conf(CommonHeader::com_ini_filepath, QSettings::IniFormat);
    QString baudrate_value = ui->comboBox_labelprint_baudrate->currentText();
    QString printcount = "1";
    QString printtype = ui->comboBox_labelprint_printtype->currentText();
    QString font_key = ui->comboBox_labelprint_font->currentText();

    QString onebartype_key = ui->comboBox_labelprint_onebartype->currentText();
    QString printpathtype = ui->comboBox_labelprint_printpathtype->currentText();
    QString printpath = ui->comboBox_labelprint_printpath->currentText();
    bool smartprint=conf.value("label_print/smartprint", true).toBool();

    int wide_width=ui->spinBox_wide->value();
    int narrow_width=ui->spinBox_narrow->value();
    //! 获取打印接口类型； 并且打开打印机
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


    //! 开始执行
    QString test_text = ui->lineEdit_labelprint_test_text->text();
    QString test_barcode = ui->lineEdit_labelprint_test_barcode->text();
    switch(CommonHeader::creat_printtypes().indexOf(printtype)){
    case 0://! POSTEK Q8/200 测试通过 字体随意 串口成功9600
    {
        Printer_POSTEK printer;
        QString onebartype = Printer_POSTEK::oneBarType_of_POSTEK().value(onebartype_key);
        QString font = Printer_POSTEK::textfont_of_POSTEK().value(font_key);
        if(font.isEmpty()) font = font_key; //若是没有在原字体里找到，则是新定义字体

        QString ins;
        ins += printer.drawText(test_text, 20, 20,font,Printer_POSTEK::ROTATE_0);
        ins += printer.drawOneCode(test_barcode,20,100,onebartype,20, Printer_POSTEK::ROTATE_0, wide_width, narrow_width);
        toprinter->write(printer.toPrint(ins, printcount.toInt(),248,40));
    }
        break;
    case 1://! TSC TTP-244 PLUS 测试通过 字体FONT001 串口成功9600  提示： toprint的高度不能为0，会认为时无限大
    {
        printer_TSC_TTP_244_PLUS printer;
        QString onebartype = printer_TSC_TTP_244_PLUS::oneBarType_of_TSC().value(onebartype_key);
        QString font = printer_TSC_TTP_244_PLUS::textfont_of_TSC().value(font_key);
        if(font.isEmpty()) font = font_key; //若是没有在原字体里找到，则是新定义字体

        QString ins;
        ins += printer.drawText(test_text, 20, 20,font);
        ins += printer.drawOneCode(test_barcode,20,100,onebartype,41, printer_TSC_TTP_244_PLUS::ROTATE_0, wide_width, narrow_width);

        toprinter->write(printer.toPrint(ins, printcount.toInt(),"30",0,"40"));
    }
        break;
    case 2://! SC2100(商超专用)测试通过 无串口
    {
        /*由于flash大小有限，需要外置sd卡才可以打印中文，这儿暂时不做*/
        Printer_ARGOX_PPLA printer;
        QString onebartype = Printer_ARGOX_PPLA::oneBarType_of_PPLA().value(onebartype_key);
        QString font = Printer_ARGOX_PPLA::textfont_of_PPLA().value(font_key);
        if(font.isEmpty()) font = font_key; //若是没有在原字体里找到，则是新定义字体

        QString ins;
        ins += printer.drawText(test_text, 20, 20,font);
        ins += printer.drawOneCode(test_barcode,20,100,onebartype,41, Printer_ARGOX_PPLA::ROTATE_0, wide_width, narrow_width);

        toprinter->write(printer.toPrint(ins, printcount.toInt()));
    }
        break;
    case 3://! ARGOX CP-2140 字体966 串口打印失败 9600 无效
    {
        Printer_ARGOX_PPLB printer;
        QString onebartype = Printer_ARGOX_PPLB::oneBarType_of_PPLB().value(onebartype_key);
        QString font = Printer_ARGOX_PPLB::textfont_of_PPLB().value(font_key);
        if(font.isEmpty()) font = font_key; //若是没有在原字体里找到，则是新定义字体

        QString ins;
        ins += printer.drawText(test_text, 20, 20,font);
        ins += printer.drawOneCode(test_barcode,20,100,onebartype,41, Printer_ARGOX_PPLB::ROTATE_0, wide_width, narrow_width);

        toprinter->write(printer.toPrint(ins, printcount.toInt()));
    }
        break;
    case 4://!  佳博GP-3120TN 测试通过 字体FONT001 串口成功9600  提示： toprint的高度不能为0，会认为时无限大
    case 5:// 佳博GP-3150TIN
    {
        Printer_GAINSCHA printer;
        QString onebartype = Printer_GAINSCHA::oneBarType_of_GAINSCHA().value(onebartype_key);
        QString font = Printer_GAINSCHA::textfont_of_GAINSCHA().value(font_key);
        if(font.isEmpty()) font = font_key; //若是没有在原字体里找到，则是新定义字体

        QString ins;
        QString height=smartprint?"0":"30";
        ins += printer.drawText(test_text, 20, 20,font);
        ins += printer.drawOneCode(test_barcode,20,100,onebartype);
        ins += printer.drawOneCode(test_barcode,20,100,onebartype,41, Printer_GAINSCHA::ROTATE_0, wide_width, narrow_width);
        toprinter->write(printer.toPrint(ins, printcount.toInt(),height,0,QString::number(40)));
    }

        break;
    case 6://佳博GP-2120TF
    {
        Printer_GAINSCHA_2120TF printer;
        QString onebartype = Printer_GAINSCHA_2120TF::oneBarType_of_GAINSCHA().value(onebartype_key);
        QString font = Printer_GAINSCHA_2120TF::textfont_of_GAINSCHA().value(font_key);
        if(font.isEmpty()) font = font_key; //若是没有在原字体里找到，则是新定义字体
        QString ins;
        float height=smartprint?0:(30*1.0/25.4);
        float width=40*1.0/25.4;
        ins += printer.drawText(test_text, 20, 20,font);
        ins += printer.drawOneCode(test_barcode,20,100,onebartype);
        ins += printer.drawOneCode(test_barcode,20,100,onebartype,41, Printer_GAINSCHA_2120TF::ROTATE_0, wide_width, narrow_width);
        toprinter->write(printer.toPrint(ins, printcount.toInt(),height,0,width));
    }
        break;
    }

    toprinter->close();
    delete toprinter;
}

void labelprint_set_dialog::update_labelprint_path(const QString &pathtype)
{
    ui->comboBox_labelprint_baudrate->setEnabled(false);
    switch(CommonHeader::creat_printpathtypes().indexOf(pathtype)){//swtich
    case 0: //usb
        ui->comboBox_labelprint_printpath->clear();
        ui->comboBox_labelprint_printpath->addItems(f_scan_usbPort());
        break;
    case 1: //串口
        ui->comboBox_labelprint_printpath->clear();
        ui->comboBox_labelprint_printpath->addItems(f_scan_ttyPort());
        ui->comboBox_labelprint_baudrate->setEnabled(true);
        break;
    case 2: //网口
    {
        QAbstractItemModel *model=ui->comboBox_labelprint_printpathtype->model();
        if(model->rowCount()>=3
                && qvariant_cast<QColor>(model->data(model->index(2, 0), Qt::TextColorRole))==QColor("red")){
            uinormal_message::warning(this, tr("标价签:"), tr("标价签错误:")+tr("不支持网口,返回串口模式"));
            ui->comboBox_labelprint_printpathtype->setCurrentIndex(1);//串口
        }
    }
    }//!swtich
}
void labelprint_set_dialog::update_labelprint_onebartype(const QString &printtype)
{
    update_combobox_textcolor(ui->comboBox_labelprint_printpathtype, QColor("red"));
    switch(CommonHeader::creat_printtypes().indexOf(printtype)){
    case 0://POSTEK Q8/200
        ui->comboBox_labelprint_onebartype->clear();
        ui->comboBox_labelprint_onebartype->addItems(Printer_POSTEK::oneBarType_of_POSTEK().keys());
        ui->comboBox_labelprint_font->clear();
        ui->comboBox_labelprint_font->addItems(Printer_POSTEK::textfont_of_POSTEK().keys());
        break;
    case 1://TSC TTP-244 PLUS"
        ui->comboBox_labelprint_onebartype->clear();
        ui->comboBox_labelprint_onebartype->addItems(printer_TSC_TTP_244_PLUS::oneBarType_of_TSC().keys());
        ui->comboBox_labelprint_font->clear();
        ui->comboBox_labelprint_font->addItems(printer_TSC_TTP_244_PLUS::textfont_of_TSC().keys());
        break;
    case 2://SC2100(商超专用)
        ui->comboBox_labelprint_onebartype->clear();
        ui->comboBox_labelprint_onebartype->addItems(Printer_ARGOX_PPLA::oneBarType_of_PPLA().keys());
        ui->comboBox_labelprint_font->clear();
        ui->comboBox_labelprint_font->addItems(Printer_ARGOX_PPLA::textfont_of_PPLA().keys());
        break;
    case 3://ARGOX CP-2140
        ui->comboBox_labelprint_onebartype->clear();
        ui->comboBox_labelprint_onebartype->addItems(Printer_ARGOX_PPLB::oneBarType_of_PPLB().keys());
        ui->comboBox_labelprint_font->clear();
        ui->comboBox_labelprint_font->addItems(Printer_ARGOX_PPLB::textfont_of_PPLB().keys());
        break;
    case 5://佳博GP-3150TIN
        update_combobox_textcolor(ui->comboBox_labelprint_printpathtype, QColor("green"));
    case 4://佳博GP-3120TN
        ui->comboBox_labelprint_onebartype->clear();
        ui->comboBox_labelprint_onebartype->addItems(Printer_GAINSCHA::oneBarType_of_GAINSCHA().keys());
        ui->comboBox_labelprint_font->clear();
        ui->comboBox_labelprint_font->addItems(Printer_GAINSCHA::textfont_of_GAINSCHA().keys());
        break;
    case 6://佳博GP-2120TF
        ui->comboBox_labelprint_onebartype->clear();
        ui->comboBox_labelprint_onebartype->addItems(Printer_GAINSCHA_2120TF::oneBarType_of_GAINSCHA().keys());
        ui->comboBox_labelprint_font->clear();
        ui->comboBox_labelprint_font->addItems(Printer_GAINSCHA_2120TF::textfont_of_GAINSCHA().keys());
        break;
    }
    update_labelprint_path(ui->comboBox_labelprint_printpathtype->currentText());
}

void labelprint_set_dialog::update_combobox_textcolor(QComboBox *com, QColor color)
{
    QAbstractItemModel *model=com->model();
    if(model->rowCount()>=3)model->setData(model->index(2,0), color, Qt::TextColorRole);
}

void labelprint_set_dialog::toportbyvidpid()
{
    labelprint_set_vidpid_dialog dialog(this);
    dialog.exec();
}

void labelprint_set_dialog::update_combobox(QComboBox *com, const QString &index)
{
    if(com->isEditable()){
        com->setEditText(index);
    } else {
        int sqr;
        if((sqr = com->findText(index)) == -1)
            com->setCurrentIndex(0);
        else
            com->setCurrentIndex(sqr);
    }
}
