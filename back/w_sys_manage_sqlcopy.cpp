#include "w_sys_manage_sqlcopy.h"
//数据库备份
#include "ui_w_sys_manage_sqlcopy_dialog.h"
#include "w_sys_manage.h"
#include "n_func.h"
#include "public_sql.h"
#include <QDebug>
#include <QFileDialog>
#include <QTimer>
#include "lds_messagebox.h"
#include "ui_w_sys_manage_sqlcopy_logo_set_dialog.h"
#include "back_view_dialog1.h"

QStringList w_sys_manage_sqlcopy::static_tobackup_infos;

w_sys_manage_sqlcopy::w_sys_manage_sqlcopy(back_view_dialog1 *parent) :
    QDialog(parent),
    ui(new Ui_w_sys_manage_sqlcopy_Dialog),
    _parent(parent)
{
    ui->setupUi(this);

    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    ui->radioButton->setAutoExclusive(false);
    ui->radioButton_2->setAutoExclusive(false);
    ui->radioButton_3->setAutoExclusive(false);
    ui->radioButton->setChecked(conf.value("w_sys_manage_sqlcopy/radioButton").toBool());
    ui->radioButton_2->setChecked(conf.value("w_sys_manage_sqlcopy/radioButton_2").toBool());
    ui->radioButton_3->setChecked(conf.value("w_sys_manage_sqlcopy/radioButton_3").toBool());
    //! toolbar
    ui->widget->setup(QStringList()
                      << tr("备份策略")
                      << tr("数据库备份")
                      << tr("数据库还原")
                      << tr("备份文件删除")
                      <<"|"
                      << tr("数据库初始化")
                      << tr("初始化")
                      << tr("数据库中文演示")
                      <<tr("数据库英文演示")
                      <<tr("退出"), Qt::AlignTop);
    connect(ui->widget->index_widget(tr("备份策略")), SIGNAL(clicked()), this,SLOT(tobackup_strategy()));
    connect(ui->widget->index_widget(tr("数据库还原")), SIGNAL(clicked()), this,SLOT(torestore()));
    connect(ui->widget->index_widget(tr("数据库备份")), SIGNAL(clicked()), this,SLOT(tobackup()));
    connect(ui->widget->index_widget(tr("数据库初始化")), SIGNAL(clicked()), this,SLOT(tosqlinit()));
    connect(ui->widget->index_widget(tr("初始化")), SIGNAL(clicked()), this,SLOT(tosetinit()));
    connect(ui->widget->index_widget(tr("备份文件删除")), SIGNAL(clicked()), this,SLOT(filedelete()));
    connect(ui->widget->index_widget(tr("数据库中文演示")), SIGNAL(clicked()), this,SLOT(sqlchineseshow()));
    connect(ui->widget->index_widget(tr("数据库英文演示")), SIGNAL(clicked()), this,SLOT(sqlenglishshow()));

    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()), this,SLOT(toexit()));
    connect(ui->toolButton,SIGNAL(clicked()),this,SLOT(getfilepath()));
    connect(ui->toolButton_2,SIGNAL(clicked()),this,SLOT(getfilepath_2()));
    connect(ui->toolButton_3,SIGNAL(clicked()),this,SLOT(getfilepath_3()));

    connect(ui->radioButton,SIGNAL(toggled(bool)),this,SLOT(to_update_btn()));
    connect(ui->radioButton_2,SIGNAL(toggled(bool)),this,SLOT(to_update_btn()));
    connect(ui->radioButton_3,SIGNAL(toggled(bool)),this,SLOT(to_update_btn()));
    connect(ui->textEdit_amdin_set,SIGNAL(textChanged()),this,SLOT(to_admin_set()));
    //! after
    //还原路径
    ui->label->setText(conf.value("w_sys_manage_sqlcopy/sqlcurpath", public_sql::sql_backup_dir).toString());
    ui->label_2->setText(conf.value("w_sys_manage_sqlcopy/sqlcurpath_2", "/mnt/sdcard").toString());
    ui->label_3->setText(conf.value("w_sys_manage_sqlcopy/sqlcurpath_3", "/mnt/usb").toString());

    //获取路径下的所有文件
    ui->comboBox->clear();
    ui->comboBox->addItems(w_sys_manage::get_flilename_path(ui->label->text(), QStringList() << "*."+public_sql::sql_danwei));
    ui->comboBox_2->clear();
    ui->comboBox_2->addItems(w_sys_manage::get_flilename_path(ui->label_2->text(), QStringList() << "*."+public_sql::sql_danwei));
    ui->comboBox_3->clear();
    ui->comboBox_3->addItems(w_sys_manage::get_flilename_path(ui->label_3->text(), QStringList() << "*."+public_sql::sql_danwei));
    to_update_btn();

