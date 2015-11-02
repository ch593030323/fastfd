
#include "public_logindialog.h"
#include "ui_public_logindialog.h"
#include "public_sql.h"
//#include "fast_order_2.h"
#include "fast_order_2.h"
#include <QDebug>
#include "n_func.h"
#include "backheader.h"
#include "w_m_member.h"
#include <QSqlQuery>
#include <QTimer>
#include "trans_u2g.h"
#include <QSettings>
#include "w_scr_dish_restaurant_dialog.h"

#include "w_i_inventory.h"
#include "w_sys_manage.h"
#include "lds_messagebox.h"
#include "ui_backviewdialog3_dialog.h"
#include "backheader.h"
#include <QInputDialog>
#include <QDir>
#include "lds_dialog_input.h"
#include "back_view_dialog1.h"
#include "lds_loadingdialog.h"
#include "w_sys_manage_sqlcopy.h"
#include "public_heartbeatdialog.h"
#include <QSqlError>
#include "w_bt_dish_kitchenprinter.h"
#include "w_sys_manage_cloudsync_basedatadown.h"
#include "clientdisplay.h"
#include "fast_order_2.h"
#include "w_sys_manage_cloudsync_basedatadown.h"
#include "w_sys_manage_cloudsync_flowdataup.h"
#include "fontviewdialog_jiulou_dialog.h"
#include "public_debugdialog.h"

public_loginDialog::public_loginDialog(QWidget *parent, const QString &u, const QString &p) :
    lds_dialog_drog(parent),
    ui(new Ui_backviewdialog3_Dialog),
    info_sqr(0),
    dialog_back(0),
    dialog_pre(0),
    dialog_pre_2(0),
    double_screen(0),
    sql_isopen(false)
{
    ui->setupUi(this);
    public_sql::save_login=this;
    ui->lineEdit_pwd->setEchoMode(QLineEdit::Password);
#ifdef QT_DEBUG
    ui->lineEdit_usr->setText("0000");
    ui->lineEdit_pwd->setText(n_func::get_godpassword());
    if(public_sql::dbHostName.isEmpty()){
        public_sql::dbHostName="192.168.1.37";
        public_sql::dbDatabaseName="hddpos";
        public_sql::dbUserName="root";
        public_sql::dbPassword="111111";
        public_sql::MySQL_PARAM_SAVE();
    }
#endif
    if(!p.isEmpty()){
        ui->lineEdit_pwd->setText(p);
    }
    if(!u.isEmpty()){
        ui->lineEdit_usr->setText(u);
        //        ui->lineEdit_usr->setFocusPolicy(Qt::NoFocus);
    }
    sql_heart_beat.setInterval(60*60*1000);
    //信号连接
    connect(&sql_heart_beat,SIGNAL(timeout()),this,SLOT(to_sql_heart_beat()));
    connect(ui->pushButton_close,SIGNAL(clicked()),this,SLOT(goto_close()));
    connect(ui->pushButton_pre,SIGNAL(clicked()),this,SLOT(togoto_pre()));
    connect(ui->pushButton_back,SIGNAL(clicked()),this,SLOT(togoto_back()));
    connect(ui->pushButton_num0,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_num1,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_num2,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_num3,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_num4,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_num5,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_num6,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_num7,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_num8,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_num9,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_dot,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_clear,SIGNAL(clicked()),this,SLOT(set_num()));
    connect(ui->pushButton_dbset,SIGNAL(clicked()),this,SLOT(todbset()));

    //! after
    ui->lineEdit_usr->setFocus();

    connect(this,SIGNAL(signal_everytime_to_print_failed_kitchen_bill()),this,SLOT(to_everytime_to_print_failed_kitchen_bill()));
    ui->pushButton->setIcon(QPixmap(public_sql::get_logo_path()));
    QTimer::singleShot(200, this, SLOT(init_dbconnect()));

    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    if(conf.value("system_setting/fupingxianshi", false).toBool()){
        double_screen=new fontviewDialog_doublescreen;
        double_screen->resize(backheader::fullwindowsize);
        backheader::static_dialogshow_double(*double_screen,tr("副屏"));
    }
}

public_loginDialog::~public_loginDialog()
{
    delete ui;
    if(dialog_pre)delete dialog_pre;
    if(dialog_pre_2)delete dialog_pre_2;
    if(dialog_back)delete dialog_back;
    if(double_screen) delete double_screen;
}

void public_loginDialog::setversion(const QString &text)
{
    //软件版本号
    qWarning() << text;
    n_func::gs_soft_version=text;//每次升级直接修改
    ui->label_version->setText(n_func::gs_soft_version);
}

