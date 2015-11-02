#ifndef BACK_CAUSES_H
#define BACK_CAUSES_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class back_causes;
}

class back_causes : public QDialog
{
    Q_OBJECT
    
public:
    explicit back_causes(QWidget *parent = 0);
    ~back_causes();
    void init_control();
    QString causes;
private slots:
    void on_pushButton_15_clicked();

    void on_pushButton_7_clicked();

    void on_checkBox_ch_curprice_4_clicked();

private:
    Ui::back_causes *ui;
    QStandardItemModel *table_model;
};

#endif // BACK_CAUSES_H
