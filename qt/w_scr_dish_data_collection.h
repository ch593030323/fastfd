#ifndef W_SCR_DISH_DATA_COLLECTION_H
#define W_SCR_DISH_DATA_COLLECTION_H

#include <QDialog>
class Ui_w_scr_dish_data_collection_dialog;
class lds_model_sqlrelationaltablemodel;
class lds_tableView;
class QDataWidgetMapper;
class w_scr_dish_data_collection : public QDialog
{
    Q_OBJECT
public:
    explicit w_scr_dish_data_collection(QWidget *parent = 0);

private slots:
    void toexit();
    void tosave();
    void mapper_setcurindex(int index);
private:
    Ui_w_scr_dish_data_collection_dialog *ui;
    lds_model_sqlrelationaltablemodel *tablemodel;
    QDataWidgetMapper *mapper;
};

#endif // W_SCR_DISH_DATA_COLLECTION_H
