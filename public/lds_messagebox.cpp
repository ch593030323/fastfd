#include "lds_messagebox.h"
#include "backheader.h"
#include <QTimeLine>
#include "public_sql.h"
#include "lds_toolbar_button.h"

lds_messagebox::lds_messagebox(QObject *parent) :
    QObject(parent)
{
}
int lds_messagebox::warning(QWidget *parent, const QString &title, const QString &text, const QString &button0Text, const QString &button1Text, const QString &button2Text)
{
    return lds_messagebox::showOldMessageBox(parent, QMessageBox::Warning, title, text,
                                             button0Text, button1Text, button2Text);
}

int lds_messagebox::warning(QWidget *parent, const QString &title, const QString &text, bool delayclose)
{
    return lds_messagebox::showOldMessageBox(parent, QMessageBox::Warning, title, text, "", "", "", delayclose);
}

int lds_messagebox::showOldMessageBox(QWidget *parent, QMessageBox::Icon icon, const QString &title, const QString &text, const QString &button0Text, const QString &button1Text, const QString &button2Text, bool delayclose)
{
    if(parent==0
            &&public_sql::save_login!=0)parent=public_sql::save_login->getcur_validparent();
    lds_msgbox messageBox(icon, title, text, QMessageBox::NoButton, parent);
    messageBox.setWindowFlags(Qt::Dialog|Qt::WindowTitleHint|Qt::CustomizeWindowHint);
    QString myButton0Text = button0Text;
    if(myButton0Text.isEmpty())myButton0Text=tr("确定");
    {
        lds_toolbar_button *b=new lds_toolbar_button;
        b->setProperty("outer_stylesheet", "pushbutton");
        b->setText(myButton0Text);
        messageBox.addButton(b, QMessageBox::ActionRole);
        messageBox.btnok=b;
    }
    if(!button1Text.isEmpty()){
        lds_toolbar_button *b=new lds_toolbar_button;
        b->setProperty("outer_stylesheet", "pushbutton");
        b->setText(button1Text);
        messageBox.addButton(b, QMessageBox::ActionRole);
    }
    if(!button2Text.isEmpty()){
        lds_toolbar_button *b=new lds_toolbar_button;
        b->setProperty("outer_stylesheet", "pushbutton");
        b->setText(button2Text);
        messageBox.addButton(b, QMessageBox::ActionRole);
    }

    QTimeLine timeLine(2000, 0);
    if(delayclose){
        timeLine.setFrameRange(0, 5);
        timeLine.setDirection(QTimeLine::Backward);
        connect(&timeLine, SIGNAL(frameChanged(int)), &messageBox, SLOT(updateText(int)));
        connect(&messageBox,SIGNAL(btnok_finished(int)),&messageBox, SLOT(done(int)));
        timeLine.start();
    }

    return messageBox.exec();
}
