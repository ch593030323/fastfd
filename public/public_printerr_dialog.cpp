#include "public_printerr_dialog.h"
#include "ui_public_printerr_dialog.h"

public_printerr_Dialog::public_printerr_Dialog(QWidget *parent) :
    lds_widget_drog(parent),
    ui(new Ui::public_printerr_Dialog)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
}

public_printerr_Dialog::~public_printerr_Dialog()
{
    delete ui;
}

void public_printerr_Dialog::setContent(const QString &text, const QString &title)
{
    ui->label->setText(text);
    ui->label_title->setText(title);
}
