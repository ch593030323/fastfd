#include "special_query.h"
#include "ui_special_query.h"
#include <QStandardItemModel>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>

special_query::special_query(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::special_query)
{
    ui->setupUi(this);
    init_control();
}

special_query::~special_query()
{
    delete ui;
}

void special_query::init_control()
{
//    ui->tableView->verticalHeader()->hide();
//    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: qlineargradient(spread:reflect, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(132, 132, 132, 255), stop:1 rgba(105, 105,105, 255));color:white;height:35px;border-radius:0px;font-size:10pt;}");
//    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//    //    ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    //    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    ui->tableView->verticalHeader()->setDefaultSectionSize(40);
//    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置属性为只读

//#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
//    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents );//文字自适应表格
//#else
//    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents );//文字自适应表格
//#endif
    QStringList labels;
    labels<<"行号"<<"日期"<<"品码"<<"品名"<<"特价"<<"状态"
        <<"有效日期"<<"操作员"<<"操作时间"<<"取消人"<<"取消时间";
    model = new QStandardItemModel();
    model->setHorizontalHeaderLabels(labels);
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(model);

    QSqlQueryModel model1;
    model1.setQuery("Select NOW()");
    QDateTime time =model1.record(0).value(0).toDateTime();
    QString str = time.toString("yyyy-MM-dd");
    QString str1 = "00:00";
    QString str2 = str + " " + str1;
    QDateTime time_box1;
    time_box1 = QDateTime::fromString(str2, "yyyy-MM-dd hh:mm");
    ui->dateTimeEdit->setDateTime(time_box1);
    ui->dateTimeEdit_2->setDateTime(model1.record(0).value(0).toDateTime());
    QString date1 = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd");
    QString date2 = ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd");

    QSqlQueryModel model_table;
   // model_table.prepare(QString(" SELECT  left(a.dt_date,10),a.ch_dishno,b.vch_dishname,a.num_price,'state' = (case when a.ch_state = 'N' then :quxiao when left(current_date(),10) > left(a.dt_date,10) then :guoqi else :youxiao end) ,  a.dt_operdate_effect, a.vch_operID, a.dt_operdate,a.vch_operID2,a.dt_operdate2 FROM cey_bt_dish b,  cey_bt_dish_special a WHERE(a.ch_dishno = b.ch_dishno)and(left(current_date(),10),left(a.dt_operdate_effect,10) >= :date1)and(left(a.dt_operdate_effect ,10) <= :date2)"));
  //  model_table.bindValue(":quxiao",tr("取消"));
  //  model_table.bindValue(":guoqi",tr("过期"));
  //  model_table.bindValue(":youxiao",tr("有效"));
    model_table.setQuery(QString("SELECT left(a.dt_date,10), a.ch_dishno, b.vch_dishname, a.num_price, (case when a.ch_state = 'N' then '取消' when left(current_date(),10) > left(a.dt_date,10) then '过期' else '有效' end) , left(a.dt_operdate_effect,10),a.vch_operID, a.dt_operdate,a.vch_operID2, a.dt_operdate2   FROM cey_bt_dish b,cey_bt_dish_special a  WHERE(a.ch_dishno = b.ch_dishno)and( left(a.dt_operdate_effect,10) >= '%1' and( left(a.dt_operdate_effect,10) <= '%2')) ").arg(date1).arg(date2));
   // model_table.bindValue(":date1",date1);
   // model_table.bindValue(":date2",date2);
   // model_table.exec();
  //  int i = -1;
    for(int i = 0; i < model_table.rowCount(); i++)
    {
        int row = model->rowCount();
        model->setRowCount(row + 1);
        model->setItem(row,0,new QStandardItem(QString::number(i+1))); //序号
        QString str3 = model_table.record(i).value(0).toDateTime().toString("yyyy-MM-dd");
        model->setItem(row,1,new QStandardItem(str3));
        model->setItem(row,2,new QStandardItem(model_table.record(i).value(1).toString()));
        model->setItem(row,3,new QStandardItem(model_table.record(i).value(2).toString()));
        float cc = 0.00;
        cc = model_table.record(i).value(3).toFloat();
        model->setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",cc)));
        model->setItem(row,5,new QStandardItem(model_table.record(i).value(4).toString()));
        model->setItem(row,6,new QStandardItem(model_table.record(i).value(5).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));
        model->setItem(row,7,new QStandardItem(model_table.record(i).value(6).toString()));
        model->setItem(row,8,new QStandardItem(model_table.record(i).value(7).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));
        model->setItem(row,9,new QStandardItem(model_table.record(i).value(8).toString()));
        model->setItem(row,10,new QStandardItem(model_table.record(i).value(9).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));
    }

    if(model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
    }
}
void special_query::on_pushButton_5_clicked()//退出
{
    this->close();
}

