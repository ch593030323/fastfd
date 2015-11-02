#ifndef BACK_VIEW_2_H
#define BACK_VIEW_2_H

#include <QDialog>
#include "ui_back_view_2.h"

namespace Ui {
class back_view_2;
}

class back_view_2 : public QDialog
{
    Q_OBJECT
public:
    explicit back_view_2(QWidget *parent = 0);
    ~back_view_2();
    void delay_update_title_pic();
    QString def_logo_path;
    QString def_logo_path_2;
    QString def_logo_text;
    QString def_logo_text_2;
public slots:
    void update_title_pic();//优先配置文件的text和pic，否这则使用 系统自带的
signals:
    void signal_index(int page, int index);
protected:
    void paintEvent(QPaintEvent *e);
private slots:
    void on_radioButton_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_5_clicked();

    void on_radioButton_4_clicked();

    void on_radioButton_8_clicked();

    void on_radioButton_7_clicked();

    void on_radioButton_6_clicked();

    void to_center_click();

public:
    Ui::back_view_2 *ui;
};

#endif // BACK_VIEW_2_H
