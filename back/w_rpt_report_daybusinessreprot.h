#ifndef W_RPT_REPORT_DAYBUSINESSREPROT_H
#define W_RPT_REPORT_DAYBUSINESSREPROT_H

#include <QDialog>
#include "w_rpt_report.h"
class QStandardItemModel;
class Ui_w_rpt_report_daybusinessreprot_Dialog;
class w_rpt_report_daybusinessreprot : public QDialog
{
    Q_OBJECT
public:
    explicit w_rpt_report_daybusinessreprot(QWidget *parent = 0);
    
    void tab1sqlinit();

private slots:
    void toselect();
    void toexit();
    void updatecombegindate(int yearindex);
    void savebeginset();
    void loadbeginset();
    QString itemtextspan(const QString &itemtext);
private:
    Ui_w_rpt_report_daybusinessreprot_Dialog *ui;
    QMap<QString , w_rpt_report::TABTYPE> tab1info;
    QStandardItemModel *standmodel;
    
};

#endif // W_RPT_REPORT_DAYBUSINESSREPROT_H
