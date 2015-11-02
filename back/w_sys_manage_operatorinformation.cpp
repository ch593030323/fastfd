#include "w_sys_manage_operatorinformation.h"
//操作员信息
#include "ui_w_bt_dish_unitset_dialog.h"
#include "lds_model_sqlrelationaltablemodel.h"
#include <QDataWidgetMapper>
#include <QSqlRelationalDelegate>
#include "w_sys_manage.h"
#include "lds_messagebox.h"
#include <QSqlError>
#include <QTimer>
#include "backheader.h"
#include "lds_tableview_delegate_check.h"
#include "ui_w_sys_manage_operatorinformation_dialog.h"
w_sys_manage_operatorinformation::w_sys_manage_operatorinformation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui_w_sys_manage_operatorinformation_Dialog)
{
    ui->setupUi(this);

    //! tableview
    tablemodel=new lds_model_sqlrelationaltablemodel(this);
    tablemodel->setTable("cey_sys_operator");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);tablemodel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    ui->tableView->setModel(tablemodel->fpmodel);

    tablemodel->save_set_header("vch_operID", tr("代码"));
    tablemodel->save_set_header("vch_operator", tr("姓名"));
    tablemodel->save_set_header("ch_flag", tr("是否生效"));
    QList<int> virtualheades=ui->tableView->rePecifyHeader(tablemodel->save_set_header_saver);
    ui->tableView->setItemDelegate(new lds_tableview_delegate_check(QVector<int>()
                                                                    << virtualheades[2]
                                   ,
                                   ui->tableView));
    tablemodel->select();
    //! datawidget
    backheader::lds_com_setdatas(&w_sys_manage::MODEL_SYS_DISH_CONTROL, ui->comboBox_ch_pre_mode);

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setModel(tablemodel);
    mapper->setItemDelegate(new QSqlRelationalDelegate(mapper));
    mapper->addMapping(ui->lineEdit_int_discount,tablemodel->fieldIndex("int_discount"));
    mapper->addMapping(ui->lineEdit_num_free,tablemodel->fieldIndex("num_free"));
    mapper->addMapping(ui->lineEdit_num_present,tablemodel->fieldIndex("num_present"));
    mapper->addMapping(ui->lineEdit_vch_operator,tablemodel->fieldIndex("vch_operator"));
    mapper->addMapping(ui->lineEdit_vch_operid,tablemodel->fieldIndex("vch_operid"));
    mapper->addMapping(ui->lineEdit_vch_password,tablemodel->fieldIndex("vch_password"));

    mapper->addMapping(ui->checkBox_ch_flag,tablemodel->fieldIndex("ch_flag"), "checked1");//将bool 转换为Y N
    mapper->addMapping(ui->checkBox_ch_free_flag,tablemodel->fieldIndex("ch_free_flag"), "checked1");//将bool 转换为Y N
    mapper->addMapping(ui->checkBox_ch_waiter,tablemodel->fieldIndex("ch_waiter"), "checked1");//将bool 转换为Y N
    mapper->addMapping(ui->comboBox_ch_pre_mode, tablemodel->fieldIndex("ch_pre_mode"), "comusrdata");//根据需要将index-1

    //! toolbar
    ui->widget->setup(QStringList() << tr("新增") << tr("刷新") << tr("删除") << tr("保存") << tr("退出"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("新增")), SIGNAL(clicked()),this,SLOT(tonew()));
    connect(ui->widget->index_widget(tr("刷新")), SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("删除")), SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget->index_widget(tr("保存")), SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->lineEdit_vch_operid,SIGNAL(textChanged(QString)),ui->lineEdit_vch_password, SLOT(setText(QString)));
    connect(ui->tableView,SIGNAL(selectchanged(int)),mapper,SLOT(setCurrentIndex(int)));

    //! ----------------------这儿采用下面的方法,来实现textChanged 对lineedit的实现---------------------begin
    connect(ui->lineEdit_vch_operid,SIGNAL(editingFinished()),ui->lineEdit_vch_password, SIGNAL(editingFinished()));
    connect(ui->lineEdit_vch_password,SIGNAL(editingFinished()),this,SLOT(mapper_setcur_tableviewcur()));
    connect(this,SIGNAL(lineedit_connect_change(QWidget *)), mapper->itemDelegate(),SIGNAL(commitData(QWidget*)));
    ui->lineEdit_vch_password->setEchoMode(QLineEdit::Password);
    //! ----------------------这儿采用下面的方法,来实现textChanged 对lineedit的实现---------------------end
    //    ui->lineEdit_vch_password->setEnabled(false);
    mapper->toFirst();
}

void w_sys_manage_operatorinformation::tonew()
{
    int rowcount=tablemodel->rowCount();
    tablemodel->insertRow(rowcount);
    ui->tableView->selectRow(rowcount);
    mapper->setCurrentIndex(rowcount);
    tablemodel->setData(tablemodel->index(rowcount, 3), "Y");
}

void w_sys_manage_operatorinformation::torefresh()
{
    tablemodel->select();
    ui->tableView->restore();
    mapper->setCurrentIndex(ui->tableView->currentIndex().row());
}

void w_sys_manage_operatorinformation::todel()
{
    tablemodel->removeRow(ui->tableView->currentIndex().row());
    tablemodel->submitAll();
}

bool w_sys_manage_operatorinformation::tosave()
{
    if(tablemodel->same_column_exist("vch_operID")){
        lds_messagebox::warning(this, tr("消息提示"),tr("编码有重复，请核实!"));
        return false;
    }
    if(tablemodel->submitAll()){
        //        torefresh();
        lds_messagebox::warning(this, tr("消息提示"),tr("保存成功!"));
        return true;
    } else {
        lds_messagebox::warning(this, tr("消息提示"),tablemodel->lastError().text());
        return false;
    }
}

void w_sys_manage_operatorinformation::toexit()
{
    if(tablemodel->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?")+tablemodel->dirty_err_string(), tr("确认"),tr("取消"))){
            if(!tosave()){
                return;
            }
        }
    }

    this->reject();
}

void w_sys_manage_operatorinformation::mapper_setcur_tableviewcur()
{
    emit lineedit_connect_change(ui->lineEdit_vch_password);
}

void w_sys_manage_operatorinformation::lineeditfinish()
{
    emit lineedit_connect_change(ui->lineEdit_vch_password);
}
