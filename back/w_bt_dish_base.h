#ifndef W_BT_DISH_BASE_H
#define W_BT_DISH_BASE_H

#include <QDialog>
#include "lds_model_sqlrelationaltablemodel.h"
#include "w_sys_manage.h"

#include "lds_model_sqlrelationaltablemodel.h"
#include "lds_model_sqlrelationaltablemodel.h"
#include <QMap>

class QSqlTableModel;
class QSqlQueryModel;
class lds_treeWidgetItem;
class Ui_w_bt_dish_Dialog;
class lds_tableView;
class w_bt_dish_base_change_select_dish_image;
//菜品信息
class w_bt_dish_base : public QDialog
{
    Q_OBJECT
public:
    class RelationalTableModel : public lds_model_sqlrelationaltablemodel{
    public:
        explicit RelationalTableModel(QObject *parent = 0,
                                          QSqlDatabase db = QSqlDatabase());
        QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;
    };
public:
    explicit w_bt_dish_base(QWidget *parent = 0);
    ~w_bt_dish_base();
public slots:
    void torefresh();//树状图刷新，tableview刷新

    void treeselect();//tableview刷新

    void to_dish_type();
    void totypeset();
    void tounitset();
    void tonew();
    void tochange();
    void todel();
    void toexport();
    void toexit();
public:
    Ui_w_bt_dish_Dialog *ui;
    lds_model_sqlrelationaltablemodel *tablemodel;
    w_bt_dish_base_change_select_dish_image *select_dish_image_dialog;
    QString sql_select;
};

//品类设置
class Ui_w_bt_dish_typeset_Dialog;
class w_bt_dish_base_typeset:public QDialog{
    Q_OBJECT
public:
    explicit w_bt_dish_base_typeset(w_bt_dish_base *parent = 0);
private slots:
    void tonew2();
    void todel2();
    bool tosave();
    void torefresh();
    void toexit();

    void tableview2update(const QModelIndex &index);
protected:
    lds_model_sqlrelationaltablemodel *tablemodel,*tablemodel_2;
    Ui_w_bt_dish_typeset_Dialog *ui;
    w_bt_dish_base *_parent;
};
//大类设置
class Ui_w_bt_dish_unitset_Dialog;
class w_bt_dish_base_dish_type:public QDialog{
    Q_OBJECT
public:
    explicit w_bt_dish_base_dish_type(w_bt_dish_base *parent = 0);
private slots:
    void tonew();
    void todel();
    bool tosave();
    void torefresh();
    void toexit();
private:
    Ui_w_bt_dish_unitset_Dialog *ui;
    lds_model_sqlrelationaltablemodel *tablemodel;
    w_bt_dish_base *_parent;
};

//单位设置
class w_bt_dish_base_unitset:public QDialog{
    Q_OBJECT
public:
    explicit w_bt_dish_base_unitset(QWidget *parent = 0);
private slots:
    void tonew();
    void todel();
    bool tosave();
    void torefresh();
    void toexit();
private:
    int findunitno(const QString &unitno, int exceptrow);
private :
    Ui_w_bt_dish_unitset_Dialog *ui;
    class lds_model_sqlrelationaltablemodel_tmp : public lds_model_sqlrelationaltablemodel{
        QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const;
    };

    lds_model_sqlrelationaltablemodel_tmp *tablemodel;

};
//修改
class Ui_w_bt_dish_new_Dialog;
class QDataWidgetMapper;
class QTableView;
class QComboBox;
class QLineEdit;
class w_bt_dish_base_change:public QDialog{
    Q_OBJECT
public:
    explicit w_bt_dish_base_change(w_bt_dish_base *parent);
protected slots:
    virtual bool tosave();
    virtual void tocontinue();
    virtual void toexit();
    void toup();//下按钮
    void todown();//上按钮
    void spellupdate(const QString &text);//拼音简码更新
    virtual void mapperupdate();//mapper->setCurrentIndex(_parent->getTableview()->currentIndex().row());
    void update_ch_deducttype(const QString &type);
    void to_select_dishpic();
protected:
    Ui_w_bt_dish_new_Dialog *ui;
    QDataWidgetMapper *mapper;
    QLineEdit *tmp_ch_deducttype;
    w_bt_dish_base *_parent;
    QString image_dish_path;
};
//新增
class w_bt_dish_base_new:public w_bt_dish_base_change{
    Q_OBJECT
public:
    explicit w_bt_dish_base_new(w_bt_dish_base *parent , const QString &vch_dish_typename,const QString &vch_sub_typename);

private slots:
    virtual bool tosave();
    virtual void tocontinue();
    virtual void toexit();
    virtual void mapperupdate();//mapper->setCurrentIndex(-1);

private:
    QString ch_dish_typeno,ch_sub_typeno;
    QString _vch_dish_typename,_vch_sub_typename;

};
//菜品选择图片
class Ui_w_bt_dish_base_change_select_dish_image_Dialog;
class QListWidgetItem;
class w_bt_dish_base_change_select_dish_image : public QDialog
{
    Q_OBJECT
public:
    explicit w_bt_dish_base_change_select_dish_image(QWidget *parent = 0);

    QString select_text;
    void generate_image();
    void update_match_info();

signals:
    void signal_one_by_one_import(const QStringList &paths, int index);
private slots:
    void one_by_one_import();
    void took();
    void toexit();
    void to_where_value();
    void to_save_check(bool flag);
protected:
    void resizeEvent(QResizeEvent *e);
    void closeEvent(QCloseEvent *e);
private:
    Ui_w_bt_dish_base_change_select_dish_image_Dialog *ui;
    QMap<QString, QListWidgetItem*>text_item_map;

    QStringList one_by_one_list;
    int one_by_one_index;
};
//139 188 71647    冯工

#endif // W_BT_DISH_BASE_H
