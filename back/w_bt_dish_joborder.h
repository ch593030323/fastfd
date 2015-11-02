#ifndef W_BT_DISH_JOBORDER_H
#define W_BT_DISH_JOBORDER_H

#include <QDialog>
#include "w_bt_dish.h"
#include "lds_model_sqlrelationaltablemodel.h"

class Ui_w_bt_dish_unitset_Dialog;
class lds_model_sqlrelationaltablemodel;
class lds_tableView;

class w_bt_dish_joborder : public QDialog
{
    Q_OBJECT
public:
    class RelationalTableModel : public lds_model_sqlrelationaltablemodel{
    public:
        explicit RelationalTableModel(QObject *parent = 0,
                                      QSqlDatabase db = QSqlDatabase()):
            lds_model_sqlrelationaltablemodel(parent, db){}
        QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;
    };
public:
    explicit w_bt_dish_joborder(QWidget *parent = 0);

private slots:
    void tonew();
    void todel();
    void torefresh();
    bool tosave();
    void toexit();
private:
    Ui_w_bt_dish_unitset_Dialog *ui;
    lds_model_sqlrelationaltablemodel *tablemodel;
    
};

#endif // W_BT_DISH_JOBORDER_H
