#ifndef BACKHEADER_H
#define BACKHEADER_H

#include <QObject>
#include <QSize>
#include <QStandardItemModel>
#include "public_mainprinter.h"
#include "public_printerr_dialog.h"
#include "lds_tableview_repecify.h"
#include "public_sql.h"
#include "w_sys_manage_cloudsync.h"
#include "lds_combobox.h"

//__FILE__
//__FUNCTION__
//__LINE__
//Qt::Dialog|Qt::WindowTitleHint;//只有logo和close的dialog
//Qt::Dialog|Qt::WindowTitleHint|Qt::CustomizeWindowHint; //什么都没有的dialog
class backheader : public QObject
{
    Q_OBJECT
public:
    explicit backheader(QObject *parent = 0);
    static QSize mainwindowsize;//主窗口的大小，底部高40px
    static QSize fullwindowsize;//前台,后台,中餐点菜,中餐前台
    static QString sys_language;//系统语言
    static QString sys_content_size;//后台图片的大小
    static QString sys_content_title_size;//后台图片title的大小
    static int sys_desk_bottom_height;//桌面下方的高度

    static void fitlter_somewindowflag(QWidget *dialog);
    static QRect static_calculat_center_rect(QWidget *dialog, const QSize &size, int flag);
    static int static_dialogexec(QDialog &dialog, const QString &title, const QSize &size=QSize(), int flag=Qt::AlignCenter);
    static void static_dialogshow(QDialog &dialog, const QString &title, const QSize &size=QSize(), int flag=Qt::AlignCenter);
    static void static_dialogshow_double(QDialog &dialog, const QString &title, const QSize &size=QSize(), int flag=Qt::AlignCenter);
    static void static_dialogshow(QWidget &dialog, const QString &title, const QSize &size=QSize(), int flag=Qt::AlignCenter);

    static bool operid_right_code(const QString& purview_no);
    static QString html2plaintext(const QString &text);

    static void standmodelfilldata(QStandardItemModel *model, const QString &sql);
    static QString getinterdata(const QString &_data_);//xx[data]
    static QString getouterdata(const QString &_data_);//xx[data]
    static QString getouterdata(const QStringList &_data_s, const QString &cmpvalue_1);//xx[data]

    static public_mainprinter *printer;//异步打印机
    static void asyn_dataprint(const blockIns &printdata);//开始打印
    static void asyn_dataprint(const blockIns &printdata, const QString &path);//开始打印
    static void asyn_dataprint(const QString &printdata, const QString &path);//开始打印
    static void asyn_dataprin_test(const QString &path);//开始打印

    static bool syn_dataprint(const blockIns &printdata);//开始打印
    static bool syn_dataprint(const blockIns &printdata, const QString &path);//开始打印
    static void syn_dataprin_test(const QString &path);//开始打印
public:
    //检索第几个的值
    static int VIPFINDVALUE(const QStandardItemModel &model, const QString &cmpvalue, int cmpindex);
    //查找第一列的值，返回第二列的值
    static QString VIPFINDVALUE(const QStandardItemModel &model, const QString &cmpvalue_0);
    static void modelSetHeader(QAbstractItemModel *model, const QStringList &headers);

    static void regetvch_operid_name();//name(id)
    static QStringList vch_operid_name;

    static void regetvch_dish_typename();//name(id)
    static QStringList vch_dish_typename;

    static void regetvch_unitname();//name(id)
    static QStringList vch_unitname;

    static void BACKHEADER_INIT();

    static void lds_com_setdatas(const QAbstractItemModel *model, lds_ComboBox *com);
    static void lds_com_setitems(const QAbstractItemModel *model, lds_ComboBox *com);
};

//菜品检索的窗口

#endif // BACKHEADER_H
