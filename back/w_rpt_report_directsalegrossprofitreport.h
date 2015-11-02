#ifndef W_RPT_REPORT_DIRECTSALEGROSSPROFITREPORT_H
#define W_RPT_REPORT_DIRECTSALEGROSSPROFITREPORT_H

#include <QDialog>
#include <QMap>
#include <QModelIndex>
#include "w_rpt_report.h"

class Ui_w_rpt_report_directsalegrossprofitreport_Dialog;
class QSqlQueryModel;
class QTableView;
class w_rpt_report_directsalegrossprofitreport : public QDialog
{
    Q_OBJECT
public:
    explicit w_rpt_report_directsalegrossprofitreport(QWidget *parent = 0);
    
    ~w_rpt_report_directsalegrossprofitreport();
    void tab1sqlinit();
//    void tab2sqlinit();

private slots:
    void toselect();
    void toexit();
    void tocom12(const QString &text);

private:
    Ui_w_rpt_report_directsalegrossprofitreport_Dialog *ui;
    QMap<QString , w_rpt_report::TABTYPE> tab1info;
    QSqlQueryModel *querymodel;

};

#endif // W_RPT_REPORT_DIRECTSALEGROSSPROFITREPORT_H