#ifdef Q_OS_WIN
    ui->lineEdit_winmysql_dir->setText(conf.value("w_sys_manage_sqlcopy/winmysql_dir",
                                                  "C:/Program Files (x86)/MySQL/MySQL Server 5.6/bin").toString());
    connect(ui->lineEdit_winmysql_dir,SIGNAL(editingFinished()),this,SLOT(to_winmysql_dir_save()));
#else
    ui->lineEdit_winmysql_dir->hide();
    ui->label_4->hide();
#endif
    //第一次使用是， 需要往conf写如新的数据
    conf.setValue("w_sys_manage_sqlcopy/sqlcurpath", ui->label->text());
    conf.setValue("w_sys_manage_sqlcopy/sqlcurpath_2", ui->label_2->text());
    conf.setValue("w_sys_manage_sqlcopy/sqlcurpath_3", ui->label_3->text());
    conf.sync();
    QTimer::singleShot(100, this, SLOT(check_backuppath()));
}

void w_sys_manage_sqlcopy::static_tobackup()
{
    static_tobackup_infos.clear();
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QList<QStringList> pathslist;
    if(conf.value("w_sys_manage_sqlcopy/radioButton").toBool()){
        pathslist.append(QStringList()
                         << conf.value("w_sys_manage_sqlcopy/sqlcurpath").toString()
                         << "normal"
                         );
    }
    if(conf.value("w_sys_manage_sqlcopy/radioButton_2").toBool()){
        pathslist.append(QStringList()
                         << conf.value("w_sys_manage_sqlcopy/sqlcurpath_2").toString()
                         << "sdcard"
                         );
    }
    if(conf.value("w_sys_manage_sqlcopy/radioButton_3").toBool()){
        pathslist.append(QStringList()
                         << conf.value("w_sys_manage_sqlcopy/sqlcurpath_3").toString()
                         << "usb"
                         );
    }

    foreach(QStringList paths, pathslist){
        //根据时间获取 文件名字
        QString name = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        QString path1=tr("sql-") + name+"."+public_sql::sql_danwei;
        //开始备份
        public_sql::MySQL_backup(paths[0] ,path1, paths.value(1));
        static_tobackup_infos.append(public_sql::MySQL_info);
    }
}

void w_sys_manage_sqlcopy::check_backuppath()
{
    //    if(QDir().exists(ui->label->text())){
    //        lds_messagebox::warning(this, tr("消息提示"),tr("磁盘路径未检测到"));
    //    }
    qDebug() << QDir("/mnt/sdcard").exists();
    if(!QDir(ui->label_2->text()).exists()){
        lds_messagebox::warning(this, tr("消息提示"),tr("SD卡路径未检测到，请插入SD卡"));
    }
    if(!QDir(ui->label_3->text()).exists()){
        lds_messagebox::warning(this, tr("消息提示"),tr("USB路径未检测到，请插入U盘"));
    }
}