void public_loginDialog::set_num()
{
    QPushButton *btn=qobject_cast<QPushButton *>(sender());
    if(!btn)return;

    QLineEdit *line=qobject_cast<QLineEdit *>(this->focusWidget());
    if(!line)return;

    if(btn==ui->pushButton_clear){
        line->clear();
    } else {
        line->insert(btn->text());
    }
}

void public_loginDialog::goto_back()
{
    if(dialog_pre&&dialog_pre->isVisible()) dialog_pre->hide();
    if(dialog_pre_2&&dialog_pre_2->isVisible()) dialog_pre_2->hide();
    if(this->isVisible())this->hide();
    if(dialog_back==0){
        dialog_back=new back_view_dialog1(this);
        static_cast<back_view_dialog1 *>(dialog_back)->update_title_pic();
        connect(dialog_back,SIGNAL(rejected()),this,SLOT(goto_close()));
    }
    backheader::static_dialogshow(*dialog_back, tr("后台"), backheader::fullwindowsize);
}

void public_loginDialog::goto_pre()
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    if(dialog_back&&dialog_back->isVisible()) dialog_back->hide();
    if(this->isVisible())this->hide();
    QString str = conf.value("system_setting/yingyemoshi").toString();
    if(str == "0"  || str.isEmpty())
    {
        //            fast_order_2 b;
        if(dialog_pre==0){
            dialog_pre=new fast_order_2(this);
            connect(dialog_pre,SIGNAL(rejected()),this,SLOT(goto_close()));
        }
        dialog_pre->torefresh();
        backheader::static_dialogshow(*dialog_pre, tr("前台"), backheader::fullwindowsize);
    }
    else
    {
        if(dialog_pre_2==0){
            dialog_pre_2=new w_scr_dish_restaurant_dialog(this);
            connect(dialog_pre_2,SIGNAL(rejected()),this,SLOT(goto_close()));
        }
        dialog_pre_2->torefresh();
        backheader::static_dialogshow(*dialog_pre_2, tr("前台"), backheader::fullwindowsize);
    }
}

bool public_loginDialog::pre_is_exist()
{
    return dialog_pre
            ||dialog_pre_2;
}

void public_loginDialog::togoto_back()
{
    QString errlogin=totrylogin();
    if(errlogin.isEmpty()){
        goto_back();
    }else {
        updateinfo(errlogin
                   );
    }
}

void public_loginDialog::togoto_pre()
{
    QString errlogin=totrylogin();
    if(errlogin.isEmpty()){
        goto_pre();
    } else {
        updateinfo(errlogin
                   );
    }
}

void public_loginDialog::updateinfo(const QString &errinfo)
{
    if(errinfo.isEmpty()) return;
    ui->label_info->setText(QString::number(info_sqr++)+errinfo);
}

void public_loginDialog::to_init_dbconnect(lds_loadingDialog *loading)
{
    QString sql_error;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    //连接数据库
    loading->ui->label_text->setText(tr("正在连接数据库..."));
    sql_isopen=public_sql::CONNECT_MySQL();
    updateinfo(public_sql::MySQL_info
               );

    while(sql_isopen){
        loading->ui->label_text->setText(tr("正在初始化..."));
        //升级数据库

        QSqlQuery query;
        query.exec("select * from cey_sys_sqlversion order by sqlversion desc");//从大到小
        query.next();
        sql_error=public_sql::MySQL_sqlversion_update(query.record().value("sqlversion").toString());
        if(sql_error.isEmpty()){

        } else {
            lds_messagebox::warning(this, tr("消息提示"),tr("数据库升级:")+sql_error);
        }
        //! 升级数据库

        //心跳
        sql_heart_beat.start();
        //!心跳

        //!获取数据库的打印机的ip
        //厨打单内容的生成
        w_bt_dish_Kitchenprinter::k_data_content_mode_init();
        //!厨打单内容的生成

        //关于会员的初始话数据
        w_m_member::VIP_MODEL_INIT();
        //!关于会员的初始话数据

        // 上传本单数据采集example
        //! 上传本单数据采集example
        //正在连接云端
        if(conf.value("w_sys_manage_cloudsync_basedataset/yun_start",false).toBool()){
            loading->ui->label_text->setText(tr("正在连接云端..."));
            //软件判断时间是否和云端一致
            if(w_sys_manage_cloudsync::is_match_local_time)break;//已经匹配成功过,则所有的云端配置都不起作用
            if(!w_sys_manage_cloudsync::match_local_time())break;//匹配失败的,则所有的云端配置都不起作用
            //!软件判断时间是否和云端一致

            //开机首次上传前一天流水
            if(conf.value("w_sys_manage_cloudsync_basedataset/first_boot_upload_data", false).toBool()){
                QDate last_update_date=conf.value("w_sys_manage_cloudsync_basedataset/last_upload_date").toDate();
                QDate before_date=QDate::currentDate().addDays(-1);
                qDebug() << "last_update_date" <<last_update_date <<"before_date" << before_date;
                if(last_update_date.isNull()
                        || last_update_date<before_date){
                    w_sys_manage_cloudsync_flowdataup dialog;
                    dialog.ui->dateTimeEdit_begin->setDateTime(QDateTime(before_date));
                    dialog.ui->dateTimeEdit_end->setDateTime(QDateTime(before_date));
                    QTimer::singleShot(500, &dialog, SLOT(on_pushButton_up_clicked()));
                    backheader::static_dialogexec(dialog,"上传流水数据");

                    //更新最后上传日期
                    conf.setValue("w_sys_manage_cloudsync_basedataset/last_upload_date",QDate::currentDate().addDays(-1).toString("yyyy-MM-dd"));
                }
            }

            //!开机首次上传前一天流水

            //开机下载基础数据
            if(conf.value("w_sys_manage_cloudsync_basedataset/first_boot_download_data", false).toBool()){
                QDate last_update_date=conf.value("w_sys_manage_cloudsync_basedataset/last_download_date").toDate();
                QDate before_date=QDate::currentDate().addDays(-1);
                if(last_update_date.isNull()
                        || last_update_date<before_date){
                    w_sys_manage_cloudsync_basedatadown dialog;
                    QTimer::singleShot(500, &dialog, SLOT(on_pushButton_down_clicked()));
                    backheader::static_dialogexec(dialog,tr("下载基础数据"));

                    //更新最后上传日期
                    conf.setValue("w_sys_manage_cloudsync_basedataset/last_download_date",QDate::currentDate().addDays(-1).toString("yyyy-MM-dd"));
                    lds_messagebox::warning(0, tr("消息提示"),tr("下载基础数据"), true);
                }
            }
            //! 开机下载基础数据

            //微信轮询读取云端数据
            public_sql::YUN_POLLING();
            //!微信轮询读取云端数据
        }
        //! 正在连接云端
        break;
    }
    /// while(sql_isopen)
}

