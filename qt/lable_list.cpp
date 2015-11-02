#include "lable_list.h"
#include <qdebug.h>

lable_list::lable_list(int x, int y, const QString &text,
                       const QString & m1,const QString & m2, QWidget *parent):
    QLabel(parent),p1(m1),p2(m2)
{
    this->setFixedSize(104,90);
#ifdef OFFLINE//若是offline
    info/*offline*/<<"0202"<<"2 0 5 "<<"苹果"<<"1.00"<<"36.0"<<"8.00"<<"80.0"<<"35.0"<<"??"<<"1.00"  <<"N"  <<"N"  <<"N"  <<"N";
#endif
    xx = x;
    yy = y;
    QFont font;
    font.setPointSize(12);
    lable_bottom = new QLabel(this);
    lable_between = new QLabel(this);
    lable_between->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    //lable_top = new QLabel(this);
    lable_bottom->setAutoFillBackground(true);
    // lable_bottom->setPixmap(QPixmap(p1));
    lable_bottom->setStyleSheet(p1);
    //    lable_bottom->setStyleSheet("background-color:rgb(0,255,255)");
    lable_bottom->setGeometry(QRect(0, 0, 104, 90));
    lable_bottom->setScaledContents(true);

    // lable_between->textsetFont(font);
    lable_between->setGeometry(QRect(0, 0, 104, 90));
    lable_between->setStyleSheet("color:black");
#ifdef OFFLINE//若是offline
    lable_between->setText(QString("%1\n%2\n%3").arg(info[1],info[2],info[4]));
#else
    lable_between->setText(text);
    lable_between->setFont(font);
#endif
    font.setPointSize(20);
}

void lable_list::mousePressEvent(QMouseEvent *)
{
    if(yy == 0){/*发送信号给type up*/ emit typeup();}
    if(yy == 1){/*发送信号给type down*/emit typedown();}
}

QPoint lsit_label_info::flag= QPoint(-1, -1);
lsit_label_info *lsit_label_info::lable1 = 0;

lsit_label_info::lsit_label_info(int x, int y,
                                 const QString &text,
                                 const QString & m1,const QString & m2,
                                 bool first_is_selected,
                                 QWidget *parent):
    lable_list(x, y, text, m1,m2,parent)
{
    lable1=0;
    if(QPoint(x,y) == flag
            &&first_is_selected) {
        lable_bottom->setStyleSheet(p2);
        lable1 = this;
    }

}

void lsit_label_info::mousePressEvent(QMouseEvent *)
{
    /*控件只能点一次*/
    if(QPoint(xx,yy) != flag){
        lable_bottom->setStyleSheet(p2);
        qDebug() << lable1;
        if(lable1 == 0){
        } else {
            lable1->lable_bottom->setStyleSheet(p1);
        }
        lable1 = this;
        flag =  QPoint(xx,yy);
    }
    emit toType(xx*5 + yy);
}

