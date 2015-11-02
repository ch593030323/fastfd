#include "backheader.h"
#include <QSqlQueryModel>
#include <qsqlrecord.h>
#include <QSettings>
#include <QApplication>
#include <QSqlQuery>
#include <QDesktopWidget>
#include "trans_u2g.h"
#include "n_func.h"
#include "lds_messagebox.h"

QSize backheader::mainwindowsize;//=QSize(1500,768);
QSize backheader::fullwindowsize;//前台,后台,中餐点菜,中餐前台
public_mainprinter *backheader::printer=new public_mainprinter;
QStringList backheader::vch_operid_name;
QStringList backheader::vch_dish_typename;
QStringList backheader::vch_unitname;
QString backheader::sys_language;
QString backheader::sys_content_size;
QString backheader::sys_content_title_size;
int backheader::sys_desk_bottom_height;

backheader::backheader(QObject *parent) : QObject(parent)
{
}

void backheader::fitlter_somewindowflag(QWidget *dialog)
{
    qDebug() << "windowTitle"<<dialog->windowTitle();
    if(dialog->windowTitle()==tr("更改图片")
            ||dialog->windowTitle()==tr("基础数据导出")
            ||dialog->windowTitle()==tr("基础数据导入")
            ||dialog->windowTitle()==tr("下载基础数据")
            ||dialog->windowTitle()==tr("上传基础数据")
            ||dialog->windowTitle()==tr("前台锁屏")
            ||dialog->windowTitle()==tr("上传流水数据")
            ||dialog->windowTitle()==tr("消息提示")
            ||dialog->windowTitle()==tr("业务数据库清除")
            ||dialog->windowTitle()==tr("快餐付款")
            ){
        dialog->setWindowFlags(Qt::Dialog|Qt::WindowTitleHint|Qt::CustomizeWindowHint);
    } else if(dialog->windowTitle()==tr("用户登录")
              ||dialog->windowTitle()==tr("LOADING...")
              ){
        dialog->setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog);
    } else if(dialog->windowTitle()==tr("打印机错误")){
        dialog->setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);
    } else {
        dialog->setWindowFlags(Qt::Dialog|Qt::WindowTitleHint);
    }
}

QRect backheader::static_calculat_center_rect(QWidget *dialog, const QSize &size, int flag)
{
#if (defined QT_DEBUG) || (defined Q_OS_WIN)
    QPoint full_left_top(100,100);
    QPoint window_left_top(100,100);
#else
    QPoint full_left_top(0,0);
    QPoint window_left_top(0,30);
#endif
    bool isfull=dialog->windowTitle()==tr("前台")
            ||dialog->windowTitle()==tr("后台")
            ||dialog->windowTitle()==tr("副屏")
            ||dialog->windowTitle()==tr("中餐点菜")
            ||dialog->windowTitle()==tr("打印机错误")
            ;
    QRect actual_rect;
    qDebug() << isfull << "size:"<<size;
    if(size.isValid()){
        actual_rect = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignmentFlag(flag), size, isfull?
                                              QRect(full_left_top, backheader::fullwindowsize):
                                              QRect(window_left_top, backheader::mainwindowsize)
                                              );
    } else {
        if((backheader::mainwindowsize - dialog->size()).isValid()){
            actual_rect = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignmentFlag(flag), dialog->size(), isfull?
                                                  QRect(full_left_top, backheader::fullwindowsize):
                                                  QRect(window_left_top, backheader::mainwindowsize)
                                                  );
        } else {
            actual_rect = isfull?
                        QRect(full_left_top, backheader::fullwindowsize):
                        QRect(window_left_top, backheader::mainwindowsize);
        }
    }
    qDebug() << isfull << "actual_rect"<<actual_rect;
    return actual_rect;
}

int backheader::static_dialogexec(QDialog &dialog, const QString &title, const QSize &size, int flag)
{
    if(!title.isEmpty())dialog.setWindowTitle(title);
    fitlter_somewindowflag(&dialog);
    dialog.setGeometry(static_calculat_center_rect(&dialog, size,flag));

    return dialog.exec();
}

void backheader::static_dialogshow(QDialog &dialog, const QString &title, const QSize &size, int flag)
{
    if(!title.isEmpty())dialog.setWindowTitle(title);
    fitlter_somewindowflag(&dialog);
    dialog.setGeometry(static_calculat_center_rect(&dialog, size,flag));
    dialog.show();
}

