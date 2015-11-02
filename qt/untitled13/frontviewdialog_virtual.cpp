#include "frontviewdialog_virtual.h"
#include <QtDebug>
#include <QItemSelectionModel>
#include "public_sql.h"
#include "clientdisplay.h"
#include <QtDebug>

QStandardItemModel frontViewDialog_virtual::modelBase;
QItemSelectionModel frontViewDialog_virtual::modelBase_select(&frontViewDialog_virtual::modelBase);
bool frontViewDialog_virtual::modelbase_headerlabel_isset=false;
QSqlQueryModel frontViewDialog_virtual::model_dalei;
QSqlQueryModel frontViewDialog_virtual::model_xiaolei;
QSqlQueryModel frontViewDialog_virtual::model_cook;
bool frontViewDialog_virtual::bianma_daicai_type;
QString frontViewDialog_virtual::dalei;
QString frontViewDialog_virtual::xiaolei;
int frontViewDialog_virtual::num_da;
int frontViewDialog_virtual::num_xiao;
int frontViewDialog_virtual::num_cai;
int frontViewDialog_virtual::row_n;
bool frontViewDialog_virtual::changyongcook;
QString frontViewDialog_virtual::diancaitime;
QStringList frontViewDialog_virtual::first_in1;
float frontViewDialog_virtual::_num=0;
float frontViewDialog_virtual::_total=0;
QString frontViewDialog_virtual::cur_desk_no;//1
QDateTime frontViewDialog_virtual::cook_time;//1
QString frontViewDialog_virtual::k_fuwuyuan;//1
QString frontViewDialog_virtual::k_beizhu;//1
QString frontViewDialog_virtual::k_renshu;//1
frontViewDialog_virtual::frontViewDialog_virtual(QWidget *parent) :
    QDialog(parent)
{
    display_led=new ClientDisplay("","");
    if(!modelbase_headerlabel_isset){
        modelBase.setHorizontalHeaderLabels(
                    QStringList() << tr("序号") << tr("编码") << tr("品名") << tr("数量") << tr("单价") //4
                    << tr("折扣值") << tr("总价") << tr("特殊做法") << tr("是否赠送") << tr("是否套菜") //9
                    << tr("是否特价") << tr("是否优惠") << tr("单位") << tr("赠送") << tr("定折人") //14
                    << tr("是否时价菜") << tr("子套菜所在主套菜编码") << tr("税点") << tr("是否折扣") << tr("折扣时间") //19
                    << tr("赠送时间") << tr("做法加价") << tr("所属大类编码") << tr("所属小类编码"));
        modelbase_headerlabel_isset=true;
    }
    //1
    //2
    connect(this, SIGNAL(signal_display_price(QString)),this,SLOT(display_price(QString)));
    connect(this, SIGNAL(signal_display_total(QString)),this,SLOT(display_total(QString)));
    connect(this, SIGNAL(signal_display_cash(QString)),this,SLOT(display_cash(QString)));
    connect(this, SIGNAL(signal_display_change(QString)),this,SLOT(display_change(QString)));
    connect(this, SIGNAL(signal_display_clear(QString)),this,SLOT(display_clear(QString)));

    //3
    QTimer::singleShot(1,this,SLOT(set_num_total_to_doublescreen()));
    connect(&timer,SIGNAL(timeout()),this,SLOT(toprintsize()));
    timer.start(1000);
}

frontViewDialog_virtual::~frontViewDialog_virtual()
{
    if(display_led)delete display_led;
}

void frontViewDialog_virtual::toprintsize()
{
    qDebug() << this->size();
}

void frontViewDialog_virtual::display_price(const QString &value)
{
    display(1, value);
}
void frontViewDialog_virtual::display_total(const QString &value)
{
    display(2, value);
}

void frontViewDialog_virtual::display_cash(const QString &value)
{
    display(3, value);
}

void frontViewDialog_virtual::display_change(const QString &value)
{
    display(4, value);
}
void frontViewDialog_virtual::display_clear(const QString &value)
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);

    if(display_led->_type!=conf.value("system_setting/kexianleixing").toString()){
        display_led->_type=conf.value("system_setting/kexianleixing").toString();
    }
    display_led->startShowinfo(conf.value("system_setting/kexian_vfd_start").toString(),
                               conf.value("system_setting/kexianduankou").toString());
}
void frontViewDialog_virtual::display(int sqr, const QString &value)
{
    qDebug() << "display" << sqr << value;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    if(display_led->_type!=conf.value("system_setting/kexianleixing").toString()){
        display_led->_type=conf.value("system_setting/kexianleixing").toString();
    }
    display_led->writeCom(sqr, value,
                          conf.value("system_setting/kexianduankou").toString());
}

void frontViewDialog_virtual::addGood(int row)
{
    if(row>=0&&row<modelBase.rowCount())
        emit_signal_display_price(QString().sprintf("%.02f",modelBase.item(row, 4)->text().toFloat()));
}

void frontViewDialog_virtual::toDel()
{
    emit_signal_display_cash(QString().sprintf("%.02f",_total));
}

void frontViewDialog_virtual::toPay()
{
}
