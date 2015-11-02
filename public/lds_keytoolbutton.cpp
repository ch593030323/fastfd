#include "lds_keytoolbutton.h"
#include "lds_keytoolbutton_pop.h"
#include <QApplication>
#include <QLineEdit>
#include "lds.h"
#include <QtDebug>
#include <QTimer>

lds_keytoolbutton::lds_keytoolbutton(QWidget *parent) : QToolButton(parent)
{
    parentl=0;
    this->setFocusPolicy(Qt::NoFocus);
    this->setAutoRaise(true);
    this->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->setIcon(QIcon(":/image/widgets_pic/keyboard.png"));
    this->setIconSize(QSize(80,40));

    connect(this,SIGNAL(clicked()),this,SLOT(showpop()));

}

void lds_keytoolbutton::tocenter(const QString &value)
{
    if(!parentl) return;
    if(value=="<<"){
        parentl->backspace();
    } else if(value=="<-"){
        parentl->cursorBackward(false);
    } else if(value=="->"){
        parentl->cursorForward(false);
    } else {
        parentl->insert(value);
    }
}

void lds_keytoolbutton::showpop()
{
    QWidget *fw=qApp->focusWidget();
    if(fw
            &&fw->inherits("QLineEdit")){
        parentl=qobject_cast<QLineEdit*>(fw);
        parentl->setStyleSheet("color:indigo;font:bold 20pt;");
        parentl->selectAll();

        QTimer::singleShot(100, this,SLOT(showpop_time()));
    }
}

void lds_keytoolbutton::showpop_time()
{
    lds_keytoolbutton_pop *pop=new lds_keytoolbutton_pop;
    connect(pop,SIGNAL(signal_click(QString)),this,SLOT(tocenter(QString)));
    connect(pop,SIGNAL(destroyed(QObject*)),this,SLOT(closepop(QObject *)));
    pop->raise();
    pop->move(lds::getPopPoint(parentl, pop));
    pop->show();
}

void lds_keytoolbutton::closepop(QObject *w)
{
    if(w&&parentl)parentl->setStyleSheet("");
}

