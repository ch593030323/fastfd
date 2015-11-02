#ifndef DISH_CHANGE_H
#define DISH_CHANGE_H

#include <QDialog>
#include <QSqlQueryModel>
#include "lable_list.h"

namespace Ui {
class dish_change;
}

class dish_change : public QDialog
{
    Q_OBJECT
    
public:
    explicit dish_change(int num,const QString & str,QWidget *parent = 0);
    ~dish_change();
    void init_data();
    QString dish_no;
    QString dish_name;
    float dish_num;
    QString dish_unit;
    bool btn_t_f;
    bool quanbu;
private slots:
    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_15_clicked();
    void show_label_num(int num = 0);
    void on_radioButton_13_clicked();

    void on_radioButton_14_clicked();

    void on_radioButton_15_clicked();

private:
    Ui::dish_change *ui;
    int cur_row;
    QSqlQueryModel *model;
    QSqlQueryModel *flowid;
    lable_list *list_info;
    int cur_num;
    QString _str;
};

#endif // DISH_CHANGE_H
