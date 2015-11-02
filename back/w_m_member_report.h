#ifndef W_M_MEMBER_REPORT_H
#define W_M_MEMBER_REPORT_H

#include <QDialog>
#include "ui_w_m_member_report_dialog.h"
#include "w_rpt_report.h"

class w_m_member_report : public QDialog
{
    Q_OBJECT
public:
    explicit w_m_member_report(QWidget *parent = 0);
    void tab1sqlinit();


    ~w_m_member_report();
private slots:
    void toselect();
    void toexit();
    void toupdate_tabw_list(int index);
private:
    Ui_w_m_member_report_Dialog *ui;
    QMap<QString , w_rpt_report::TABTYPE> tab1info;
    QSqlQueryModel *querymodel,*querymodel_1,*querymodel_2;
    QSqlQueryModel *querymodel_3,*querymodel_4,*querymodel_5;

};

#endif // W_M_MEMBER_REPORT_H
