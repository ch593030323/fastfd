#include "lds_tableview_updown.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollBar>
lds_tableView_updown::lds_tableView_updown(QWidget *parent) :
    QWidget(parent)
{
    tableview=new lds_tableView(this);
    up=new QPushButton(this);
    down=new QPushButton(this);
    up->setFocusPolicy(Qt::NoFocus);
    down->setFocusPolicy(Qt::NoFocus);
    up->setEnabled(false);
    down->setEnabled(false);
    QHBoxLayout *hlayout=new QHBoxLayout;
    hlayout->addWidget(up);
    hlayout->addWidget(down);
    QVBoxLayout *vlayout=new QVBoxLayout;
    vlayout->setSpacing(0);
    vlayout->setContentsMargins(0,0,0,0);

    vlayout->addWidget(tableview);
    vlayout->addLayout(hlayout);
    this->setLayout(vlayout);

    up->setProperty("outer_stylesheet", "pushbutton_up");
    down->setProperty("outer_stylesheet", "pushbutton_down");
    connect(up,SIGNAL(clicked()),this,SLOT(toup()));
    connect(down,SIGNAL(clicked()),this,SLOT(todown()));
    connect(tableview->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(toisenabled(int)));
}
void lds_tableView_updown::todown()
{
    tableview->verticalScrollBar()->setValue(
                tableview->verticalHeader()->defaultSectionSize()*((tableview->verticalScrollBar()->value()+tableview->viewport()->height())/tableview->verticalHeader()->defaultSectionSize())
                );
}

void lds_tableView_updown::toup()
{
    tableview->verticalScrollBar()->setValue(
                tableview->verticalHeader()->defaultSectionSize()*((tableview->verticalScrollBar()->value()-tableview->viewport()->height())/tableview->verticalHeader()->defaultSectionSize())
                );
}

void lds_tableView_updown::toisenabled(int value)
{
    up->setEnabled(value!=tableview->verticalScrollBar()->minimum());
    down->setEnabled(value!=tableview->verticalScrollBar()->maximum());
}
