#include "lds_selectgoods.h"
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QSqlQuery>
#include <QSqlRecord>
#include "backheader.h"
#include <QMenu>
#include <QWidgetAction>
#include <QPushButton>
#include "lds_menu.h"
#include "backheader.h"

lds_selectgoods::lds_selectgoods(QWidget *parent) :
    QWidget(parent)
{
    //layout
    line=new QLineEdit(this);
    QPushButton *tool=new QPushButton(this);tool->setText("商品检索");tool->setProperty("outer_stylesheet", "pushbutton");
    QHBoxLayout *hlayout=new QHBoxLayout;

    tool->setMaximumSize(60,30);
    tool->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tool->setFocusPolicy(Qt::NoFocus);
    line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    hlayout->addWidget(line,1);
    hlayout->addWidget(tool);
    hlayout->setMargin(0);
    hlayout->setSpacing(0);

    setLayout(hlayout);
    connect(tool,SIGNAL(clicked()),this,SLOT(show_Selectgoods()));

    //menu
    //    lds_menu *menu=new lds_menu;
    //    QWidgetAction *widgetaction=new QWidgetAction(tool);
    //    Selectgoods *action=new Selectgoods(menu);
    //    widgetaction->setDefaultWidget(action);
    //    menu->addAction(widgetaction);
    //    tool->setMenu(menu);

    //    //after
    //    connect(action,SIGNAL(okstr(QString)),line,SLOT(setText(QString)));
    //    connect(action,SIGNAL(okstr1(QString)),this,SIGNAL(okstr1(QString)));

    this->setFixedHeight(34);

}

QString lds_selectgoods::gettext()
{
    return line->text();
}

void lds_selectgoods::settext(const QString &text)
{
    line->setText(text);
}

void lds_selectgoods::show_Selectgoods()
{
    Selectgoods dialog;
    connect(&dialog,SIGNAL(okstr(QString)),line,SLOT(setText(QString)));
    connect(&dialog,SIGNAL(okstr1(QString)),this,SIGNAL(okstr1(QString)));
    backheader::static_dialogexec(dialog, qobject_cast<QAbstractButton* >(sender())->text());
}


//=========================================
#include "lds_model_sqlrelationaltablemodel.h"
#include "ui_selectgoods_dialog.h"

Selectgoods::Selectgoods(lds_menu *parent):
    QDialog(parent),
    ui(new Ui_selectgoods_Widget),
    _parent(parent)
{
    ui->setupUi(this);

    //! tableview
    tablemodel=new lds_model_sqlrelationaltablemodel(this);
    tablemodel->setTable("cey_bt_dish");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);tablemodel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    tablemodel->setRelation(4/*ch_dish_typeno*/, QSqlRelation("cey_bt_dish_type", "ch_dish_typeno","vch_dish_typename"));
    tablemodel->setRelation(14/*ch_sub_typeno*/, QSqlRelation("cey_bt_dish_subtype", "ch_sub_typeno","vch_sub_typename"));
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel->fpmodel);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

//    ui->tableView->setSortingEnabled(true);
    //! toselect
    toselect();
    //重新布局表头和隐藏相关表头
    QStringList headerstmp;
    QList<int> virtualheades;
    tablemodel->setHeaderData(0,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("品码"),headerstmp));
    tablemodel->setHeaderData(1,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("品名"),headerstmp));
    tablemodel->setHeaderData(4,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("大类"),headerstmp));
    tablemodel->setHeaderData(14,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("小类"),headerstmp));
    virtualheades=ui->tableView->rePecifyHeader( headerstmp);

    //! resize
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(took()));
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(toselect(QString)));

    ui->pushButton->setProperty("outer_stylesheet",QVariant("pushbutton"));
    this->setFixedSize(500, 300);

}

void Selectgoods::setselectmultimode()
{
    ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);
}

void Selectgoods::setitem_flag(const QChar &flag)
{
    _item_flag=QString(flag);
    ui->label->setText(ui->label->text()+tr("[直销商品]"));
    toselect(ui->lineEdit->text());
}

void Selectgoods::setstcheck_sheetno()
{
    setitem_flag('Y');//直销商品
    _check_sheetno =
            " ("
            "not exists (select cey_st_check_detail.ch_materialno "
            " from cey_st_check_master, cey_st_check_detail"
            " where cey_st_check_master.ch_sheetno = cey_st_check_detail.ch_sheetno"
            " and cey_st_check_master.ch_state = 'N'  and cey_st_check_detail.ch_materialno = cey_bt_dish.ch_dishno)"
            ")"
            ;

    ui->label->setText(ui->label->text()+tr("[盘点商品]"));
    toselect(ui->lineEdit->text());
}

void Selectgoods::toselect(const QString &filter)
{
    QString sqlselect;

    if(filter.isEmpty()){

    } else {
        sqlselect +=QString(" (ch_dishno like '%1%' or vch_dishname like '%2%' or vch_dish_typename like '%3%' or vch_sub_typename like '%4%')and")
                .arg(filter)
                .arg(filter)
                .arg(filter)
                .arg(filter)
                ;
    }
    //是否停用
    sqlselect +=QString(" ch_stopflag like 'N' ");

    //外部直销商品过滤
    if(_item_flag.isEmpty()){

    } else {
        sqlselect +=QString(" and item_flag='%1' ")
                .arg(_item_flag);
    }

    //外部盘点过滤
    if(_check_sheetno.isEmpty()){

    } else {
        sqlselect +=QString(" and %1 ")
                .arg(_check_sheetno);
    }

    qDebug() << "sqlselect" << sqlselect;
    //检索
    tablemodel->setFilter(sqlselect);
    tablemodel->select();
    tablemodel->relationModel(4)->select();
    tablemodel->relationModel(14)->select();
}

void Selectgoods::took()
{
    QString linestr;
    QString vch_name;

    if(ui->tableView->currentIndex().row()>=0){
        linestr=tablemodel->record(ui->tableView->currentIndex().row()).value("ch_dishno").toString();
        vch_name=tablemodel->record(ui->tableView->currentIndex().row()).value("vch_dishname").toString();
        emit okstr(linestr);
        emit okstr1(vch_name);
    }

    if(ui->tableView->selectionMode()&QAbstractItemView::MultiSelection){
        QStringList infos;
        QSet<int> rows=ui->tableView->getcurrows();
        foreach(int row, rows){
            infos.append(tablemodel->record(row).value("ch_dishno").toString());
        }
        emit okstrs(infos);
    }

    ui->tableView->clearSelection();
    if(_parent)
        _parent->close();
    else
        this->accept();
}
