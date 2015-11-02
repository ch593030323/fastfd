#include "lds_keytoolbutton_pop.h"
#include "ui_lds_keytoolbutton_pop.h"
#include <QtDebug>

lds_keytoolbutton_pop::lds_keytoolbutton_pop(QWidget *parent) :
    lds_roundedwidget(parent),
    ui(new Ui::lds_keytoolbutton_pop)
{
    ui->setupUi(this);
    //1
    setAttribute(Qt::WA_DeleteOnClose);
    //2
    connect(ui->pushButton, SIGNAL(clicked()),this,SLOT(tocenter()));
    connect(ui->pushButton_2, SIGNAL(clicked()),this,SLOT(tocenter()));
    connect(ui->pushButton_3, SIGNAL(clicked()),this,SLOT(tocenter()));
    connect(ui->pushButton_4, SIGNAL(clicked()),this,SLOT(tocenter()));
    connect(ui->pushButton_5, SIGNAL(clicked()),this,SLOT(tocenter()));
    connect(ui->pushButton_6, SIGNAL(clicked()),this,SLOT(tocenter()));
    connect(ui->pushButton_7, SIGNAL(clicked()),this,SLOT(tocenter()));
    connect(ui->pushButton_8, SIGNAL(clicked()),this,SLOT(tocenter()));
    connect(ui->pushButton_9, SIGNAL(clicked()),this,SLOT(tocenter()));
    connect(ui->pushButton_10, SIGNAL(clicked()),this,SLOT(tocenter()));
    connect(ui->pushButton_11, SIGNAL(clicked()),this,SLOT(tocenter()));
    connect(ui->pushButton_12, SIGNAL(clicked()),this,SLOT(tocenter()));
    connect(ui->pushButton_13, SIGNAL(clicked()),this,SLOT(tocenter()));
    connect(ui->pushButton_14, SIGNAL(clicked()),this,SLOT(tocenter()));
    connect(ui->pushButton_16, SIGNAL(clicked()),this,SLOT(tocenter()));
}

lds_keytoolbutton_pop::~lds_keytoolbutton_pop()
{
    qDebug() << "delete";
    delete ui;
}

void lds_keytoolbutton_pop::tocenter()
{
    QPushButton *p=qobject_cast<QPushButton*>(this->sender());
    if(p){
        emit signal_click(p->text());
    }
}
