#ifndef W_SYS_MANAGE_OPERATORINFORMATION_H
#define W_SYS_MANAGE_OPERATORINFORMATION_H

#include <QDialog>
#include <QModelIndex>
class Ui_w_sys_manage_operatorinformation_Dialog;
class lds_model_sqlrelationaltablemodel;
class QDataWidgetMapper;

class w_sys_manage_operatorinformation : public QDialog
{
    Q_OBJECT
public:
    explicit w_sys_manage_operatorinformation(QWidget *parent = 0);
    lds_model_sqlrelationaltablemodel *tablemodel;
    Ui_w_sys_manage_operatorinformation_Dialog *ui;

private slots:
    void tonew();
    void torefresh();
    void todel();
    bool tosave();
    void toexit();
    void mapper_setcur_tableviewcur();
    void lineeditfinish();
signals:
    void lineedit_connect_change(QWidget *widget);
private:
    QDataWidgetMapper *mapper;
};
#endif // W_SYS_MANAGE_OPERATORINFORMATION_H
