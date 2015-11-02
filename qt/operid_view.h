#ifndef OPERID_VIEW_H
#define OPERID_VIEW_H

#include <QDialog>
#include <QSqlQueryModel>
#include "lable_list.h"

namespace Ui {
class operid_view;
}

class operid_view : public QDialog
{
    Q_OBJECT
    
public:
    explicit operid_view(QWidget *parent = 0);
    ~operid_view();
    QString operid;
    bool btn_t_f;
    void init_data();
private slots:
    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void show_label_num(int num = 0);

private:
    Ui::operid_view *ui;
    QSqlQueryModel *model;
    lable_list *list_info;
};

#endif // OPERID_VIEW_H
