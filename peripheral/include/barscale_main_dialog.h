#ifndef BARSCALE_MAIN_DIALOG_H
#define BARSCALE_MAIN_DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QItemDelegate>
#include <QSqlTableModel>

#ifdef BARSCALE_MAIN_DIALOG
#define BARSCALE_MAIN_DIALOG_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define BARSCALE_MAIN_DIALOG_COMMON_DLLSPEC  Q_DECL_IMPORT
#endif
//1需要设置配置文件的filepath:CommonHeader::com_ini_filepath=qApp->applicationDirPath()+"/conf.ini";
//2需要设置根目录的dirpath:CommonHeader::com_root_dirpath=qApp->applicationDirPath();
namespace Ui {
class barscale_main_dialog;
}
class barscale_downdialog;
class BARSCALE_MAIN_DIALOG_COMMON_DLLSPEC barscale_main_dialog : public QDialog
{
    Q_OBJECT
public:
    struct BARSCAEL_ITEMS{
        BARSCAEL_ITEMS(){}
        BARSCAEL_ITEMS(
                const QString &_pluno,//plu号
                const QString &_dishname,//品名
                const QString &_dishno,//品码
                const QString &_price//价格
                ):
            pluno(_pluno),
            dishname(_dishname),
            dishno(_dishno),
            price(_price)
        {}
        QString pluno;//PLU编号
        QString dishname;//商品名
        QString dishno;//商品代码
        QString price;//单价(/kg)
    };

public:
    explicit barscale_main_dialog(QWidget *parent = 0);
    ~barscale_main_dialog();
    void add_down_infolist(const BARSCAEL_ITEMS &items);//添加商品信息
    void set_shop_name(const QString &shop_name);//设置商店名称
    QString shop_name();
    void inputfinished();
private:
    void update_downmodel();
    void creatItem(QList<QStandardItem *> &items, const QStringList &list);
    bool brefore_getscaletypes();
protected:
    void goodsadd(const QList<BARSCAEL_ITEMS> items);
protected slots:
    virtual void togoodadd();//添加

private slots:
    void on_pushButton_clicked();//下载商品信息

    void on_pushButton_2_clicked();//清除PLU

    void on_pushButton_3_clicked();//设置标签(内置)

    void on_pushButton_4_clicked();//网络管理

    void on_pushButton_5_clicked();//退出

    void downdialog_exit();

    void update_barcodetype(const QString scaletype);

    void save_barscale();//保存
    void popdjianinfo();

    void update_scaleflag(const QString &flag);//前导码

    void on_pushButton_export_clicked();//导出

    void on_pushButton_import_clicked();

    void on_pushButton_remove_clicked();//移除

private:
    Ui::barscale_main_dialog *ui;
    QSqlTableModel *downmodel0;
    QStandardItemModel *downmodel;
    barscale_downdialog *downdialog;
    bool down_isbreak;
    QList<BARSCAEL_ITEMS> items_list;
};

class barscale_main_delegate: public QItemDelegate{
public:
    barscale_main_delegate(barscale_main_dialog *parent, QObject *parent1=0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QMap<int, QString> modelinfos;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    barscale_main_dialog *_parent;
};
#endif // BARSCALE_MAIN_DIALOG_H
