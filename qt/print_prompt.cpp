#include "print_prompt.h"
#include "ui_print_prompt.h"

print_prompt::print_prompt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::print_prompt)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);
}

print_prompt::~print_prompt()
{
    delete ui;
}
