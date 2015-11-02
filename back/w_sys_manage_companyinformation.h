#ifndef W_SYS_MANAGE_COMPANYINFORMATION_H
#define W_SYS_MANAGE_COMPANYINFORMATION_H

#include <QDialog>
class Ui_w_sys_manage_companyinformation_Dialog;
class lds_model_sqlrelationaltablemodel;
class QDataWidgetMapper;
class w_sys_manage_companyinformation : public QDialog
{
    Q_OBJECT
public:
    explicit w_sys_manage_companyinformation(QWidget *parent = 0);
    
private slots:
    bool tosave();
    void toexit();
private:
    Ui_w_sys_manage_companyinformation_Dialog *ui;
    lds_model_sqlrelationaltablemodel *tablemodel;
    QDataWidgetMapper *mapper;

};

#endif // W_SYS_MANAGE_COMPANYINFORMATION_H
