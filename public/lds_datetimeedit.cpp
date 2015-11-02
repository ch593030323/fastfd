#include "lds_datetimeedit.h"
#include <QMouseEvent>
#include <QCalendarWidget>
#include <QtDebug>
#include <QStyleOptionComboBox>
#include <QPainter>
#include <QStyle>
#include <QTimer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include "lds.h"
#include "lds_datetimeedit_pop.h"

lds_datetimeEdit::lds_datetimeEdit(QWidget *parent):
    QDateTimeEdit(parent),
    popwidget(0),
    subcontrol_ismouseover(true)
{
    this->setCalendarPopup(true);
    this->setDisplayFormat("yyyy-MM-dd h:m:s");
}

lds_datetimeEdit::~lds_datetimeEdit()
{
    if(popwidget)delete popwidget;
}

void lds_datetimeEdit::mousePressEvent(QMouseEvent *event)
{
    //0
    if(this->calendarPopup()==false){
        QDateTimeEdit::mousePressEvent(event);
        return;
    }
    //0.2
    if(!popwidget){
        popwidget=new lds_datetimeEdit_pop(this);
    }
    if(!subcontrol_ismouseover) return;
    popwidget->setDateTime(this->dateTime());
    //1
    QPoint pos=lds::getPopPoint(this, popwidget);
    //2
    popwidget->move(pos);
    popwidget->show();
    popwidget->raise();
}

void lds_datetimeEdit::paintEvent(QPaintEvent *event)
{
    QStyleOptionSpinBox opt;
    initStyleOption(&opt);

    QStyleOptionComboBox optCombo;

    optCombo.init(this);
    optCombo.editable = true;
    optCombo.frame = opt.frame;
    optCombo.subControls = opt.subControls;
    optCombo.activeSubControls = opt.activeSubControls;
    optCombo.state = opt.state;
    if (this->isEnabled()==false) {
        optCombo.state &= ~QStyle::State_Enabled;
    }
    subcontrol_ismouseover=(optCombo.state&QStyle::State_MouseOver);
    QPainter p(this);
    style()->drawComplexControl(QStyle::CC_ComboBox, &optCombo, &p, this);
}
