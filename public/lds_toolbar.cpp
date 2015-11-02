
#include "lds_toolbar.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QLine>
#include <QDebug>
#include <QFrame>
#include <QPushButton>
#include "lds_toolbar_button.h"
#include <QTimer>
#include <QWidgetAction>
#include <QMenu>
#include "lds_menu.h"

lds_toolbar::lds_toolbar(QWidget *parent) :
    QFrame(parent)
{
    time=new QTime;
    label=new QLabel;
    label->setStyleSheet("color:blue;border:0px;background:transparent;");
    this->setFrameShape(QFrame::NoFrame);
}

void lds_toolbar::setup(const QStringList &names, Qt::Alignment flag)
{

    if(this->layout()){
        qDebug() << tr("layout existed!");
        return;
    }
    QLayout *layout=0;
    if(flag==Qt::AlignRight) {
        layout=new QHBoxLayout;
        static_cast<QHBoxLayout *>(layout)->addStretch();
        layout->addWidget(label);
        layoutaddwidget(layout, names, flag);
    } else if(flag==Qt::AlignLeft){
        layout=new QHBoxLayout;
        layoutaddwidget(layout, names, flag);
        layout->addWidget(label);
        static_cast<QHBoxLayout *>(layout)->addStretch();
    } else if(flag==Qt::AlignTop){
        layout=new QVBoxLayout;
        layoutaddwidget(layout, names, flag);
        layout->addWidget(label);
        static_cast<QVBoxLayout *>(layout)->addStretch();

    } else if(flag==Qt::AlignBottom){
        layout=new QVBoxLayout;
        static_cast<QVBoxLayout *>(layout)->addStretch();
        layout->addWidget(label);
        layoutaddwidget(layout, names, flag);
    }


    if(layout){
        label->setAlignment(flag);
        layout->setMargin(0);
        setLayout(layout);
    }
}

QAbstractButton* lds_toolbar::index_widget(const QString &name)
{
    return nameTool.value(name);
}

void lds_toolbar::setselectgoodsenable_itemflag(const QString &name)
{
    lds_toolbar_button *btn=static_cast<lds_toolbar_button*>(this->index_widget(name));
    if(btn){
        lds_menu *menu=new lds_menu;
        QWidgetAction *widgetaction=new QWidgetAction(btn);
        Selectgoods *action=new Selectgoods(menu);
        widgetaction->setDefaultWidget(action);
        menu->addAction(widgetaction);
        btn->setMenu(menu);
        action->setselectmultimode();//设置多选
        action->setitem_flag('Y');
        //after
        connect(action,SIGNAL(okstrs(QStringList)),this,SIGNAL(okstrs(QStringList)));
    }

}

void lds_toolbar::setselectgoodsenable_check_sheetno(const QString &name)
{
    lds_toolbar_button *btn=static_cast<lds_toolbar_button*>(this->index_widget(name));
    if(btn){
        lds_menu *menu=new lds_menu(btn);
        QWidgetAction *widgetaction=new QWidgetAction(btn);
        Selectgoods *action=new Selectgoods(menu);
        widgetaction->setDefaultWidget(action);
        menu->addAction(widgetaction);
        btn->setMenu(menu);
        action->setselectmultimode();//设置多选
        action->setstcheck_sheetno();
        //after
        connect(action,SIGNAL(okstrs(QStringList)),this,SIGNAL(okstrs(QStringList)));
    }
}

void lds_toolbar::starttime()
{
    time->start();
}

void lds_toolbar::updatetime()
{
    label->setText(QString::number(time->elapsed()));
}

void lds_toolbar::labelclear()
{
    label->clear();
}

void lds_toolbar::layoutaddwidget(QLayout *layout, const QStringList &names, Qt::Alignment flag)
{
    foreach(QString s, names){
        if(s=="|"){
            QFrame *line = new QFrame(this);
            line->setObjectName(QString::fromUtf8("line"));
            if(flag==Qt::AlignLeft||flag==Qt::AlignRight){
                line->setFrameShape(QFrame::VLine);
            } else {
                line->setFrameShape(QFrame::HLine);
            }
            line->setFrameShadow(QFrame::Sunken);
            line->setStyleSheet("background:black;");
            layout->addWidget(line);
        } else {
            lds_toolbar_button *t=new lds_toolbar_button(this);
            t->setText(s);
            if(t->text()==tr("退出")
                    ||t->text()==tr("取消"))t->setProperty("outer_stylesheet", "pushbutton_blue");
            else t->setProperty("outer_stylesheet", "pushbutton");
            layout->addWidget(t);
            nameTool.insert(s, t);
        }
    }
}
