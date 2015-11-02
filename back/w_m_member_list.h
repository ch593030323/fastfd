#ifndef W_M_MEMBER_LIST_H
#define W_M_MEMBER_LIST_H

#include <QDialog>
#include "lds_model_sqlrelationaltablemodel.h"
#include <QSqlRecord>
#include <QStandardItemModel>
#include "uo_rw_member.h"

//主界面
class QSqlTableModel;
class Ui_w_m_member_Dialog;
class QTableView;
class lds_model_sqlrelationaltablemodel;
class lds_tableView;
class lds_treeWidget;
class w_m_member_list : public QDialog
{
    Q_OBJECT
public:
    explicit w_m_member_list(QWidget *parent = 0);
    lds_tableView *getTableview();
    lds_treeWidget *getTreew();//获取tree

    lds_model_sqlrelationaltablemodel *getTablemodel();
    QString save_submit();

public:
    class RelationalTableModel:public lds_model_sqlrelationaltablemodel{
    public:
        RelationalTableModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
        QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;
    };
    static bool is_yun_member;//对这儿的会员是否是云端，这里需要开启和关闭功能

public slots:
    void torefresh();
    void treeselect();
    //外部调用例子    w_m_member_list::statictosendcard(this, tablemodel_2, ui->tableView, false);
    static void statictosendcard(w_m_member_list *parent, QSqlRelationalTableModel *tablemodel, lds_tableView *tableview, bool isenabled=true, const QString &title="");
    //外部调用例子    w_m_member_list::statictorecharge(this, 0, 0,"");
    static void statictorecharge(QWidget *parent, QSqlRelationalTableModel *tablemodel, lds_tableView *tableview, const QString &parm);
    //外部调用例子  statictomembertype(this);
    static void statictomembertype(w_m_member_list *parent);
    //外部调用读卡例子
    static bool static_to_memeber_readcard(QString &cardid, uo_rw_member &uo_member, bool isyun=false);
    static void yun_member_open();
    static void yun_member_close();
private slots:
    void tonew();
    void todel();
    void tonews();
    void tomembertype();
    void tosendcard();
    void torecharge();
    void tosetpwd();
    void toexport();
    void toexit();

signals:

public slots:
private:
    Ui_w_m_member_Dialog *ui;
    RelationalTableModel *tablemodel_2;QString t2_ch_typeno; QStringList headerstmp_2;
};
//发卡
class QDataWidgetMapper;
class Ui_w_m_member_sendcard_Dialog;
class lds_model_sqlrelationaltablemodel;
#include "uo_rw_member.h"
class w_m_member_list_sendcard : public QDialog{
    Q_OBJECT
public:
    explicit w_m_member_list_sendcard(w_m_member_list *parent ,
                                      QSqlRelationalTableModel *tablemodel, lds_tableView *tableview, bool isenable=true);
protected slots:
    void tonew();
    void todel();
    void tosendcard();
    void toup();
    void todown();
    virtual bool tosave();
    void toexit();

    virtual void torefresh();
protected:
    Ui_w_m_member_sendcard_Dialog *ui;
    QSqlRelationalTableModel *_tablemodel;
    lds_tableView *_tableview;
    QDataWidgetMapper *mapper;
    lds_model_sqlrelationaltablemodel *tablemodel_2;    QStringList headerstmp_2;
    lds_model_sqlrelationaltablemodel *tablemodel_3;
    QString is_cardtype;
    uo_rw_member *cb5;//pb其实时发卡按钮
    w_m_member_list *_parent;
};

//新增
class w_m_member_list_new : public w_m_member_list_sendcard{
    Q_OBJECT

public:
    explicit w_m_member_list_new(w_m_member_list *parent,QSqlRelationalTableModel *tablemodel,lds_tableView *tableview, bool isnew=true);
protected slots:
    virtual bool tosave();
    virtual void torefresh();
};
//会员类型
class Ui_w_bt_dish_unitset_Dialog;
class lds_model_sqlrelationaltablemodel;
class w_m_member_list_type : public QDialog{
    Q_OBJECT
public :
    class TableModel : public lds_model_sqlrelationaltablemodel{
    public:
        explicit TableModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase()):
            lds_model_sqlrelationaltablemodel(parent, db){}
        QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;

    };
public:
    explicit w_m_member_list_type(w_m_member_list *parent = 0);
    lds_model_sqlrelationaltablemodel *getTablemodel();
    lds_tableView *getTableView();
    w_m_member_list *_parent;
public slots:
    void torefresh();
private slots:
    void tonew();
    void todel();
    bool tosave();
    void toexit();
    void tochange();


public:
    Ui_w_bt_dish_unitset_Dialog *ui;
    lds_model_sqlrelationaltablemodel *tablemodel;    QStringList headerstmp;

};

#include <QSqlRelationalDelegate>
class mappdelegate:public QSqlRelationalDelegate{

public:
    explicit mappdelegate(QObject *parent = 0){}

};

//会员类型x修改
class Ui_w_m_member_list_type_new_Dialog;
class w_m_member_list_type_change: public QDialog{
    Q_OBJECT
public:
    explicit w_m_member_list_type_change(w_m_member_list_type *parent = 0);
protected slots:
    virtual bool tosave();
    virtual void toexit();
    virtual void mapperinit();

    void is_recharge(const QString &text);
    void is_point(const QString &text);

protected:
    Ui_w_m_member_list_type_new_Dialog *ui;
    w_m_member_list_type *_parent;
    QDataWidgetMapper *mapper;

};

//会员类型新增
class w_m_member_list_type_new : public w_m_member_list_type_change{
    Q_OBJECT
public:
    explicit w_m_member_list_type_new(w_m_member_list_type *parent = 0);
private slots:
    virtual bool tosave();
    virtual void toexit();
    virtual void mapperinit();
};
//会员卡充值
#include "uo_rw_member.h"
class Ui_w_m_member_list_recharge_Dialog;
class w_m_member_list_recharge : public QDialog{
    Q_OBJECT
public:
    explicit w_m_member_list_recharge(QWidget *parent, QSqlRelationalTableModel *tablemodel, lds_tableView *tableview, const QString &parm /*""充值, "C"取款, "D"退卡*/);
    ~w_m_member_list_recharge();
    //取款C
    void ue_fetch();
    //退卡D
    void ue_blankcard();

private slots:
    void rechargeprint();
    void toremainprint();
    void toreadcard();
    void torecharge();
    void toexit();

    void TtolineEdit_amount();

    void tochecksave();
protected:
    Ui_w_m_member_list_recharge_Dialog *ui;
    QDataWidgetMapper *mapper;
    bool _isready;
    QSqlRelationalTableModel *_tablemodel;
    lds_tableView *_tableview;
    QString _parm;
    uo_rw_member::read_card_type lst_card;
    QString bckey;

};
//密码修改
class Ui_w_m_member_pwdchange_Dialog;
class w_m_member_pwdchange  : public QDialog{
    Q_OBJECT
public:
    explicit w_m_member_pwdchange(QWidget *parent);
    QString check();
private slots:
    void took();
    void tocancel();
private:
    Ui_w_m_member_pwdchange_Dialog *ui;
    w_m_member_list *_parent;
    QString is_password;
    QString is_memberno;

};
//批量新增
class Ui_w_m_member_news_Dialog;
class w_m_member_news  : public QDialog{
    Q_OBJECT
public:
    explicit w_m_member_news(w_m_member_list *parent);
private slots:
    void took();
    void tocancel();
private:
    Ui_w_m_member_news_Dialog *ui;
    w_m_member_list *_parent;

};
#endif // W_M_MEMBER_LIST_H
