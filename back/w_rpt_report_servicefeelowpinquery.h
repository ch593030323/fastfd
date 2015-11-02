#ifndef W_RPT_REPORT_SERVICEFEELOWPINQUERY_H
#define W_RPT_REPORT_SERVICEFEELOWPINQUERY_H

#include <QDialog>
#include <QMap>
#include <QModelIndex>
#include "w_rpt_report.h"

class QSqlQueryModel;
class Ui_w_rpt_report_cashreport_Dialog;
class QTableView;
class w_rpt_report_servicefeelowpinquery : public QDialog
{
    Q_OBJECT
public:
    explicit w_rpt_report_servicefeelowpinquery(QWidget *parent = 0);
    ~w_rpt_report_servicefeelowpinquery();
    void tab1sqlinit();

private slots:
    void toselect();
    void toexit();
private:
    Ui_w_rpt_report_cashreport_Dialog *ui;
    QMap<QString , w_rpt_report::TABTYPE> tab1info;
    QSqlQueryModel *querymodel;
};

#endif // W_RPT_REPORT_SERVICEFEELOWPINQUERY_H
