#include "public_debugdialog.h"
#include "ui_public_debugdialog.h"
#include <QProcess>
#include "public_sql.h"
public_debugDialog::public_debugDialog(QWidget *parent) :
    lds_dialog_drog(parent),
    ui(new Ui::public_debugDialog)
{
    ui->setupUi(this);
    connect(&timer,SIGNAL(timeout()),this,SLOT(on_pushButton_clicked()));
    timer.setInterval(1000);
    timer.start();
    ui->pushButton->hide();
    ui->lineEdit->hide();
}

public_debugDialog::~public_debugDialog()
{
    delete ui;
}

void public_debugDialog::on_pushButton_clicked()
{
    QProcess p;
    QString out;
    QStringList outs;
    p.start("free");
    if(p.waitForReadyRead()){
        out = p.readAll();
    }
    outs= out.split(" ", QString::SkipEmptyParts);
    if(!outs.isEmpty()){
        ui->tableWidget->item(0,0)->setText(outs[0]);
        ui->tableWidget->item(0,1)->setText(outs[1]);
        ui->tableWidget->item(0,2)->setText(outs[2]);
        ui->tableWidget->item(0,3)->setText(outs[3]);
        ui->tableWidget->item(0,4)->setText(outs[4]);

        ui->tableWidget->item(1,0)->setText(outs[6]);
        ui->tableWidget->item(1,1)->setText(outs[7]);
        ui->tableWidget->item(1,2)->setText(outs[8]);
        ui->tableWidget->item(1,3)->setText(outs[9]);
        ui->tableWidget->item(1,4)->setText(outs[10]);
    }
    p.close();
    this->raise();
}

void public_debugDialog::on_lineEdit_editingFinished()
{
    timer.setInterval(qMax(100, ui->lineEdit->text().toInt()));
}
