#include "lds_loadingdialog.h"

lds_loadingDialog::lds_loadingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_lds_loadingDialog)
{
    ui->setupUi(this);
    movie=new QMovie(":/image/loading.gif");
    ui->label_loading->setMovie(movie);
    movie->start();
    this->setFocusPolicy(Qt::NoFocus);
    ui->label_loading->setFocusPolicy(Qt::NoFocus);
    ui->label_text->setFocusPolicy(Qt::NoFocus);

}

lds_loadingDialog::~lds_loadingDialog()
{
    movie->stop();
    delete ui;
}

void lds_loadingDialog::updatetext(const QString &text)
{
    ui->label_text->setText(text);
}