void public_loginDialog::showprinterr(const QString &errstring, const QString &title)
{
    public_printerr_Dialog *dialog=new public_printerr_Dialog;
    dialog->raise();
    dialog->setContent(errstring, title);
    QTimer::singleShot(5000, dialog, SLOT(close()));
    backheader::static_dialogshow(*dialog, tr("打印机错误"),QSize(), Qt::AlignRight|Qt::AlignTop);
}

void public_loginDialog::showmemoinfo()
{
    public_debugDialog *dialog=new public_debugDialog(getcur_validparent());
    backheader::static_dialogshow(*dialog, tr("打印机错误"),QSize(), Qt::AlignLeft|Qt::AlignTop);
}

QWidget *public_loginDialog::getcur_validparent()
{
    if(dialog_back&&dialog_back->isVisible()) {
        return dialog_back;
    } else if(dialog_pre&&dialog_pre->isVisible()){
        return dialog_pre;
    } else if(dialog_pre_2&&dialog_pre_2->isVisible()){
        return dialog_pre_2;
    }
    return this;
}

void public_loginDialog::to_sql_heart_beat()
{
    qDebug() << "to_sql_heart_beat";
    QSqlQuery query;
    query.exec("select now()");
    if(query.next()){
        sql_heart_beat.start();
    } else {
        sql_heart_beat.stop();
        public_heartbeatDialog dialog;
        backheader::static_dialogexec(dialog, "");
        sql_heart_beat.start();
    }
}

void public_loginDialog::to_everytime_to_print_failed_kitchen_bill(const QString &int_id, const QString &vch_ip)//开始下载第一条漏打单
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    if(!conf.value("kitchen_leakage/is_auto_print", false).toBool()){
        return;
    }
    w_bt_dish_Kitchenprinter::one_kdata_stragety_print(QList<BINS_IP>());
}

void public_loginDialog::goto_close()
{
    qDebug() << "public_loginDialog::goto_close()";
    if(double_screen){
        double_screen->close();
    }
    //开始关闭
    this->reject();
}

void public_loginDialog::todbset()
{
    lds_dialog_input inputdialog;
    inputdialog.ui->label->setText(tr("密钥(***7):"));
    inputdialog.ui->lineEdit->setEchoMode(QLineEdit::Password);
#ifdef  QT_DEBUG
    inputdialog.ui->lineEdit->setText("7777");
#endif
    if(QDialog::Accepted==backheader::static_dialogexec(inputdialog, tr("请输入密钥"))){
        if(inputdialog._text=="7777"){
            public_sqlSet dialog(this);
            if(QDialog::Accepted==backheader::static_dialogexec(dialog, tr("数据库设置"))){
                init_dbconnect();
            }
        }
    }
}

