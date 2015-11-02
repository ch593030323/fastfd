#include "lds_lineedit.h"
#include <QTimer>

lds_lineedit::lds_lineedit(QWidget *parent):
    QLineEdit(parent),
    _isignore(false)
{
    connect(this,SIGNAL(textChanged(QString)),this,SLOT(totextnotinput(QString)));
}

void lds_lineedit::set_ignore_keyevent(bool isignore)
{
    _isignore=!isignore;
}

void lds_lineedit::totextnotinput(const QString &text)
{
    if(_isignore)
        if(text.length() ==1)
            QTimer::singleShot(100, this,SLOT(clear()));
}

void lds_lineedit::keyPressEvent(QKeyEvent *e)
{
    QLineEdit::keyPressEvent(e);
}

lds_lineedit::~lds_lineedit()
{

}