void backheader::static_dialogshow_double(QDialog &dialog, const QString &title, const QSize &size, int flag)
{
    if(!title.isEmpty())dialog.setWindowTitle(title);
    fitlter_somewindowflag(&dialog);
    QRect rect=static_calculat_center_rect(&dialog, size,flag);
    rect.moveLeft(backheader::mainwindowsize.width()+rect.x());
    dialog.setGeometry(rect);
    dialog.show();
}

void backheader::static_dialogshow(QWidget &dialog, const QString &title, const QSize &size, int flag)
{
    if(!title.isEmpty())dialog.setWindowTitle(title);
    fitlter_somewindowflag(&dialog);
    dialog.setGeometry(static_calculat_center_rect(&dialog, size,flag));
    dialog.show();
}

bool backheader::operid_right_code(const QString &purview_no)
{
    if(n_func::gs_operid=="0000") return true;

    QSqlQuery query;
    query.exec(QString("SELECT vch_purviewname FROM hddpos.cey_sys_purview_list where ch_purviewno='%1';").arg(purview_no));
    query.next();
    QString vch_purviewname=query.record().value(0).toString();
    qDebug() << query.lastQuery();

    query.exec(QString("select * from cey_sys_oper_purview where ch_purviewno = '%1' and vch_operID='%2'")
               .arg(purview_no)
               .arg(n_func::gs_operid)
               );
    if(query.next()){
        return true;
    }
    lds_messagebox::warning(0, tr("消息提示"),purview_no+vch_purviewname+"\n"+tr("没有该权限"));
    return false;
}

QString backheader::html2plaintext(const QString &text)
{
    QTextDocument d;
    d.setHtml(text);
    return d.toPlainText();
}

void backheader::standmodelfilldata(QStandardItemModel *model, const QString &sql)
{
    model->removeRows(0,  model->rowCount());
    QSqlQueryModel querymodel;
    querymodel.setQuery(sql);
    for(int r=0,r_count=querymodel.rowCount();r<r_count;r++){
        QList<QStandardItem *>  items;
        for(int c=0,c_count=querymodel.columnCount();c<c_count;c++){
            items << new QStandardItem(querymodel.record(r).value(c).toString());
        }
        model->appendRow(items);
    }
}

QString backheader::getinterdata(const QString &_data_)
{
    QString ret;
    if(_data_.contains("[") && _data_.contains("]")){
        ret=_data_;
        ret=ret.split("[")[1];
        ret.chop(1);
    } else if(_data_.contains("(") && _data_.contains(")")){
        ret=_data_;
        ret=ret.split("(")[1];
        ret.chop(1);
    }

    return ret;
}

QString backheader::getouterdata(const QString &_data_)
{
    QString ret;
    if(_data_.contains("[") && _data_.contains("]")){
        ret=_data_.split("[")[0];
    } else if(_data_.contains("(") && _data_.contains(")")){
        ret=_data_.split("(")[0];
    }

    return ret;
}

QString backheader::getouterdata(const QStringList &_data_s, const QString &cmpvalue_1)
{
    foreach(QString _data_, _data_s){
        if(getinterdata(_data_)==cmpvalue_1){
            return getouterdata(_data_);
        }
    }
    return "";
}

void backheader::asyn_dataprint(const blockIns &printdata)
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    asyn_dataprint(printdata, conf.value("system_setting/printerpath").toString());
}

void backheader::asyn_dataprint(const blockIns &printdata, const QString &path)
{
    printer->asynchPrint(printdata, path);
}

void backheader::asyn_dataprint(const QString &printdata, const QString &path)
{
    blockIns bins;
    bins.append(pieceIns("",printdata.toLocal8Bit()));
    asyn_dataprint(bins,path);
}

void backheader::asyn_dataprin_test(const QString &path)
{
    blockIns bins;
    bins.append(pieceIns("",
                         "====print test success!====\n"
                         "====print test success!====\n"));
    asyn_dataprint(bins,path);
}

bool backheader::syn_dataprint(const blockIns &printdata)
{
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    return syn_dataprint(printdata,conf.value("system_setting/printerpath").toString());
}

bool backheader::syn_dataprint(const blockIns &printdata, const QString &path)
{
    return printer->synchPrint(printdata, path);
}

