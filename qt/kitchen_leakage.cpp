#include "kitchen_leakage.h"
#include "ui_kitchen_leakage.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDateTime>
#include "fast_order_2.h"
#include "public_mainprinter.h"
#include <QSettings>
#include "lds_messagebox.h"
#include <QSqlQuery>
#include "backheader.h"
#include"n_func.h"
#include <QTextEdit>
#include "lds_toolbar.h"
#include "lds_messagebox.h"
#include "w_bt_dish_kitchenprinter.h"
#include <QSqlError>


kitchen_leakage::kitchen_leakage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::kitchen_leakage)
{
    ui->setupUi(this);
    lostform_db=QSqlDatabase::database("db_kitchen");

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif
    tab_model=new QStandardItemModel;
    ui->tableView->setModel(tab_model);
    init_control();
    ui->dateTimeEdit->setDate(n_func::f_get_sysdatetime().date());
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    ui->checkBox_refresh->setChecked(conf.value("kitchen_leakage/update_refresh").toBool());
    ui->lineEdit_refresh_time->setText(conf.value("kitchen_leakage/update_refresh_time").toString());
    ui->checkBox_auto_print->setChecked(conf.value("kitchen_leakage/is_auto_print").toBool());
    connect(ui->checkBox_refresh,SIGNAL(clicked(bool)),this,SLOT(update_refresh(bool)));
    connect(ui->lineEdit_refresh_time,SIGNAL(textEdited(QString)),this,SLOT(update_refresh_time(QString)));
    connect(&refresh_timer,SIGNAL(timeout()),this,SLOT(torefresh()));
    connect(ui->checkBox_auto_print,SIGNAL(toggled(bool)),this,SLOT(to_auto_print(bool)));
    update_refresh(ui->checkBox_refresh->isChecked());
}

kitchen_leakage::~kitchen_leakage()
{
    delete ui;
}

void kitchen_leakage::init_control()
{
    ui->radioButton->setChecked(true);
    on_radioButton_clicked();
}

void kitchen_leakage::re_model()
{
    //    fast_order_2::model_dalei.setQuery(QString("SELECT  * FROM cey_bt_dish_type"));
    //    for(int i = 0; i < fast_order_2::model_dalei.rowCount(); i++)
    //    {
    //        if(i == fast_order_2::num_da)
    //        {
    //            fast_order_2::dalei = fast_order_2::model_dalei.record(i).value(0).toString();
    //        }
    //    }

    //    // model_xiaolei->clear();
    //    fast_order_2::model_xiaolei.setQuery(QString("SELECT  * FROM cey_bt_dish_subtype where ch_dish_typeno = '%1' ").arg(fast_order_2::dalei));
    //    for(int i = 0; i < fast_order_2::model_xiaolei.rowCount(); i++)
    //    {
    //        if(i == fast_order_2::num_xiao)
    //        {
    //            fast_order_2::xiaolei = fast_order_2::model_xiaolei.record(0).value(0).toString();
    //        }
    //    }


    //    //  model_cook->clear();
    //    if(fast_order_2::changyongcook == false)
    //    {
    //        fast_order_2::model_cook.setQuery(QString("SELECT  * FROM cey_bt_dish where ch_dish_typeno = '%1' and ch_sub_typeno ='%2' and (ch_stopflag = null or ch_stopflag = 'N') ").arg(fast_order_2::dalei).arg(fast_order_2::xiaolei));
    //    }
    //    else
    //    {
    //        fast_order_2::model_cook.setQuery(QString("select * from cey_bt_dish where ch_off = 'Y' aand (ch_stopflag = null or ch_stopflag = 'N') "));
    //    }
}

