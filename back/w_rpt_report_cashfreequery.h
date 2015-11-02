#ifndef W_RPT_REPORT_CASHFREEQUERY_H
#define W_RPT_REPORT_CASHFREEQUERY_H

#include <QDialog>
#include "w_rpt_report.h"
class Ui_w_rpt_report_cashfreequery_Dialog;
class w_rpt_report_cashfreequery : public QDialog
{
    Q_OBJECT
public:
    explicit w_rpt_report_cashfreequery(QWidget *parent = 0);
    
    void tab1sqlinit();

private slots:
    void toselect();
    void toexit();

private:
    Ui_w_rpt_report_cashfreequery_Dialog *ui;
    QMap<QString , w_rpt_report::TABTYPE> tab1info;
    QSqlQueryModel *querymodel;
    
};

#endif // W_RPT_REPORT_CASHFREEQUERY_H
