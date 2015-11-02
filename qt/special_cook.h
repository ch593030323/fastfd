#ifndef SPECIAL_COOK_H
#define SPECIAL_COOK_H

#include <QDialog>
#include <QSqlQueryModel>
#include "lable_list.h"

namespace Ui {
class special_cook;
}

class special_cook : public QDialog
{
    Q_OBJECT
    
public:
    explicit special_cook(const QString & str,const QString & neirong,const QString & price,QWidget *parent = 0);
    ~special_cook();
    bool btn_t_f;
    void init_data();
    QStringList list;
    float add_price;
    QString zuofaneirong;
private slots:
    void show_label_num(int num = 0);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::special_cook *ui;
    QSqlQueryModel *model;
    QString zuofa;
    QString _neirong;
    QString _price;
    lable_list *list_info;
};

#endif // SPECIAL_COOK_H
