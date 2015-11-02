#ifndef W_RPT_REPORT_CATEGORYSALESANALYSIS_H
#define W_RPT_REPORT_CATEGORYSALESANALYSIS_H

#include <QDialog>
#include <QMap>
#include <QModelIndex>
#include "w_rpt_report.h"
class Ui_w_rpt_report_categorysalesanalysis_Dialog;
class QSqlQueryModel;
class QTableView;
class QStandardItemModel;
class w_rpt_report_categorysalesanalysis : public QDialog
{
    Q_OBJECT
public:
    explicit w_rpt_report_categorysalesanalysis(QWidget *parent = 0);
    
    void tab1sqlinit();
//    void tab2sqlinit();

private slots:
    void toselect();
    void tochart();
    void toexit();
    void tocom12(const QString &text);

private:
    Ui_w_rpt_report_categorysalesanalysis_Dialog *ui;
    QMap<QString , w_rpt_report::TABTYPE> tab1info;
    QStandardItemModel *standmodel;

    
};

#endif // W_RPT_REPORT_CATEGORYSALESANALYSIS_H
