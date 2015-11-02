#include "lds_chartdialog.h"
#include "ui_lds_chartdialog.h"
#include <QAbstractItemModel>

lds_chartDialog::lds_chartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::lds_chartDialog)
{
    ui->setupUi(this);

    //! toolbar
    ui->widget->setup(QStringList() << tr("切换") << tr("退出"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("切换")), SIGNAL(clicked()),this,SLOT(toswitch()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));
}

lds_chartDialog::~lds_chartDialog()
{
    delete ui;
}

void lds_chartDialog::setmodel(QAbstractItemModel *model)
{
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(model);
    ui->tableView_2->setModel(model);
    ui->tableView_3->setModel(model);

    QItemSelectionModel *selectmodel=new QItemSelectionModel(model);
    ui->tableView->setSelectionModel(selectmodel);
    ui->tableView_2->setSelectionModel(selectmodel);
    ui->tableView_3->setSelectionModel(selectmodel);

//    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->tableView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

void lds_chartDialog::toswitch()
{
    ui->stackedWidget->setCurrentIndex(1-ui->stackedWidget->currentIndex());
}

void lds_chartDialog::toexit()
{
    this->reject();
}
