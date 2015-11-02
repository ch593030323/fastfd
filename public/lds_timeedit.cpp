#include "lds_timeedit.h"
#include "ui_lds_timeedit.h"
#include <QTime>

//正数 最大约数
static int minYUESHU(int value){
    int half=value/2;
    while(half>0&&value%half!=0)half--;
    if(half<=1)return value;
    else return half;
}
lds_timeEdit::lds_timeEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lds_timeEdit),
    checkedrb(0)
{
    ui->setupUi(this);
    connect(ui->pushButton_sub1,SIGNAL(clicked()),this,SLOT(tocenter()));
    connect(ui->pushButton_sub2,SIGNAL(clicked()),this,SLOT(tocenter()));
    connect(ui->pushButton_sub3,SIGNAL(clicked()),this,SLOT(tocenter()));
    connect(ui->pushButton_sub4,SIGNAL(clicked()),this,SLOT(tocenter()));
    connect(ui->pushButton_sub5,SIGNAL(clicked()),this,SLOT(tocenter()));
    connect(ui->pushButton_sub6,SIGNAL(clicked()),this,SLOT(tocenter()));
}

lds_timeEdit::~lds_timeEdit()
{
    delete ui;
}

void lds_timeEdit::setTime(const QTime &t)
{
    ui->radioButton_hour->setText(QString::number(t.hour()));
    ui->radioButton_minute->setText(QString::number(t.minute()));
    ui->radioButton_second->setText(QString::number(t.second()));
}

void lds_timeEdit::setShowVisiable(const QString &df)
{
    ui->radioButton_hour->setVisible(df.contains("h")||df.contains("H"));
    ui->radioButton_minute->setVisible(df.contains("m"));
    ui->radioButton_second->setVisible(df.contains("s"));

    ui->label->clear();
    if(!ui->radioButton_hour->isHidden())ui->label->setText(ui->label->text()+tr("小时-"));
    if(!ui->radioButton_minute->isHidden())ui->label->setText(ui->label->text()+tr("分钟-"));
    if(!ui->radioButton_second->isHidden())ui->label->setText(ui->label->text()+tr("秒-"));

    int v=!ui->radioButton_hour->isHidden()+!ui->radioButton_minute->isHidden()+!ui->radioButton_second->isHidden();
    this->setVisible(v);

}

void lds_timeEdit::tocenter()
{
    QPushButton *b=qobject_cast<QPushButton*>(this->sender());
    if(!b) return;

    QStringList be=b->text().split("-");
    if(be.size()==1){if(checkedrb&&b->text().isEmpty()==false){checkedrb->setText(b->text());totimechanged();}return;}
    if(be.size()!=2)return;
    int intervalue=be[1].toInt()-be[0].toInt()+1;
    int maxys=minYUESHU(intervalue);
    ui->pushButton_sub1->setText("");
    ui->pushButton_sub2->setText("");
    ui->pushButton_sub3->setText("");
    ui->pushButton_sub4->setText("");
    ui->pushButton_sub5->setText("");
    ui->pushButton_sub6->setText("");

    if(maxys==intervalue){
        switch(maxys){
        case 6:ui->pushButton_sub6->setText(QString::number(be[0].toInt()+5));
        case 5:ui->pushButton_sub5->setText(QString::number(be[0].toInt()+4));
        case 4:ui->pushButton_sub4->setText(QString::number(be[0].toInt()+3));
        case 3:ui->pushButton_sub3->setText(QString::number(be[0].toInt()+2));
        case 2:ui->pushButton_sub2->setText(QString::number(be[0].toInt()+1));
        case 1:ui->pushButton_sub1->setText(QString::number(be[0].toInt()+0));
        }
    } else {
        int between=intervalue/maxys;
        switch(maxys){
        case 6:ui->pushButton_sub6->setText(QString("%1-%2").arg(be[0].toInt()+5*between).arg(be[0].toInt()+5*between+between-1));
        case 5:ui->pushButton_sub5->setText(QString("%1-%2").arg(be[0].toInt()+4*between).arg(be[0].toInt()+4*between+between-1));
        case 4:ui->pushButton_sub4->setText(QString("%1-%2").arg(be[0].toInt()+3*between).arg(be[0].toInt()+3*between+between-1));
        case 3:ui->pushButton_sub3->setText(QString("%1-%2").arg(be[0].toInt()+2*between).arg(be[0].toInt()+2*between+between-1));
        case 2:ui->pushButton_sub2->setText(QString("%1-%2").arg(be[0].toInt()+1*between).arg(be[0].toInt()+1*between+between-1));
        case 1:ui->pushButton_sub1->setText(QString("%1-%2").arg(be[0].toInt()+0*between).arg(be[0].toInt()+0*between+between-1));
        }
    }
}

void lds_timeEdit::on_radioButton_hour_clicked(bool checked)
{
    if(checked){
        ui->pushButton_sub1->setText("1-6");
        ui->pushButton_sub2->setText("7-12");
        ui->pushButton_sub3->setText("13-18");
        ui->pushButton_sub4->setText("19-24");
        ui->pushButton_sub5->setText("");
        checkedrb=ui->radioButton_hour;
    }
}

void lds_timeEdit::on_radioButton_minute_clicked(bool checked)
{
    if(checked){
        ui->pushButton_sub1->setText("1-12");
        ui->pushButton_sub2->setText("13-24");
        ui->pushButton_sub3->setText("25-36");
        ui->pushButton_sub4->setText("37-48");
        ui->pushButton_sub5->setText("49-60");
        checkedrb=ui->radioButton_minute;
    }
}

void lds_timeEdit::on_radioButton_second_clicked(bool checked)
{
    if(checked){
        ui->pushButton_sub1->setText("1-12");
        ui->pushButton_sub2->setText("13-24");
        ui->pushButton_sub3->setText("25-36");
        ui->pushButton_sub4->setText("37-48");
        ui->pushButton_sub5->setText("49-60");
        checkedrb=ui->radioButton_second;
    }
}

void lds_timeEdit::totimechanged()
{
    emit timechanged(QTime(ui->radioButton_hour->text().toInt(),
                           ui->radioButton_minute->text().toInt(),
                           ui->radioButton_second->text().toInt()));
}