void backheader::syn_dataprin_test(const QString &path)
{
    blockIns bins;
    bins.append(pieceIns("1B6102",
                         "====print test success!====\n"
                         "====print test success!====\n"));

    syn_dataprint(bins,path);
}

int backheader::VIPFINDVALUE(const QStandardItemModel &model, const QString &cmpvalue, int cmpindex)
{
    int ret=-1;
    for(int r=0,r_count=model.rowCount(),c_count=model.columnCount();r<r_count;r++){
        if(cmpindex >=c_count) break;
        if(model.item(r, cmpindex)->text()==cmpvalue){
            ret=r;
            break;
        }
    }
    return ret;
}

QString backheader::VIPFINDVALUE(const QStandardItemModel &model, const QString &cmpvalue_0)
{
    QString ret;
    for(int r=0,r_count=model.rowCount();r<r_count;r++){
        if(model.item(r, 0)->text()==cmpvalue_0){
            ret=model.item(r, 1)->text();
            break;
        }
    }
    return ret;
}

void backheader::modelSetHeader(QAbstractItemModel *model, const QStringList &headers)
{
    for(int r=0,r_count=headers.count(); r < r_count; r++)
        model->setHeaderData(r, Qt::Horizontal, headers[r]);
}

void backheader::regetvch_operid_name()
{
    vch_operid_name.clear();
    QSqlQuery query;
    query.exec("Select concat(vch_operator, '(', vch_operID, ')')as typename from (Select vch_operID, vch_operator from cey_sys_operator union Select '0000' as vch_operID,'system' as vch_operator)t ");
    while(query.next()){
        vch_operid_name.append(query.record().value(0).toString());
    }
}

void backheader::regetvch_dish_typename()
{
    vch_dish_typename.clear();
    QSqlQuery query;
    query.exec("Select concat(vch_dish_typename, '(', ch_dish_typeno, ')')as typename from cey_bt_dish_type ");
    while(query.next()){
        vch_dish_typename.append(query.record().value(0).toString());
    }
}

void backheader::regetvch_unitname()
{
    vch_unitname.clear();
    QSqlQuery query;
    query.exec("Select concat(vch_unitname, '(', ch_unitno, ')')as typename from cey_bt_unit ");
    while(query.next()){
        vch_unitname.append(query.record().value(0).toString());
    }
}

void backheader::BACKHEADER_INIT()
{
    qDebug() << "BACKHEADER_INIT...";
#ifdef QT_DEBUG
//    QSize desk_size=QSize(1024, 768);
        QSize desk_size=QSize(800, 600);
    //    QSize desk_size=qApp->desktop()->size();
    sys_language="zn";
#else
    QSize desk_size;
    if(qApp->desktop()->size().width()==800)
        desk_size=QSize(800,600);
    else
        desk_size=QSize(1024,768);
    QSettings conf("/usr/desk/desktop.ini", QSettings::IniFormat);
    QString tran=conf.value("desktop/language").toString();
    if(tran!="1"){
        sys_language="en";
    } else {
        sys_language="zn";
    }
#endif

    if(desk_size.width()==800){
        sys_desk_bottom_height=40+30;
        sys_content_size="800_560";
        sys_content_title_size="800_108";
    } else {
        sys_desk_bottom_height=40+30;//1024 738
        sys_content_size="1024_728";
        sys_content_title_size="1024_147";
    }
    mainwindowsize=QSize(desk_size.width(), desk_size.height()-sys_desk_bottom_height);
    fullwindowsize=QSize(desk_size.width(), desk_size.height()-40);

}

void backheader::lds_com_setdatas(const QAbstractItemModel *model, lds_ComboBox *com)
{
    com->clear();
    QList<lds_ComboBox::PAIRLIST> citems;
    for(int row=0,rowcount=model->rowCount();row<rowcount;row++){
        citems.append(lds_ComboBox::PAIRLIST(model->index(row, 1).data().toString()
                                             ,model->index(row, 0).data().toString()));
    }
    com->addItems(citems);
}

void backheader::lds_com_setitems(const QAbstractItemModel *model, lds_ComboBox *com)
{
    com->clear();
    QStringList items;
    for(int row=0,rowcount=model->rowCount();row<rowcount;row++){
        items.append(model->index(row, 1).data().toString());
    }
    com->addItems(items);
}
