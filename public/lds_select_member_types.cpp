#include "lds_select_member_types.h"
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

lds_Select_member_types::lds_Select_member_types(QWidget *parent) :
    QWidget(parent)
{
    //layout
    line=new QLineEdit(this);
    QPushButton *tool=new QPushButton(this);tool->setText("会员检索");
    QHBoxLayout *hlayout=new QHBoxLayout;

    tool->setMaximumSize(70,40);
    tool->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tool->setFocusPolicy(Qt::NoFocus);
    line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    hlayout->addWidget(line,1);
    hlayout->addWidget(tool);
    hlayout->setMargin(0);
    hlayout->setSpacing(0);

    setLayout(hlayout);
    connect(tool,SIGNAL(clicked()),this,SLOT(show_member_types()));

    //menu
//    lds_menu *menu=new lds_menu(this);menu->setWindowFlags(Qt::Widget);
//    QWidgetAction *widgetaction=new QWidgetAction(tool);
//    Select_member_types *action=new Select_member_types(menu);
//    widgetaction->setDefaultWidget(action);action->setWindowFlags(Qt::Widget);
//    menu->addAction(widgetaction);
//    tool->setMenu(menu);

//    //after
}

QString lds_Select_member_types::gettext()
{
    return line->text();
}

void lds_Select_member_types::settext(const QString &text)
{
    line->setText(text);
}

void lds_Select_member_types::show_member_types()
{
    Select_member_types dialog;
    connect(&dialog,SIGNAL(okstr(QString)),line,SLOT(setText(QString)));
    backheader::static_dialogexec(dialog, qobject_cast<QAbstractButton* >(sender())->text());
}

//==============================
#include "ui_selectgoods_dialog.h"
#include <QDateTime>
#include "w_m_member.h"

QVariant Select_member_types::RelationalTableModel::data(const QModelIndex &item, int role) const
{
    QVariant _d=lds_model_sqlrelationaltablemodel::data(item, role);
    int column=item.column();
    if(role==Qt::DisplayRole){
        if(column==4){//生日
            return _d.toDateTime().toString("yyyy-MM-dd");
        }
        if(column==9){//状态
            return  backheader::VIPFINDVALUE(w_m_member::VIP_MODEL_STATE, _d.toString());
        }

        if(column==11){//持卡
            return  backheader::VIPFINDVALUE(w_m_member::VIP_MODEL_FLAG, _d.toString());
        }
    }

    return _d;
}

Qt::ItemFlags Select_member_types::RelationalTableModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

Select_member_types::Select_member_types(QWidget *parent):
    QDialog(parent),
    ui(new Ui_selectgoods_Widget)
{
    ui->setupUi(this);

//    ui->tableView->setSortingEnabled(true);
    //! tableview
    QStringList headerstmp_2;
    tablemodel=new RelationalTableModel(this);
    tablemodel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);tablemodel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    tablemodel->setTable("t_m_member");
    tablemodel->setRelation(2/*ch_typeno*/, QSqlRelation("t_m_member_type", "ch_typeno","vch_typename"));
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel->fpmodel);
    tablemodel->setHeaderData(0,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("会员编号"),headerstmp_2));
    tablemodel->setHeaderData(1,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("会员名称"),headerstmp_2));
    tablemodel->setHeaderData(2,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("会员类型"),headerstmp_2));
    tablemodel->setHeaderData(4,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("生日"),headerstmp_2));
    tablemodel->setHeaderData(5,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("手机"),headerstmp_2));
    tablemodel->setHeaderData(6,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("电话"),headerstmp_2));
    tablemodel->setHeaderData(9,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("状态"),headerstmp_2));
    tablemodel->setHeaderData(11,Qt::Horizontal,lds_tableview_repecify::saveHeader(tr("持卡"),headerstmp_2));

    ui->tableView->rePecifyHeader( headerstmp_2);
    //! toselect
    toselect();

    //! resize
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(took()));
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(toselect(QString)));

    this->setProperty("outer_stylesheet",QVariant("pushbutton"));
    ui->lineEdit->setPlaceholderText("<--[会员编号][会员名称][手机][电话]关键字-->");
    this->setFixedSize(500, 300);
}

void Select_member_types::toselect(const QString &filter)
{
    QString sqlselect = QString("(vch_memberno like '%1%' or vch_member like '%1%')or (ifnull(vch_tel, '') like '%2%' or ifnull(vch_handtel, '') like '%2%') ")
            .arg(filter)
            .arg(filter);
    //检索
    tablemodel->setFilter(sqlselect);
    tablemodel->select();
    tablemodel->relationModel(2)->select();
}

void Select_member_types::took()
{
    if(ui->tableView->currentIndex().row()>=0){
        emit okstr(tablemodel->record(ui->tableView->currentIndex().row()).value("vch_memberno").toString());
    }
    ui->tableView->clearSelection();
    this->accept();

}