void special_query::on_pushButton_4_clicked()//检索
{
    model->clear();
    QStringList labels;
    labels<<"行号"<<"日期"<<"品码"<<"品名"<<"特价"<<"状态"
        <<"有效日期"<<"操作员"<<"操作时间"<<"取消人"<<"取消时间";
    model->setHorizontalHeaderLabels(labels);
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(model);

    QString date1 = ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd");
    QString date2 = ui->dateTimeEdit_2->dateTime().toString("yyyy-MM-dd");

    QSqlQueryModel model_table;
   // model_table.prepare(QString(" SELECT  left(a.dt_date,10),a.ch_dishno,b.vch_dishname,a.num_price,'state' = (case when a.ch_state = 'N' then :quxiao when left(current_date(),10) > left(a.dt_date,10) then :guoqi else :youxiao end) ,  a.dt_operdate_effect, a.vch_operID, a.dt_operdate,a.vch_operID2,a.dt_operdate2 FROM cey_bt_dish b,  cey_bt_dish_special a WHERE(a.ch_dishno = b.ch_dishno)and(left(current_date(),10),left(a.dt_operdate_effect,10) >= :date1)and(left(a.dt_operdate_effect ,10) <= :date2)"));
  //  model_table.bindValue(":quxiao",tr("取消"));
  //  model_table.bindValue(":guoqi",tr("过期"));
  //  model_table.bindValue(":youxiao",tr("有效"));
    model_table.setQuery(QString("SELECT left(a.dt_date,10), a.ch_dishno, b.vch_dishname, a.num_price, (case when a.ch_state = 'N' then '取消' when left(current_date(),10) > left(a.dt_date,10) then '过期' else '有效' end) , left(a.dt_operdate_effect,10),a.vch_operID, a.dt_operdate,a.vch_operID2, a.dt_operdate2   FROM cey_bt_dish b,cey_bt_dish_special a  WHERE(a.ch_dishno = b.ch_dishno)and( left(a.dt_operdate_effect,10) >= '%1' and( left(a.dt_operdate_effect,10) <= '%2')) ").arg(date1).arg(date2));
   // model_table.bindValue(":date1",date1);
   // model_table.bindValue(":date2",date2);
   // model_table.exec();
  //  int i = -1;
    for(int i = 0; i < model_table.rowCount(); i++)
    {
        int row = model->rowCount();
        model->setRowCount(row + 1);
        model->setItem(row,0,new QStandardItem(QString::number(i+1))); //序号
        QString str3 = model_table.record(i).value(0).toDateTime().toString("yyyy-MM-dd");
        model->setItem(row,1,new QStandardItem(str3));
        model->setItem(row,2,new QStandardItem(model_table.record(i).value(1).toString()));
        model->setItem(row,3,new QStandardItem(model_table.record(i).value(2).toString()));
        float cc = 0.00;
        cc = model_table.record(i).value(3).toFloat();
        model->setItem(row,4,new QStandardItem(QString().sprintf("%0.2f",cc)));
        model->setItem(row,5,new QStandardItem(model_table.record(i).value(4).toString()));
        model->setItem(row,6,new QStandardItem(model_table.record(i).value(5).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));
        model->setItem(row,7,new QStandardItem(model_table.record(i).value(6).toString()));
        model->setItem(row,8,new QStandardItem(model_table.record(i).value(7).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));
        model->setItem(row,9,new QStandardItem(model_table.record(i).value(8).toString()));
        model->setItem(row,10,new QStandardItem(model_table.record(i).value(9).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));
    }

    if(model->rowCount() > 0)
    {
        ui->tableView->selectRow(0);
    }
}
