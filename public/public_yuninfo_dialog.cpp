#include "public_yuninfo_dialog.h"

public_yuninfo_Dialog::public_yuninfo_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::public_yuninfo_Dialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog|Qt::Popup);this->setStyleSheet("QDialog{border:2px solid gray}");
}

public_yuninfo_Dialog::~public_yuninfo_Dialog()
{
    delete ui;
}
