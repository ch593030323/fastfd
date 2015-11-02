#ifndef CHOOSE_VIP_H
#define CHOOSE_VIP_H

#include <QDialog>
#include <QStandardItemModel>
namespace Ui {
class choose_vip;
}

class choose_vip : public QDialog
{
    Q_OBJECT
    
public:
    explicit choose_vip(QWidget *parent = 0);
    ~choose_vip();
    void init_control();
    QString _text;
private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::choose_vip *ui;
   QStandardItemModel *model_table;
};

#endif // CHOOSE_VIP_H
