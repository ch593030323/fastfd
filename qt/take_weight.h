#ifndef TAKE_WEIGHT_H
#define TAKE_WEIGHT_H

#include <QDialog>
#include "fast_order_2.h"
#include "ui_take_weight.h"

class ElectronicCountPrice;
namespace Ui {
class take_weight;
}

class take_weight : public QDialog
{
    Q_OBJECT
    
public:
    explicit take_weight(fast_order_2 *parent = 0);
    ~take_weight();
    QString _weight;
    Ui::take_weight *ui;
private slots:
    void toappend();
    void took();
    void toexit();

    void toqupi();
    void tozhiling();

    void updatetotal(const QString &text);
private:

    ElectronicCountPrice *countprice_scale;
    fast_order_2 *_parent;

};

#endif // TAKE_WEIGHT_H
