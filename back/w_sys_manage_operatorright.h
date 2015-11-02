#ifndef W_SYS_MANAGE_OPERATORRIGHT_H
#define W_SYS_MANAGE_OPERATORRIGHT_H

#include <QDialog>
#include "w_sys_manage.h"
class Ui_w_sys_manage_operatorright_Dialog;
class QSqlTableModel;
class QSqlQueryModel;
class QStandardItemModel;

class w_sys_manage_operatorright : public QDialog
{
    Q_OBJECT
public:    
    explicit w_sys_manage_operatorright(QWidget *parent = 0);

    QSqlQueryModel *querymodel;
    Ui_w_sys_manage_operatorright_Dialog *ui;

public slots:
    void torefresh();
private slots:
    void toselectall();
    void toselectno();
    void todishright();
    void torightcopy();
    void topwdchange();
    bool tosave();
    void toexit();

    void rightmodelupdate();
private:
    QStandardItemModel *standmodel;
};

//权限copy
class Ui_w_sys_manage_operatorright_rightcopy_Dialog;
class w_sys_manage_operatorright_rightcopy : public QDialog
{
    Q_OBJECT
public:
    explicit w_sys_manage_operatorright_rightcopy(w_sys_manage_operatorright *parent = 0);
private slots:
    void tocopy();
    void toexit();
private:
    Ui_w_sys_manage_operatorright_rightcopy_Dialog *ui;
    w_sys_manage_operatorright *_parent;
    QSqlQueryModel *querymodel;
    QString vch_operidfrom;
};

//密码修改
class Ui_w_m_member_pwdchange_Dialog;
class w_sys_manage_operatorright_pwdchange : public QDialog{
    Q_OBJECT
public:
    explicit w_sys_manage_operatorright_pwdchange(w_sys_manage_operatorright *parent = 0);

    Ui_w_m_member_pwdchange_Dialog *ui;
private slots:
    void took();
    void toexit();
private:
};

#endif // W_SYS_MANAGE_OPERATORRIGHT_H