void kitchen_leakage::torefresh()
{
    if(ui->radioButton->isChecked()){
        ui->pushButton_8->setVisible(false);
        ui->pushButton_9->setVisible(false);
        ui->pushButton_11->setVisible(false);
        tab_model->clear();
        tab_model->setHorizontalHeaderLabels(QStringList() << tr("行号") << tr("单号") << tr("ip") << tr("日期")<< tr("类型"));


        QSqlQueryModel model;
        QSqlQueryModel model_pinming;
        QString pinming;
        model.setQuery(QString("select * from cey_u_orderdish where dt_operdate >= '%1' and ch_printflag = 'Y'  and ifnull(ch_outflag,'')  <>'Y' ")
                       .arg(ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"))
                       );
        qDebug() << model.query().lastQuery();
        for(int i = 0; i < model.rowCount(); i++)
        {
            model_pinming.clear();
            model_pinming.setQuery(QString("select vch_dishname from cey_bt_dish where ch_dishno ='%1' ").arg(model.record(i).value(4).toString()));
            pinming = model_pinming.record(0).value(0).toString();
            int row = tab_model->rowCount();
            tab_model->setRowCount(row + 1);
            tab_model->setItem(row,0,new QStandardItem(QString::number(row+1)));
            tab_model->item(row,0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled); //序号
            tab_model->setItem(row,1,new QStandardItem(model.record(i).value(1).toString()));
            tab_model->item(row,1)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled); //单号
            tab_model->setItem(row,2,new QStandardItem(pinming));
            tab_model->item(row,2)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);//品名
            tab_model->setItem(row,3,new QStandardItem(model.record(i).value(26).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));
            tab_model->item(row,3)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled); //日期
            tab_model->setItem(row,4,new QStandardItem(tr("厨打")));
            tab_model->item(row,4)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled); //厨打
        }

        model.setQuery(QString("select * from cey_u_orderdish where dt_operdate >= '%1' and ifnull(ch_printflag,'')  <> 'Y'  and ch_outflag ='Y'  ")
                       .arg(ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"))
                       );
        qDebug() << model.query().lastQuery();

        for(int i = 0; i < model.rowCount(); i++)
        {
            model_pinming.clear();
            model_pinming.setQuery(QString("select vch_dishname from cey_bt_dish where ch_dishno ='%1' ").arg(model.record(i).value(4).toString()));
            pinming = model_pinming.record(0).value(0).toString();
            int row = tab_model->rowCount();
            tab_model->setRowCount(row + 1);
            tab_model->setItem(row,0,new QStandardItem(QString::number(row+1)));
            tab_model->item(row,0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled); //序号
            tab_model->setItem(row,1,new QStandardItem(model.record(i).value(1).toString()));
            tab_model->item(row,1)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled); //单号
            tab_model->setItem(row,2,new QStandardItem(pinming));
            tab_model->item(row,2)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);//品名
            tab_model->setItem(row,3,new QStandardItem(model.record(i).value(26).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));
            tab_model->item(row,3)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled); //日期
            tab_model->setItem(row,4,new QStandardItem(tr("出品")));
            tab_model->item(row,4)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled); //出品
        }

        model.setQuery(QString("select * from cey_u_orderdish where dt_operdate >= '%1' and ch_printflag = 'Y'  and ch_outflag ='Y'  ")
                       .arg(ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"))
                       );
        qDebug() << model.query().lastQuery();

        for(int i = 0; i < model.rowCount(); i++)
        {
            model_pinming.clear();
            model_pinming.setQuery(QString("select vch_dishname from cey_bt_dish where ch_dishno ='%1' ").arg(model.record(i).value(4).toString()));
            pinming = model_pinming.record(0).value(0).toString();
            int row = tab_model->rowCount();
            tab_model->setRowCount(row + 1);
            tab_model->setItem(row,0,new QStandardItem(QString::number(row+1)));
            tab_model->item(row,0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled); //序号
            tab_model->setItem(row,1,new QStandardItem(model.record(i).value(1).toString()));
            tab_model->item(row,1)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled); //单号
            tab_model->setItem(row,2,new QStandardItem(pinming));
            tab_model->item(row,2)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);//品名
            tab_model->setItem(row,3,new QStandardItem(model.record(i).value(26).toDateTime().toString("yyyy-MM-dd hh:mm:ss")));
            tab_model->item(row,3)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled); //日期
            tab_model->setItem(row,4,new QStandardItem(tr("厨打,出品")));
            tab_model->item(row,4)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled); //出品
        }
    }

    if(ui->radioButton_2->isChecked()){
        ui->pushButton_8->setVisible(true);
        ui->pushButton_9->setVisible(true);
        ui->pushButton_11->setVisible(true);

        tab_model->clear();
        tab_model->setHorizontalHeaderLabels(QStringList() << tr("行号") << tr("单号") << tr("ip") << tr("日期")<< tr("类型"));
        QSqlQuery query(lostform_db);
        query.exec(QString("select * from loss_form_table where dt_date >= '%1' ")
                   .arg(ui->dateTimeEdit->dateTime().toString("yyyy-MM-dd hh:mm:ss"))
                   );
        while(query.next()){
            QList<QStandardItem *> items;
            items<<new QStandardItem(query.record().value("int_id").toString());items.last()->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            items<<new QStandardItem(query.record().value("vch_billno").toString());items.last()->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            items<<new QStandardItem(query.record().value("vch_ip").toString());items.last()->setFlags(Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            items<<new QStandardItem(query.record().value("dt_date").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));items.last()->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            items<<new QStandardItem(w_bt_dish_Kitchenprinter::value_of_kitchenkey(
                                         query.record().value("vch_type").toString())
                                     );items.last()->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            tab_model->appendRow(items);
        }
    }
    ui->tableView->restore();
}