void w_sys_manage_sqlcopy::tobackup_strategy()
{
    w_sys_manage_sqlcopy_backup_strategy dialog;
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_sys_manage_sqlcopy::torestore()
{
    if(0!=lds_messagebox::warning(this,tr("数据库还原"),tr("该操作将会还原数据库，是否继续?"),tr("继续"),tr("取消")))return;
    bool flag=public_sql::MySQL_restore(getlabeltext(), getcomtext());
    ui->textBrowser->append(public_sql::MySQL_info);
    if(flag){
        lds_messagebox::warning(this, tr("消息提示"),tr("数据库还原成功,点击重启"));
        public_sql::save_login->hideall_and_back_login();
    }
}

void w_sys_manage_sqlcopy::tobackup()
{
    if(0!=lds_messagebox::warning(this,tr("数据库备份"),tr("该操作将会在多个盘符备份，是否继续?"),tr("继续"),tr("取消")))return;
    static_tobackup();
    updatecomrtext();
    foreach(QString info, static_tobackup_infos)
        ui->textBrowser->append(info);
}

void w_sys_manage_sqlcopy::tosqlinit()
{
    if(0!=lds_messagebox::warning(this,tr("数据库初始化"),tr("是否继续?"),tr("继续"),tr("取消")))return;
    bool flag=public_sql::MySQL_restore(public_sql::init_sql_file_dir, public_sql::init_sql_file);
    ui->textBrowser->append(tr("初始化")+public_sql::MySQL_info);
    if(flag){
        lds_messagebox::warning(this, tr("消息提示"),tr("数据库还原成功,点击重启"));
        public_sql::save_login->hideall_and_back_login();
    }
}

void w_sys_manage_sqlcopy::tosetinit()
{
    if(0!=lds_messagebox::warning(this,tr("配置初始化"),tr("该操作将会删除所有已保存的配置信息，是否继续?"),tr("继续"),tr("取消")))return;

    bool flag=public_sql::fastfd_init_UNZIP();
    ui->textBrowser->append(tr("配置初始化")+public_sql::MySQL_info);
}

void w_sys_manage_sqlcopy::filedelete()
{
    if(0!=lds_messagebox::warning(this,tr("文件删除"),tr("该操作将会删除当前备份文件，是否继续?"),tr("继续"),tr("取消")))return;
    QString filename = getcomtext();

    if(filename.isEmpty()){

    } else {
        if(QFile(getlabeltext()+"/"+filename).remove()){
            ui->textBrowser->append(tr("文件删除成功!"));
            //获取路径下的所有文件
            updatecomrtext();
        } else {
            ui->textBrowser->append(tr("文件删除失败!"));
        }
    }
}

void w_sys_manage_sqlcopy::toexit()
{
    this->reject();
}

void w_sys_manage_sqlcopy::getfilepath()
{
    //获取路径
    QString dirname=QFileDialog::getExistingDirectory(this,
                                                      tr("Open File"),
                                                      ui->label->text().isEmpty()?"/uar/desk":ui->label->text(),
                                                      QFileDialog::ShowDirsOnly);
    if(!dirname.isEmpty()){
        ui->label->setText( dirname
                            );
        //获取路径下的所有文件
        updatecomrtext();

        //保存路径
        QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
        conf.setValue("w_sys_manage_sqlcopy/sqlcurpath", ui->label->text());
    }
}

void w_sys_manage_sqlcopy::getfilepath_2()
{
    //获取路径
    QString dirname=QFileDialog::getExistingDirectory(this,
                                                      tr("Open File"),
                                                      ui->label_2->text().isEmpty()?"/uar/desk":ui->label_2->text(),
                                                      QFileDialog::ShowDirsOnly);
    if(!dirname.isEmpty()){
        ui->label_2->setText( dirname
                              );
        //获取路径下的所有文件
        updatecomrtext();

        //保存路径
        QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
        conf.setValue("w_sys_manage_sqlcopy/sqlcurpath_2", ui->label_2->text());
    }
}

void w_sys_manage_sqlcopy::getfilepath_3()
{
    //获取路径
    QString dirname=QFileDialog::getExistingDirectory(this,
                                                      tr("Open File"),
                                                      ui->label_3->text().isEmpty()?"/uar/desk":ui->label_3->text(),
                                                      QFileDialog::ShowDirsOnly);
    if(!dirname.isEmpty()){
        ui->label_3->setText( dirname
                              );
        //获取路径下的所有文件
        updatecomrtext();

        //保存路径
        QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
        conf.setValue("w_sys_manage_sqlcopy/sqlcurpath_3", ui->label_3->text());
    }
}

void w_sys_manage_sqlcopy::sqlchineseshow()
{
    if(0!=lds_messagebox::warning(this,tr("中文演示数据"),tr("是否继续?"),tr("继续"),tr("取消")))return;
    bool flag=public_sql::MySQL_restore(public_sql::show_chinese_sql_file_dir, public_sql::show_chinese_sql_file);
    ui->textBrowser->append(tr("演示")+public_sql::MySQL_info);
    if(flag){
        lds_messagebox::warning(this, tr("消息提示"),tr("数据库还原成功,点击重启"));
        public_sql::save_login->hideall_and_back_login();
    }
}

void w_sys_manage_sqlcopy::sqlenglishshow()
{
    if(0!=lds_messagebox::warning(this,tr("英文演示数据"),tr("是否继续?"),tr("继续"),tr("取消")))return;
    bool flag=public_sql::MySQL_restore(public_sql::show_english_sql_file_dir, public_sql::show_english_sql_file);
    ui->textBrowser->append(tr("演示")+public_sql::MySQL_info);
    if(flag){
        lds_messagebox::warning(this, tr("消息提示"),tr("数据库还原成功,点击重启"));
        public_sql::save_login->hideall_and_back_login();
    }
}

void w_sys_manage_sqlcopy::to_update_btn()
{
    int checknum=ui->radioButton->isChecked()+ui->radioButton_2->isChecked()+ui->radioButton_3->isChecked();
    ui->widget->index_widget(tr("数据库备份"))->setEnabled(checknum>0);
    ui->widget->index_widget(tr("数据库还原"))->setEnabled(checknum==1);
    ui->widget->index_widget(tr("备份文件删除"))->setEnabled(checknum==1);

    //保存路径
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    conf.setValue("w_sys_manage_sqlcopy/radioButton", ui->radioButton->isChecked());
    conf.setValue("w_sys_manage_sqlcopy/radioButton_2", ui->radioButton_2->isChecked());
    conf.setValue("w_sys_manage_sqlcopy/radioButton_3", ui->radioButton_3->isChecked());
}

void w_sys_manage_sqlcopy::to_admin_set()
{
    if(n_func::gs_operid!="0000")return;

    QString key=ui->textEdit_amdin_set->toPlainText().trimmed();
    if(key=="80.90"){//更改logo 80.90
        w_sys_manage_sqlcopy_logo_set dialog(this);
        backheader::static_dialogexec(dialog , tr("后台logo设置"));
    }

    if(key=="70.80"){//软件使用限制 70.80
        w_sys_manage_sqlcopy_soft_limit dialog;
        backheader::static_dialogexec(dialog , tr("设置软件到期无法使用"));
    }
}

void w_sys_manage_sqlcopy::to_winmysql_dir_save()
{
#ifdef Q_OS_WIN
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    conf.setValue("w_sys_manage_sqlcopy/winmysql_dir",
                  ui->lineEdit_winmysql_dir->text());
#endif
}

QString w_sys_manage_sqlcopy::getcomtext()
{
    QString filename;
    if(ui->radioButton->isChecked()){
        filename=ui->comboBox->currentText();
    } else if(ui->radioButton_2->isChecked()){
        filename=ui->comboBox_2->currentText();

    } else if(ui->radioButton_3->isChecked()){
        filename=ui->comboBox_3->currentText();
    }

    return filename;
}

QString w_sys_manage_sqlcopy::getlabeltext()
{
    QString filename;
    if(ui->radioButton->isChecked()){
        filename=ui->label->text();
    } else if(ui->radioButton_2->isChecked()){
        filename=ui->label_2->text();

    } else if(ui->radioButton_3->isChecked()){
        filename=ui->label_3->text();
    }

    return filename;
}

void w_sys_manage_sqlcopy::updatecomrtext()
{
    if(ui->radioButton->isChecked()){//磁盘
        ui->comboBox->clear();
        ui->comboBox->addItems(w_sys_manage::get_flilename_path(ui->label->text(), QStringList() << "*."+public_sql::sql_danwei));

    }

    if(ui->radioButton_2->isChecked()){//sd
        ui->comboBox_2->clear();
        ui->comboBox_2->addItems(w_sys_manage::get_flilename_path(ui->label_2->text(), QStringList() << "*."+public_sql::sql_danwei));

    }

    if(ui->radioButton_3->isChecked()){//usb
        ui->comboBox_3->clear();
        ui->comboBox_3->addItems(w_sys_manage::get_flilename_path(ui->label_3->text(), QStringList() << "*."+public_sql::sql_danwei));
    }
}

//===========================
#include"ui_w_sys_manage_sqlcopy_backup_strategy_dialog.h"

w_sys_manage_sqlcopy_backup_strategy::w_sys_manage_sqlcopy_backup_strategy(QWidget *parent):
    QDialog(parent),
    ui(new Ui_w_sys_manage_sqlcopy_backup_strategy_Dialog)
{
    ui->setupUi(this);
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    ui->checkBox->setChecked(conf.value("w_sys_manage_sqlcopy_backup_strategy/sql_start_backup").toBool());

    ui->widget->setup(QStringList()  << tr("确定")<< tr("取消"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("确定")), SIGNAL(clicked()),this,SLOT(took()));
    connect(ui->widget->index_widget(tr("取消")), SIGNAL(clicked()),this,SLOT(toexit()));
}

void w_sys_manage_sqlcopy_backup_strategy::took()
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    conf.setValue("w_sys_manage_sqlcopy_backup_strategy/sql_start_backup", ui->checkBox->isChecked());;
    this->accept();
}