void public_loginDialog::hideall_and_back_login(const QString &p, const QString &u)
{
    QString _p=p;
    QString _u=u;

    if(p.isEmpty() && u.isEmpty()){
        _u=n_func::gs_operid;
        _p=n_func::gs_password;
    }

    goto_close();
    qApp->closeAllWindows();
    QProcess::startDetached(qApp->applicationFilePath(), QStringList()<<("-p"+_p)<<("-u"+_u));
}

void public_loginDialog::hideall_and_back_login()
{
    hideall_and_back_login(n_func::gs_operid, n_func::gs_password);
}

void public_loginDialog::init_dbconnect()
{
    QEventLoop eloop;
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    //开机备份
    if(conf.value("w_sys_manage_sqlcopy_backup_strategy/sql_start_backup").toBool()){
        w_sys_manage_sqlcopy::static_tobackup();
    }
    //!开机备份
    updateinfo(tr("正在连接数据库..."));
    //进度条
    lds_loadingDialog loading(this);
    loading.setModal(true);
    backheader::static_dialogshow(loading, tr("LOADING..."));
    //开始连接数据库
    QTimer::singleShot(100,&eloop,SLOT(quit()));
    eloop.exec(QEventLoop::ExcludeUserInputEvents);
    to_init_dbconnect(&loading);
    //连接完成后,选中焦点
    ui->lineEdit_usr->setFocus();
}

QString public_loginDialog::totrylogin()
{
    //ue_ok (none) returns (none)
    QString ls_operid;
    QString ls_opername;
    QString ls_password;
    QString ls_name;
    QString ls_login_type;
    qlonglong ll_exist;
    QSqlQuery query;
    if(sql_isopen){
    } else {
        return public_sql::MySQL_info;
    }
    //Has been Shielded.
    ls_operid = ui->lineEdit_usr->text().trimmed();
    ls_password = ui->lineEdit_pwd->text().trimmed();
    if(ls_operid.isEmpty()){
        return QString("用户名不能为空！");
    }
    //    if(ls_password.isEmpty()){
    //        return QString("密码不能为空！");
    //    }

    if(ls_operid=="0000"){
        if(!n_func::f_cmp_syspwd(ls_password)){//ls_password!=n_func::f_get_sysparm("system_pass", "0000")){
            return QString("操作员密码错误，请核实！");
        }
        ls_opername="system";

    } else {
        query.prepare(QString("Select Count(1)  from cey_sys_operator  where vch_operID =:ls_operid and ifnull(vch_password , '') =:ls_password and ch_flag ='Y'"));
        query.bindValue(":ls_operid",  ls_operid);
        query.bindValue(":ls_password",  ls_password);
        query.exec();
        query.next();
        if(query.record().value(0).toLongLong() == 0){
            return QString("操作员密码错误，请核实！");
        }

        query.prepare(QString(" select vch_operator   from cey_sys_operator  where vch_operID =:ls_operid"));
        query.bindValue(":ls_operid",  ls_operid);
        query.exec();
        query.next();
        ls_opername=query.record().value(0).toString();
    }

    n_func::gs_operid=ls_operid;
    n_func::gs_opername=ls_opername;
    n_func::gs_password=ls_password;
    kitchen_timer.start(10000, this);
    return "";
}


//U正数 F负数   S零
//0102532d30302e3030326b67  6e 030400
//0102  S   -  0  0   . 0   0   2  k   g
void public_loginDialog::eveyread()
{
    qDebug() << 3;
    QString ret="0.0000";
    printer->writedirect("P");
    const QByteArray b0=printer->readAll();
    int index0, index1;
    char head1,head2,head3,flag1,sign,checksum,tail1,tail2,tail3,flag2;
    QByteArray weight,unit;
    head1=0x02;//头
    head2=0x20;//称重数据定位
    head3=0x20;//称重数据定位
    tail1=0x20;
    tail2=0x4b;
    tail3=0x47;

    //    qDebug() << b0;
    //    qDebug() << b0.toHex();
    if((index0=b0.indexOf(QByteArray()+head1+head2+head3))>-1){//0x0102固定为0x0102 开头
        if((index1=b0.indexOf(QByteArray()+tail1+tail2+tail3))>-1){//0x0304固定为0x0304 结束
            QByteArray inter =b0.mid(index0, index1-index0);
            inter=inter.mid(3);
            qDebug() << inter.toHex();
            if(inter.count()==5){
                ret=inter;
            }
            qDebug() << ret;
        }
    }
}

void public_loginDialog::timerEvent(QTimerEvent *e)
{
    //打印漏打单
    if(e->timerId() == kitchen_timer.timerId()){
        emit signal_everytime_to_print_failed_kitchen_bill();
    }
    QDialog::timerEvent(e);
}
