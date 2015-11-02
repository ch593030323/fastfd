#ifndef TAKEOUT_COOK_H
#define TAKEOUT_COOK_H

#include <QDialog>
#include "public_mainprinter.h"
#include "ui_takeout_cook.h"
#include "w_scr_dish.h"
class fast_order_2;

namespace Ui {
class takeout_cook;
}

class takeout_cook : public QDialog
{
    Q_OBJECT
    
public:
    explicit takeout_cook(fast_order_2 *parent = 0);
    ~takeout_cook();
    void init_control();
    void init_data();
    bool btn_t_f;
    void print();

    Ui::takeout_cook *ui;
public slots:
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void print_lost(const QString &str);

private:
    QString liushuihao;
    QString wdanhao;
    fast_order_2 *_parent;

//    public_mainprinter *printer;
};

#endif // TAKEOUT_COOK_H