void kitchen_leakage::to_auto_print(bool flag)
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    conf.setValue("kitchen_leakage/is_auto_print", flag);
    conf.sync();
}

void kitchen_leakage::on_pushButton_7_clicked()
{
    this->close();
}

void kitchen_leakage::on_radioButton_clicked()//已打单
{
    torefresh();
}

void kitchen_leakage::on_radioButton_2_clicked()//漏打单
{
    torefresh();
}

void kitchen_leakage::on_pushButton_8_clicked()//补打
{
    int row = ui->tableView->currentIndex().row();

    if(row <= -1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择一条需要打印的数据!"));

        return;
    }
    QSqlQuery query(QSqlDatabase::database("db_kitchen"));
    query.exec(QString("select * from loss_form_table where int_id = '%1' ")
               .arg(tab_model->item(row, 0)->text()));
    query.next();
    QByteArray byte=query.record().value("vch_content").toByteArray();
    QDataStream in(&byte, QIODevice::ReadOnly);
    blockIns printdata;
    while(!in.atEnd()){
        pieceIns pins;
        in>>pins.first;
        in>>pins.second;
        printdata.append(pins);
    }

    if(backheader::syn_dataprint(printdata, query.record().value("vch_ip").toString())){
        query.exec(QString("delete from loss_form_table where int_id='%1' ")
                   .arg(query.record().value("int_id").toString()));
    }
    torefresh();
}


void kitchen_leakage::on_pushButton_9_clicked()//查看详情
{
    int row = ui->tableView->currentIndex().row();
    if(row <= -1)
    {
        lds_messagebox::warning(this, tr("消息提示"),tr("请选择一条需要打印的数据!"));
        
        return;
    }
    QSqlQuery query(lostform_db);
    query.exec(QString("select vch_content from loss_form_table where int_id = '%1' ")
               .arg(tab_model->item(row, 0)->text()));
    query.next();
    QByteArray byte=query.record().value(0).toByteArray();
    QDataStream in(&byte, QIODevice::ReadOnly);
    blockIns printdata;
    while(!in.atEnd()){
        pieceIns pins;
        in>>pins.first;
        in>>pins.second;
        printdata.append(pins);
    }

    QDialog dialog;
    lds_toolbar *toolbar=new lds_toolbar(&dialog);
    toolbar->setup(QStringList() << tr("打印") <<  tr("退出"), Qt::AlignRight);
    connect(toolbar->index_widget(tr("退出")), SIGNAL(clicked()),&dialog, SLOT(reject()));
    connect(toolbar->index_widget(tr("打印")), SIGNAL(clicked()), this,SLOT(on_pushButton_8_clicked()));

    QTextEdit *textedit=new QTextEdit(&dialog);
    foreach(pieceIns p, printdata)
        textedit->append(p.second);
    QVBoxLayout *vlayout=new QVBoxLayout;
    vlayout->addWidget(textedit);
    vlayout->addWidget(toolbar);
    dialog.setLayout(vlayout);
    dialog.resize(backheader::mainwindowsize);
    backheader::static_dialogexec(dialog, tr("详情"));
}

void kitchen_leakage::on_pushButton_10_clicked()//刷新
{
    torefresh();
}

void kitchen_leakage::update_refresh(bool flag)
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    conf.setValue("kitchen_leakage/update_refresh", flag);
    if(flag)
        refresh_timer.start(qMax(ui->lineEdit_refresh_time->text().toInt(), 1000));
    else
        refresh_timer.stop();
}

void kitchen_leakage::update_refresh_time(const QString &time)
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    conf.setValue("kitchen_leakage/update_refresh_time", time);
}

void kitchen_leakage::on_pushButton_11_clicked()
{
    QSqlQuery query(lostform_db);
   if(query.exec("delete from loss_form_table")){
       lds_messagebox::warning(this,tr("删除漏打单"), tr("删除漏打单成功"));
   } else {
       lds_messagebox::warning(this,tr("删除漏打单"), tr("删除漏打单失败"));
   }
}
