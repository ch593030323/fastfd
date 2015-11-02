#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "commonheader.h"
#include "trans_u2g.h"
#include "printer_normal_t2us.h"
#include "printer_pos_serial.h"
#include "printer_pos_net.h"
#include "printer_pos_virtual.h"
#include "printer_pos_vidpid.h"
#include <QMessageBox>
#include "electroniccountprice.h"
#include "clientdisplay.h"
#include "reader_rf.h"
#include "labelprint_main_dialog.h"
#include "barscale_main_dialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    countprice_scale(0)
{
    ui->setupUi(this);
    //1
    ui->comboBox->addItems(CommonHeader::creat_normal_printtypes());
    ui->comboBox_3->addItems(CommonHeader::creat_baudrates().values());
    ui->comboBox_2->addItems(f_scan_ttyPort());
    ui->comboBox_4->addItems(f_scan_usbPort());


    //2
    ui->comboBox_5->addItems(CommonHeader::creat_pricecount_scaletypes());
    ui->comboBox_6->addItems(CommonHeader::creat_baudrates().values());

    //3
    ui->comboBox_7->addItems(CommonHeader::creat_clientdisplay_ledtypes());
    ui->comboBox_8->addItems(f_scan_ttyPort());
    //4
    CommonHeader::com_ini_filepath=qApp->applicationDirPath()+"/"+qApp->applicationName()+".ini";
    CommonHeader::com_root_dirpath=qApp->applicationDirPath();
    CommonHeader::com_widget_dir=Qt::RightToLeft;

    this->setLayoutDirection(CommonHeader::com_widget_dir);
    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(countprice_scale)delete countprice_scale;
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    //("58mm热敏") << QObject::tr("80mm热敏") << QObject::tr("KF58") << QObject::tr("MINI58");
    switch(index){
    case 0:
    case 1:
    case 2:
        ui->groupBox->setEnabled(true);
        ui->groupBox_2->setEnabled(true);
        ui->groupBox_3->setEnabled(true);
        ui->groupBox_4->setEnabled(false);
        break;
    case 3:
        ui->groupBox->setEnabled(false);
        ui->groupBox_2->setEnabled(false);
        ui->groupBox_3->setEnabled(false);
        ui->groupBox_4->setEnabled(true);
        ui->lineEdit_3->setText(Printer_Normal_T2US().vidpid);
        break;
    }
}

void MainWindow::on_groupBox_toggled(bool arg1)
{
    if(arg1){
        ui->groupBox_4->setChecked(false);
        ui->groupBox_2->setChecked(false);
        ui->groupBox_3->setChecked(false);
    }
}
void MainWindow::on_groupBox_2_toggled(bool arg1)
{
    if(arg1){
        ui->groupBox_4->setChecked(false);
        ui->groupBox->setChecked(false);
        ui->groupBox_3->setChecked(false);
    }
}
void MainWindow::on_groupBox_3_toggled(bool arg1)
{
    if(arg1){
        ui->groupBox_4->setChecked(false);
        ui->groupBox_2->setChecked(false);
        ui->groupBox->setChecked(false);
    }
}
void MainWindow::on_groupBox_4_toggled(bool arg1)
{
    if(arg1){
        ui->groupBox->setChecked(false);
        ui->groupBox_2->setChecked(false);
        ui->groupBox_3->setChecked(false);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    Printer_POS_Virtual *printer=0;
    if(ui->groupBox->isChecked()){
        printer=new Printer_POS_SERIAL;
        static_cast<Printer_POS_SERIAL*>(printer)->psettings.BaudRate=(BaudRateType)ui->comboBox_3->currentText().toInt();
        printer->tryOpen(ui->comboBox_2->currentText());
    }
    if(ui->groupBox_2->isChecked()){
        printer=new Printer_POS_SERIAL;
        static_cast<Printer_POS_SERIAL*>(printer)->setType(Printer_POS_SERIAL::USB);
        printer->tryOpen(ui->comboBox_4->currentText());
    }
    if(ui->groupBox_3->isChecked()){
        printer=new Printer_POS_NET;
        printer->tryOpen(ui->lineEdit->text()+":"+ui->lineEdit_2->text());
    }
    if(ui->groupBox_4->isChecked()){
        printer=new Printer_POS_VIDPID;
        printer->tryOpen(ui->lineEdit_3->text());
    }

    if(printer){
        if(printer->lastError().isEmpty()){
            printer->write("=====test successed!====\n");
            printer->close();
        } else {
            QMessageBox::warning(this, tr("小票打印错误"), printer->lastError());
        }

        delete printer;
    } else {
        QMessageBox::warning(this, tr("小票打印错误"), tr("没有任何端口类型被选中"));
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    if(countprice_scale == 0) {
        countprice_scale=new   ElectronicCountPrice(0, "", "");
        connect(countprice_scale,SIGNAL(weighComming(QString)),ui->lineEdit_4,SLOT(setText(QString)));
    }
    countprice_scale->stop();
    countprice_scale->setType(ui->comboBox_5->currentText());
    countprice_scale->setBaudrate(ui->comboBox_6->currentText());
    if(!countprice_scale->tryStart_qs(ui->comboBox_5->currentText())){
        QMessageBox::warning(this, tr("消息提示"),countprice_scale->lastError());
    }
}

void MainWindow::on_comboBox_7_currentIndexChanged(int index)
{
    ui->lineEdit_5->setEnabled(index==1);
}

void MainWindow::on_pushButton_4_clicked()
{
    static int sqr=-0;
    ClientDisplay display(ui->comboBox_7->currentText(), "");
    if(sqr%5==0){
        if(!display.startShowinfo(ui->lineEdit_5->text(), ui->comboBox_8->currentText())){
            QMessageBox::warning(this, tr("客显错误"),display.lastError());
        }
    } else {
        if(!display.writeCom(sqr%5, ui->lineEdit_6->text(), ui->comboBox_8->currentText())){
            QMessageBox::warning(this, tr("客显错误"),display.lastError());
        }
    }
    sqr ++;
}

void MainWindow::on_pushButton_5_clicked()
{
    Reader_RF reader;
    reader.beep();
}

void MainWindow::on_pushButton_6_clicked()
{
    barscale_main_dialog dialog(this);
    dialog.add_down_infolist(barscale_main_dialog::BARSCAEL_ITEMS(
                                 "1",
                                 "cookie",
                                 "10000",
                                 "-12.6"
                                 )
                             );
    dialog.set_shop_name("temp_shopname");
    dialog.setWindowTitle(tr("条码秤"));
    dialog.exec();
}

void MainWindow::on_pushButton_7_clicked()
{
    labelprint_main_dialog dialog(this);
    dialog.printmodel->setHorizontalHeaderLabels(QStringList() << tr("打印次数")
                                               <<  tr("品名")
                                               <<  tr("品码")
                                               <<  tr("价格"));
    dialog.printmodel->appendRow(QList<QStandardItem*>()
                                 << new QStandardItem("1")
                                 << new QStandardItem("cookie")
                                 << new QStandardItem("1000")
                                 << new QStandardItem("-12.6")

                                 );
    dialog.inputfinished();
    dialog.setWindowTitle(tr("条码打印机"));
    dialog.exec();
}