void w_sys_manage_sqlcopy_backup_strategy::toexit()
{
    this->reject();
}

//=========================
#include "ui_w_sys_manage_sqlcopy_soft_limit_dialog.h"
#include "lds_dialog_input.h"

w_sys_manage_sqlcopy_soft_limit::   w_sys_manage_sqlcopy_soft_limit(QWidget *parent):
    QDialog(parent),
    ui(new Ui_w_sys_manage_sqlcopy_soft_limit_Dialog)
{
    ui->setupUi(this);

    connect(ui->pushButton_beginset,SIGNAL(clicked()),this,SLOT(to_beginset()));
    connect(ui->pushButton_cancel,SIGNAL(clicked()),this,SLOT(to_quit()));
    connect(ui->pushButton_cancelset,SIGNAL(clicked()),this,SLOT(to_cancelset()));

    to_readfrom();
}

bool w_sys_manage_sqlcopy_soft_limit::static_soft_limit_check()
{
    //软件试用期
    //1第一次用的话，跳过，直接进入界面
    //2进入界面，【后台】【系统设置】【数据库备份】【右边的输入框 70.80】
    //3选择号结束日期，和密码(可以为空)；点击开始设置，下次启动就是实行该流程
    //4设置界面的左边，绿色表示启用，灰色表示停用
    //5进入设置界面必须是0000账户才可以
    w_sys_manage_sqlcopy_soft_limit limit_dialog;
    if(!limit_dialog.dt_end.isValid()) return true;//!若是第一次使用则，通过
    if(!limit_dialog.isrun) return true;//!没有启动

    //保证dt_begin只增不减
    if(limit_dialog.dt_begin < QDateTime::currentDateTime()){//1
        limit_dialog.dt_begin = QDateTime::currentDateTime();
        limit_dialog.to_save();
        limit_dialog.to_readfrom();
    }//!1

    lds_dialog_input inputdialog;
    inputdialog.ui->label->setText(QObject::tr("密钥:"));
    inputdialog.set_key_hidden(true);
    inputdialog.ui->lineEdit->setEchoMode(QLineEdit::Password);

    int limit_day=limit_dialog.dt_begin.daysTo(limit_dialog.dt_end);

    if(limit_day >10) return true;//!启动，过期时间超过10

    QString info;
    if(limit_day <= 10 && limit_day > 0){
        info=tr("软件试用期还剩%1天，请输入验证密钥").arg(limit_day);
    }
    if(limit_day  <= 0){
        info=tr("软件超过试用期，请输入验证密钥");
    }

    if(QDialog::Accepted==backheader::static_dialogexec(inputdialog, info)){//5
        if(inputdialog._text==limit_dialog.password){
            limit_dialog.isrun=false;//未启动
            limit_dialog.to_save();
            lds_messagebox::warning(0, tr("消息提示"),QObject::tr("验证成功"));
            return true;//!验证通过
        } else {
            lds_messagebox::warning(0, tr("消息提示"),QObject::tr("验证错误"));
            return false;//!验证错误
        }
    }
    if(limit_day <= 10 && limit_day > 0){
        lds_messagebox::warning(0, tr("消息提示"),QObject::tr("取消验证，但是在使用期限内"));
        return true;//!取消验证，但是在使用期限内
    }

    return false;//!取消验证，但是已过期
}

