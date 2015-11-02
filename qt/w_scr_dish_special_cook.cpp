#include "w_scr_dish_special_cook.h"
#include <QSqlQuery>
#include <QSqlRecord>

w_scr_dish_special_cook::w_scr_dish_special_cook(const QString & dishno,const QString & neirong,const QString & price,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::w_scr_dish_special_cook),
    btn_t_f(false)
{
    ui->setupUi(this);
    //!---------------------------------1begin
    ui->frame->is_like_tab=false;
    int horizontal_count;
    int vertical_count;
    ui->frame->tool_size=QSize(96,80);
    horizontal_count=5;
    vertical_count=3;
    ui->frame->horizontal_count=horizontal_count;
    ui->frame->vertical_count=vertical_count;
    ui->frame->set_btn_property("pushbutton_purple");
    //!---------------------------------2end

    QSqlQuery query;
    query.exec(QString("SELECT a.ch_no,a.vch_cp_memo,a.ch_typeno,a.num_add_price FROM cey_bt_cp_memo a,cey_bt_cp_dish b WHERE(a.ch_no = b.ch_no)and( b.ch_dishno = '%1')")
               .arg(dishno));
    while(query.next()){
        ui->frame->add_item(query.record().value("ch_no").toString(),
                            QString("%1\n%2").arg(query.record().value("vch_cp_memo").toString()).arg(query.record().value("num_add_price").toString()),
                            false);
    }
    ui->frame->generat_from_items();

    ui->widget->setup(QStringList() << tr("所有做法") << tr("确定") << tr("取消"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("所有做法")),SIGNAL(clicked()),this,SLOT(to_showalldish()));
    connect(ui->widget->index_widget(tr("确定")),SIGNAL(clicked()),this,SLOT(to_ok()));
    connect(ui->widget->index_widget(tr("取消")),SIGNAL(clicked()),this,SLOT(to_exit()));

    connect(ui->frame,SIGNAL(signal_text(QString)),this,SLOT(to_update_labelreturn(QString)));

    query.exec(QString("select  vch_dishname from cey_bt_dish where ch_dishno='%1' ").arg(dishno));
    query.next();
    ui->label_state->setText(query.record().value("vch_dishname").toString());
    ui->label_return->setText(neirong);
   add_price = price.toFloat();
}

w_scr_dish_special_cook::~w_scr_dish_special_cook()
{
    delete ui;
}

void w_scr_dish_special_cook::to_showalldish()
{
    ui->label_state->setText(tr("所有做法"));
    ui->frame->clearall_item();
    QSqlQuery query;
    query.exec(QString("SELECT a.ch_no,a.vch_cp_memo,a.ch_typeno,a.num_add_price FROM cey_bt_cp_memo a"));
    while(query.next()){
        ui->frame->add_item(query.record().value("ch_no").toString(),
                            QString("%1\n%2").arg(query.record().value("vch_cp_memo").toString()).arg(query.record().value("num_add_price").toString()),
                            false);
    }
    ui->frame->generat_from_items();
}

void w_scr_dish_special_cook::to_ok()
{
    zuofaneirong=ui->label_return->text();
    btn_t_f=true;
    this->accept();
}

void w_scr_dish_special_cook::to_exit()
{
    this->reject();
}

void w_scr_dish_special_cook::to_update_labelreturn(const QString &dishno)
{
    QString namelist;
    float total=0.0;
    QStringList indexs=ui->frame->hasSelected();
    QSqlQuery query;
    foreach(QString index, indexs){
        query.exec(QString("SELECT a.ch_no,a.vch_cp_memo,a.ch_typeno,a.num_add_price FROM cey_bt_cp_memo a where a.ch_no='%1' ")
                   .arg(index));
        query.next();
        namelist+=query.record().value("vch_cp_memo").toString()+"+";

        total+=query.record().value("num_add_price").toFloat();
    }
    if(!namelist.isEmpty())namelist.chop(1);
    add_price=total;
    ui->label_return->setText(QString("%1=%2")
                              .arg(namelist)
                              .arg(QString().sprintf("%.02f",add_price)));
}
