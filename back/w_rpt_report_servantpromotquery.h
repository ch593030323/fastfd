#ifndef W_RPT_REPORT_SERVANTPROMOTQUERY_H
#define W_RPT_REPORT_SERVANTPROMOTQUERY_H

#include <QDialog>
#include <QMap>
#include <QDateTime>
#include "w_rpt_report.h"
class Ui_w_rpt_report_servantpromotquery_Dialog;
class w_rpt_report_servantpromotquery : public QDialog
{
    Q_OBJECT
public:
    explicit w_rpt_report_servantpromotquery(QWidget *parent = 0);
    ~w_rpt_report_servantpromotquery();

private:
    void tab1sqlinit();
private slots:
    void toselect();
    void toexit();

    void tocomtype(const QString &text);
    
private:
    Ui_w_rpt_report_servantpromotquery_Dialog *ui;
    QMap<QString , w_rpt_report::TABTYPE> tab1info;
    QSqlQueryModel *querymodel;

    QMap<QString , w_rpt_report::TABTYPE> tab2info;
    QSqlQueryModel *querymodel_2;
};

#endif // W_RPT_REPORT_SERVANTPROMOTQUERY_H