void w_sys_manage_sqlcopy_soft_limit::to_cancelset()//取消设置
{
    isrun=false;//停止
    to_save();
    to_readfrom();
}

void w_sys_manage_sqlcopy_soft_limit::to_beginset()
{
    dt_end=ui->dateTimeEdit->dateTime();
    password=ui->lineEdit_pwd->text();
    isrun=true;//开始启动
    to_save();
    to_readfrom();
}

void w_sys_manage_sqlcopy_soft_limit::to_quit()
{
    this->reject();
}

void w_sys_manage_sqlcopy_soft_limit::to_save()
{
    QByteArray byte;
    QDataStream in(&byte, QIODevice::WriteOnly);
    in << dt_begin;//开始时间指示会在开机是更新
    in << dt_end;
    in << isrun;
    in << password;

    qDebug() << dt_begin << dt_end << isrun << password;
    QFile file(public_sql::settings_txt_soft_limit_dir_file);
    if(file.open(QFile::WriteOnly)){
        file.write(byte);
        file.close();
    }
}

void w_sys_manage_sqlcopy_soft_limit::to_readfrom()
{
    QFile file(public_sql::settings_txt_soft_limit_dir_file);
    if(file.open(QFile::ReadOnly)){
        QByteArray byte;
        byte=file.readAll();
        QDataStream in(&byte, QIODevice::ReadOnly);
        if(!in.atEnd())
            in >> dt_begin;//开始时间指示会在开机是更新
        if(!in.atEnd())
            in >> dt_end;
        if(!in.atEnd())
            in >> isrun;
        if(!in.atEnd())
            in >> password;
        if(dt_end.isNull())
            isrun=false;
    }
    ui->dateTimeEdit->setDateTime(dt_end.isValid()?dt_end:QDateTime::currentDateTime());
    ui->lineEdit_pwd->setText(password);
    ui->label_led->setStyleSheet(isrun?
                                     "background:green":
                                     "background:red");
}

