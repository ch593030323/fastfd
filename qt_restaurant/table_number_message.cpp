#include "table_number_message.h"
#include <QPainter>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>

Base_label1::Base_label1(int x, int y, const QString &text,int type, QWidget *parent):
    QLabel(parent)
{
    //"行号"<<"品码"<<"品名"<<"数量"<<"价格"<<"折扣"<<"折扣(%)"<<"金额"<<"厨打说明"<<"点菜行号"<<"套菜"<<"特价"<<"赠送"<<"优惠券";
#ifdef OFFLINE//若是offline
    info/*offline*/<<"0202"<<"2 0 5 "<<"苹果"<<"1.00"<<"36.0"<<"8.00"<<"80.0"<<"35.0"<<"??"<<"1.00"  <<"N"  <<"N"  <<"N"  <<"N";
#endif
    xx = x;
    yy = y;
    this->setStyleSheet("background-color:rgb(114,238,114);border-radius:5px;");
    // setGeometry(QRect(x * 74, y * 70, 74, 70));
    this->setFixedSize(119,80);
    QFont font;
    font.setPointSize(12);
    font.setFamily(QString::fromUtf8("Andalus"));

    // lable_bottom = new QLabel(this);
    lable_between = new QLabel(this);
    lable_between->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    lable_top = new QLabel(this);

    // lable_bottom->setPixmap(QPixmap(fileName));
    //    lable_bottom->setStyleSheet("background-color:rgb(0,255,255)");
    // lable_bottom->setGeometry(QRect(0, 0, 74, 70));
    // lable_bottom->setScaledContents(true);
    if(type == 1)
    {
        lable_between->setStyleSheet("color:#7C7C7C");
    }
    else
    {
        lable_between->setStyleSheet("color:#FB7422");
    }
    lable_between->setFont(font);
    lable_between->setGeometry(QRect(0, 0, 119, 80));

#ifdef OFFLINE//若是offline
    lable_between->setText(QString("%1\n%2\n%3").arg(info[1],info[2],info[4]));
#else
    lable_between->setText(text);
#endif
    font.setPointSize(20);
    lable_top->setFont(font);
    lable_top->setGeometry(0,0,20,lable_top->height());
    lable_top->setStyleSheet("color:rgb(255, 0, 0)");

}


void Base_label1::mousePressEvent(QMouseEvent *)
{
    if(yy == 0){/*发送信号给type up*/ emit typeup();}
    if(yy == 1){/*发送信号给type down*/emit typedown();}
}


QPoint Table_Number_Message::flag= QPoint(0,0);
Table_Number_Message *Table_Number_Message::lable1 = NULL;

Table_Number_Message::Table_Number_Message(int x, int y,
                                           const QString &text,int type, QWidget *parent):
    Base_label1(x, y, text,type,parent)
{
    if(QPoint(x,y) == flag) {
        QString str_lable("√");

        lable_top->setText(str_lable);
        lable1 = this;
        //        emit toType(xx + yy * 4);此时connect未完全建立
    }
}


void Table_Number_Message::mousePressEvent(QMouseEvent *)
{
    /*控件只能点一次*/
    // if(QPoint(xx,yy) != flag){
    //        QString str_lable("√");
    //        lable_top->setText(str_lable);

    //        lable1->lable_top->clear();
    //        lable1 = this;
    //        flag =  QPoint(xx,yy);
    //        emit toType(xx*10 + yy);

    // }
    QString str_lable("√");
    lable_top->setText(str_lable);
    if(lable1 != NULL && QPoint(xx,yy) != flag )//开始第一个
        lable1->lable_top->clear();

    lable1 = this;
    flag =  QPoint(xx,yy);
    QDesktopWidget *d=QApplication::desktop();
    if(d->width() < 1024)
    {
        emit toType(xx*6 + yy);
    }
    else
    {
        emit toType(xx*8 + yy);
    }
}
