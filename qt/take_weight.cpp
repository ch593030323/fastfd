#include "take_weight.h"
#include <QSettings>
#include "public_sql.h"
#include "electroniccountprice.h"

take_weight::take_weight(fast_order_2 *parent) :
    QDialog(parent),
    ui(new Ui::take_weight),
    _parent(parent)
{
    ui->setupUi(this);
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    ui->lineEdit_type->setText(conf.value("system_setting/dianzichengxinghao").toString());
    if(ui->lineEdit_type->text().isEmpty())
        ui->lineEdit_type->setText(tr("没有类型"));

    countprice_scale=new   ElectronicCountPrice(0, "", "");
    countprice_scale->setType(conf.value("system_setting/dianzichengxinghao").toString());
    countprice_scale->setBaudrate(conf.value("system_setting/dianzicheng_baudrate").toString());
    connect(countprice_scale,SIGNAL(weighComming(QString)),ui->lineEdit_getweight,SLOT(setText(QString)));
    countprice_scale->tryStart_qs(conf.value("system_setting/dianzichengduankou").toString());

//    ui->lineEdit_dishname->setText(parent->get_currow_dishname());
//    ui->lineEdit_dishprice->setText(parent->get_currow_dishprice());
//    ui->lineEdit_getweight->setText(parent->get_currow_dishnum());

    ui->widget->setup(QStringList() <<tr("追加")<<tr("确认") << tr("取消"), Qt::AlignRight);
    ui->widget_2->setup(QStringList() <<  tr("置零") << tr("去皮"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("追加")), SIGNAL(clicked()),this,SLOT(toappend()));
    connect(ui->widget->index_widget(tr("确认")), SIGNAL(clicked()),this,SLOT(took()));
    connect(ui->widget->index_widget(tr("取消")), SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->widget_2->index_widget(tr("置零")), SIGNAL(clicked()),this,SLOT(tozhiling()));
    connect(ui->widget_2->index_widget(tr("去皮")), SIGNAL(clicked()),this,SLOT(toqupi()));
    connect(ui->lineEdit_getweight,SIGNAL(textChanged(QString)),this,SLOT(updatetotal(QString)));
}

take_weight::~take_weight()
{
    delete ui;
    delete countprice_scale;
}

void take_weight::toappend()
{
    //在原来的基础上追加
    _weight=QString::number(ui->lineEdit_getweight->text().toDouble()+_parent->get_currow_dishnum().toDouble());
    this->accept();
}

void take_weight::took()
{
    _weight=ui->lineEdit_getweight->text();
    this->accept();
}

void take_weight::toexit()
{
    this->reject();
}

void take_weight::toqupi()
{
    countprice_scale->qupi();
}

void take_weight::tozhiling()
{
    countprice_scale->zhiling();
}

void take_weight::updatetotal(const QString &text)
{
    ui->lineEdit_dishtotal->setText(QString().sprintf("%.02f", text.toFloat()*ui->lineEdit_dishprice->text().toFloat()));
}