//===========================
w_sys_manage_sqlcopy_logo_set::w_sys_manage_sqlcopy_logo_set(w_sys_manage_sqlcopy *parent):
    QDialog(parent),
    ui(new Ui_w_sys_manage_sqlcopy_logo_set_Dialog),
    _parent(parent)
{
    ui->setupUi(this);
    ui->label_preview->setFixedSize(_parent->_parent->ui->label_2->size());
    //    this->setFixedWidth(ui->label_preview->width());
    ui->label_preview_2->setText(tr("预览(%1):")
                                 .arg(backheader::sys_content_title_size)
                                 );
    connect(ui->pushButton_quit,SIGNAL(clicked()),this,SLOT(to_quit()));
    connect(ui->pushButton_save,SIGNAL(clicked()),this,SLOT(to_save()));
    connect(ui->pushButton_selectpic,SIGNAL(clicked()),this,SLOT(to_selectpic()));
    connect(ui->pushButton_text_app,SIGNAL(clicked()),this,SLOT(to_app()));
    connect(ui->pushButton_default,SIGNAL(clicked()),this,SLOT(to_default()));
    connect(ui->pushButton_selectpic,SIGNAL(clicked()),this,SLOT(to_selectpic()));
    connect(ui->pushButton_selectpic_2,SIGNAL(clicked()),this,SLOT(to_selectpic_2()));
    connect(ui->pushButton_logo,SIGNAL(clicked()),this,SLOT(tologo()));


    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    ui->lineEdit_path->setText(conf.value("w_sys_manage_sqlcopy_logo_set/logo_path").toString());
    ui->lineEdit_path_2->setText(conf.value("w_sys_manage_sqlcopy_logo_set/logo_path_2").toString());
    ui->lineEdit_backtitle->setText(conf.value("w_sys_manage_sqlcopy_logo_set/logo_text").toString());
    ui->lineEdit_backtitle_2->setText(conf.value("w_sys_manage_sqlcopy_logo_set/logo_text_2").toString());
    ui->label_logopath->setText(conf.value("public_sql/login_logo").toString());

    to_text_app();
}

void w_sys_manage_sqlcopy_logo_set::to_default()
{
    ui->lineEdit_path->setText(_parent->_parent->def_logo_path);
    ui->lineEdit_path_2->setText(_parent->_parent->def_logo_path_2);
    ui->lineEdit_backtitle->setText(_parent->_parent->def_logo_text);
    ui->lineEdit_backtitle_2->setText(_parent->_parent->def_logo_text_2);
    ui->label_logopath->setText((public_sql::def_logo));

    to_text_app();
}

