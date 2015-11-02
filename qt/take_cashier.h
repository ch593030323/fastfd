#ifndef TAKE_CASHIER_H
#define TAKE_CASHIER_H

#include <QDialog>
#include "sqlquerymodel_linenum1.h"
#include "fast_order_2.h"

namespace Ui {
class take_cashier;
}

class take_cashier : public QDialog
{
    Q_OBJECT
    
public:
    explicit take_cashier(fast_order_2 *parent = 0);
    ~take_cashier();
    void init_control();
    void init_fastmodel();
    static int row;
    static SqlQueryModel_Linenum1 *model_top;
    static SqlQueryModel_Linenum1 *model_bottom;
private slots:
    void on_pushButton_5_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void update_quitbutton (const QModelIndex & current);

private:
    Ui::take_cashier *ui;
    fast_order_2 *_parent;
};

#endif // TAKE_CASHIER_H
