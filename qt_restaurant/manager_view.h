#ifndef MANAGER_VIEW_H
#define MANAGER_VIEW_H

#include <QDialog>
#include "ui_manager_view.h"

namespace Ui {
class manager_view;
}

class manager_view : public QDialog
{
    Q_OBJECT
    
public:
    explicit manager_view(QWidget *parent = 0);
    ~manager_view();
    QString fast_model;
signals:
    void kuacan();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_13_clicked();

public:
    Ui::manager_view *ui;
};

#endif // MANAGER_VIEW_H
