#ifndef W_RPT_REPORT_DISHSALERANK_H
#define W_RPT_REPORT_DISHSALERANK_H

#include <QDialog>
#include <QMap>
#include <QModelIndex>
#include "w_rpt_report.h"
#include <QSqlQueryModel>

class Ui_w_rpt_report_directsalegrossprofitreport_Dialog;
class QSqlQueryModel;
class QTableView;
class w_rpt_report_dishsalerank : public QDialog
{
    Q_OBJECT
   private:
    class RankQueryModel : public QSqlQueryModel{
    public:
        explicit RankQueryModel(QObject *parent = 0) :
            QSqlQueryModel(parent),
        _ldec_cost(0.0),
        _ldec_income(0.0){}
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &item, int role=Qt::DisplayRole) const;

        double _ldec_cost;
        double _ldec_income;
    };

public:
    explicit w_rpt_report_dishsalerank(QWidget *parent = 0);

    void tab1sqlinit();
//    void tab2sqlinit();

private slots:
    void toselect();
    void toexit();
    void tocom12(const QString &text);

private:
    Ui_w_rpt_report_directsalegrossprofitreport_Dialog *ui;
    QMap<QString , w_rpt_report::TABTYPE> tab1info;
    RankQueryModel *querymodel;

    //由于这儿会多出一列用于 计算 收入金额
    
};

#endif // W_RPT_REPORT_DISHSALERANK_H
