#include "back_view_2.h"
#include <QPainter>
#include "backheader.h"
#include <QSettings>
#include "w_sys_manage_sqlcopy.h"

back_view_2::back_view_2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::back_view_2)
{
    ui->setupUi(this);
    ui->label_2->raise();
    foreach(QToolButton *t, back_view_2_botton::toollist){
        connect(t,SIGNAL(clicked()),this,SLOT(to_center_click()));
    }

    QSize tool_icon_size;
    QFont tool_font;
    tool_font.setBold(true);
    def_logo_path=(":/image/widgets_pic/logo.png");def_logo_path_2=(":/image/back_title_right.png");
    if(backheader::mainwindowsize.width()==800){
        tool_icon_size=QSize(56,46);
        tool_font.setPixelSize(15);
        ui->frame->setMinimumWidth(150);
    } else {
        tool_icon_size=QSize(74,58);
        tool_font.setPixelSize(22);
        ui->frame->setMinimumWidth(200);
    }
    qDebug() << "def_logo_path" << def_logo_path;
    def_logo_text=tr("摇钱树餐饮软件");
    def_logo_text_2="Nanjing Yaoqianshu Software Co., Ltd. www.yqspos.com";
    delay_update_title_pic();
    foreach(QToolButton *t, back_view_2_botton::toollist){
        t->setFont(tool_font);
        t->setIconSize(tool_icon_size);
    }

    foreach(QRadioButton *t, back_view_2_radio::rbtnlist){
        t->setFont(tool_font);
    }
}

back_view_2::~back_view_2()
{
    delete ui;
}

void back_view_2::update_title_pic()
{
    ui->label_2->setStyleSheet("");
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QString logo_text_conf=conf.value("w_sys_manage_sqlcopy_logo_set/logo_text").toString();
    QString logo_path_conf=conf.value("w_sys_manage_sqlcopy_logo_set/logo_path").toString();
    QString logo_path_conf_2=conf.value("w_sys_manage_sqlcopy_logo_set/logo_path_2").toString();
    QString logo_text_conf_2=conf.value("w_sys_manage_sqlcopy_logo_set/logo_text_2").toString();

    if(logo_text_conf.isEmpty())logo_text_conf=def_logo_text;
    if(logo_path_conf.isEmpty())logo_path_conf=def_logo_path;
    if(logo_path_conf_2.isEmpty())logo_path_conf_2=def_logo_path_2;

    ui->label_2->setPixmap(w_sys_manage_sqlcopy_logo_set::update_preview(
                               this,
                               logo_text_conf,
                               logo_path_conf,
                               logo_path_conf_2
                               ));
    if(!logo_text_conf_2.isEmpty())ui->label->setText(logo_text_conf_2);
}

void back_view_2::delay_update_title_pic()
{
    QTimer::singleShot(100, this,SLOT(update_title_pic()));
}

void back_view_2::paintEvent(QPaintEvent *e)
{
    QDialog::paintEvent(e);
}

void back_view_2::on_radioButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void back_view_2::on_radioButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void back_view_2::on_radioButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void back_view_2::on_radioButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void back_view_2::on_radioButton_8_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void back_view_2::on_radioButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void back_view_2::on_radioButton_6_clicked()
{
    emit signal_index(7,0);//退出
}

void back_view_2::to_center_click()
{
    QToolButton *t=qobject_cast<QToolButton *>(this->sender());
    if(t){
        emit signal_index(ui->stackedWidget->currentIndex()+1,t->property("id").toInt());
    }
}
