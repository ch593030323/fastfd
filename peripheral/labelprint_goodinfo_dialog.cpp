#include "labelprint_goodinfo_dialog.h"
#include "ui_labelprint_goodinfo_dialog.h"
#include <QtDebug>
#include "ui_labelprint_goodinfo_add_dialog.h"
#include "commonheader.h"
#include "labelprint_goodinfo_add_dialog.h"

labelprint_goodinfo_dialog::labelprint_goodinfo_dialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::labelprint_goodinfo_dialog),
    isfinishinput(false)
{
    ui->setupUi(this);setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);this->setLayoutDirection(CommonHeader::com_widget_dir);
    this->setWindowTitle(tr("商品信息"));

    printmodel=new QStandardItemModel;
    inputallgoods_model=new QStandardItemModel;
    ui->tableView->setModel(printmodel);
    ui->tableView->setEditTriggers(QTableView::DoubleClicked|QTableView::SelectedClicked);
//    ui->tableView->setSelectionMode(QTableView::MultiSelection);

    connect(ui->pushButton_goodadd,SIGNAL(clicked()),this,SIGNAL(signal_add()));
    connect(ui->pushButton_goodremove,SIGNAL(clicked()),this,SLOT(togoodremove()));
    connect(printmodel,SIGNAL(rowsInserted(QModelIndex,int,int)),this,SLOT(printmodel_has_newrow(QModelIndex,int,int)));
    //    QTableView *tableview=new QTableView;
    //    tableview->setModel(inputallgoods_model);
    //    tableview->show();
    connect(ui->tableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(tocommvalue(int)));

}

labelprint_goodinfo_dialog::~labelprint_goodinfo_dialog()
{
    delete ui;
    delete inputallgoods_model;
}

void labelprint_goodinfo_dialog::tocommvalue(int column)
{
    QAbstractItemModel *model=ui->tableView->model();
    qDebug() << "sectionclicked" << model;
    if(model){
        QString first=model->index(0, column).data().toString();
        for(int r=0,rcount=model->rowCount();r<rcount;r++){
            model->setData(model->index(r, column), first);
        }
    }
}

void labelprint_goodinfo_dialog::togoodremove()
{
    //1
    QModelIndexList indexlist=ui->tableView->selectionModel()->selectedIndexes();
    QSet<int> index_set;
    foreach(QModelIndex index, indexlist){
        index_set.insert(index.row());
    }
    QList<int> index_list=index_set.toList();
    //2
    qSort(index_list.begin(), index_list.end(), qGreater<int>());
    foreach(int index, index_list){
        printmodel->removeRow(index);
    }
}

void labelprint_goodinfo_dialog::printmodel_has_newrow(const QModelIndex &parent, int first, int last)
{
    if(isfinishinput) return;
    if(first==0){
        for(int c=0,ccount=printmodel->columnCount();c<ccount;c++){
            inputallgoods_model->setHorizontalHeaderItem(c, new QStandardItem(printmodel->headerData(c, Qt::Horizontal).toString()));
        }
    }
    QList<QStandardItem*> items;
    for(int c=0,ccount=printmodel->columnCount();c<ccount;c++){
        items.append(new QStandardItem(printmodel->item(first, c)->text()));
    }
    inputallgoods_model->appendRow(items);
}
