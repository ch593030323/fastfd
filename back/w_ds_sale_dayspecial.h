#ifndef W_DS_SALE_DAYSPECIAL_H
#define W_DS_SALE_DAYSPECIAL_H

#include <QDialog>
 #include <QSqlQueryModel>
#include "lds_model_sqlrelationaltablemodel.h"
#include "backheader.h"

class Ui_w_ds_sale_dayspecial_Dialog;

class w_ds_sale_dayspecial : public QDialog
{
    Q_OBJECT
public:
    explicit w_ds_sale_dayspecial(QWidget *parent = 0);


private:
    Ui_w_ds_sale_dayspecial_Dialog *ui;
    
};

#endif // W_DS_SALE_DAYSPECIAL_H
