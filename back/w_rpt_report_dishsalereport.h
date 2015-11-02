#ifndef W_RPT_REPORT_DISHSALEREPORT_H
#define W_RPT_REPORT_DISHSALEREPORT_H

#include <QDialog>
#include <QMap>
#include <QModelIndex>
#include "w_rpt_report.h"

class QSqlQueryModel;
class Ui_w_rpt_report_dishsalereport_Dialog;
class QTableView;
class w_rpt_report_dishsalereport : public QDialog
{
    Q_OBJECT
public:
    explicit w_rpt_report_dishsalereport(QWidget *parent = 0);
    ~w_rpt_report_dishsalereport();
    void tab1sqlinit();
    void tab2sqlinit();

private slots:
    void toselect();
    void toexit();
    void tocom12(const QString &text);

private:
    Ui_w_rpt_report_dishsalereport_Dialog *ui;
    QMap<QString , w_rpt_report::TABTYPE> tab1info;
    QSqlQueryModel *querymodel;

    QMap<QString , w_rpt_report::TABTYPE> tab2info;
    QSqlQueryModel *querymodel_2;
};

#endif // W_RPT_REPORT_DISHSALEREPORT_H