void w_sys_manage_sqlcopy_logo_set::to_quit()
{
    this->reject();
}

void w_sys_manage_sqlcopy_logo_set::to_save()
{
    to_app();
    this->accept();
}

void w_sys_manage_sqlcopy_logo_set::to_selectpic()
{
    //获取路径
    QFileInfo finfo(ui->lineEdit_path->text());
    QString dirname=QFileDialog::getOpenFileName(this,
                                                 tr("Open File"),
                                                 finfo.exists()?
                                                     finfo.path():"/usr/desk"
                                                     );
    if(!dirname.isEmpty()){
        ui->lineEdit_path->setText(dirname);
        to_text_app();
    }
}
void w_sys_manage_sqlcopy_logo_set::to_selectpic_2()
{
    //获取路径
    QFileInfo finfo(ui->lineEdit_path_2->text());
    QString dirname=QFileDialog::getOpenFileName(this,
                                                 tr("Open File"),
                                                 finfo.exists()?
                                                     finfo.path():"/usr/desk"
                                                     );
    if(!dirname.isEmpty()){
        ui->lineEdit_path_2->setText(dirname);
        to_text_app();
    }
}

void w_sys_manage_sqlcopy_logo_set::tologo()
{
    //获取路径
    QFileInfo finfo(ui->lineEdit_path_2->text());
    QString dirname=QFileDialog::getOpenFileName(this,
                                                 tr("Open File"),
                                                 finfo.exists()?
                                                     finfo.path():"/usr/desk"
                                                     );
    ui->label_logopath->setText(dirname);
    to_text_app();
}

void w_sys_manage_sqlcopy_logo_set::to_text_app()
{
    ui->pushButton_logo_view->setIcon(QPixmap(ui->label_logopath->text()));

    ui->label_preview->setPixmap(
                update_preview(_parent->_parent, ui->lineEdit_backtitle->text(), ui->lineEdit_path->text(), ui->lineEdit_path_2->text())
                );
}

void w_sys_manage_sqlcopy_logo_set::to_app()
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    conf.setValue("w_sys_manage_sqlcopy_logo_set/logo_path", ui->lineEdit_path->text());
    conf.setValue("w_sys_manage_sqlcopy_logo_set/logo_text", ui->lineEdit_backtitle->text());
    conf.setValue("w_sys_manage_sqlcopy_logo_set/logo_path_2", ui->lineEdit_path_2->text());
    conf.setValue("w_sys_manage_sqlcopy_logo_set/logo_text_2", ui->lineEdit_backtitle_2->text());
    conf.setValue("public_sql/login_logo", ui->label_logopath->text());

    _parent->_parent->update_title_pic();
}

QPixmap w_sys_manage_sqlcopy_logo_set::update_preview(back_view_2 *parent, const QString &text, const QString &path, const QString &path_2)
{
    QPixmap p(parent->ui->label_2->size());
    p.fill(Qt::transparent);
    QPainter painter(&p);
    QPixmap logo(path);    QPixmap logo_2(path_2);
    logo=logo.scaled(p.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    logo_2=logo_2.scaled(p.size().width(), p.size().height()*2.0/3.0, Qt::KeepAspectRatio);
    if(!logo.isNull())painter.drawPixmap(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignLeft|Qt::AlignTop, logo.size(), QRect(QPoint(0,0),p.size())), logo);
    if(!logo_2.isNull())painter.drawPixmap(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignRight|Qt::AlignTop, logo_2.size(), QRect(QPoint(0,0),p.size())), logo_2);

    QFont font=qApp->font();
    font.setPixelSize(40);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(Qt::black);
    painter.drawText(QRect(2,2, p.width(), p.height()), int(Qt::AlignCenter|Qt::TextWordWrap),
                     text);
    painter.setPen(Qt::white);
    painter.drawText(QRect(0,0, p.width(), p.height()), int(Qt::AlignCenter|Qt::TextWordWrap),
                     text);
    return (logo.isNull()&&logo_2.isNull()&&text.isEmpty())?QPixmap():p;
}
