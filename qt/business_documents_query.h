#ifndef BUSINESS_DOCUMENTS_QUERY_H
#define BUSINESS_DOCUMENTS_QUERY_H

#include <QDialog>
#include <QStandardItemModel>
#include "public_mainprinter.h"
#include "sqlquerymodel_linenum1.h"
#include "ui_business_documents_query.h"
//营业单据查询
namespace Ui {
class business_documents_query;
}

class business_documents_query : public QDialog
{
    Q_OBJECT
    
public:
    explicit business_documents_query(QWidget *parent = 0);
    ~business_documents_query();
    void init_data();
    Ui::business_documents_query *ui;
private slots:
    void on_pushButton_6_clicked();

    void on_pushButton_2_clicked();

    void tableView_clicked(const QModelIndex &index);

    void on_pushButton_4_clicked();

    void on_checkBox_clicked();

    void on_pushButton_5_clicked();

    void print_lost(const QString & str);

private:
//    QStandardItemModel *model_top;
//    QStandardItemModel *model_left;
//    QStandardItemModel *model_right;
    SqlQueryModel_Linenum1 *model_top;
    SqlQueryModel_Linenum1 *model_left;
    SqlQueryModel_Linenum1 *model_right;
//    public_mainprinter *printer;
};

#endif // BUSINESS_DOCUMENTS_QUERY_H
