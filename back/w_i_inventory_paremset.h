#ifndef W_I_INVENTORY_PAREMSET_H
#define W_I_INVENTORY_PAREMSET_H

#include <QDialog>
class Ui_w_i_inventory_paremset_Dialog;
class w_i_inventory_paremset : public QDialog
{
    Q_OBJECT
public:
    explicit w_i_inventory_paremset(QWidget *parent = 0);
    
private slots:
    void took();
    void toexit();
private:
    Ui_w_i_inventory_paremset_Dialog *ui;
    
};

#endif // W_I_INVENTORY_PAREMSET_H
