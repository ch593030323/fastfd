#include "public_heartbeatdialog.h"
#include "ui_public_heartbeatdialog.h"
#include "public_logindialog.h"
#include "public_sql.h"

public_heartbeatDialog::public_heartbeatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::public_heartbeatDialog)
{
    ui->setupUi(this);
    ui->widget->setup(QStringList() << tr("重连")/*<<tr("取消")*/, Qt::AlignRight);
    connect(ui->widget->index_widget(tr("重连")), SIGNAL(clicked()),this,SLOT(took()));
    connect(ui->widget->index_widget(tr("取消")), SIGNAL(clicked()),this,SLOT(toexit()));

}

public_heartbeatDialog::~public_heartbeatDialog()
{
    delete ui;
}

void public_heartbeatDialog::took()
{
    QSqlDatabase::database();//默认的,open
    this->accept();
}

void public_heartbeatDialog::toexit()
{
    this->reject();
}
