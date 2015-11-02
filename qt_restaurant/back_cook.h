#ifndef BACK_COOK_H
#define BACK_COOK_H

#include <QDialog>

namespace Ui {
class back_cook;
}

class back_cook : public QDialog
{
    Q_OBJECT
    
public:
    explicit back_cook(const QString & name,const QString bianma,QWidget *parent = 0);
    ~back_cook();
    void init_control();
    bool bakc_one;
    bool back_all;
    QString cook_causes;
    float back_num;
private slots:
    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::back_cook *ui;
    QString _name;
    QString _bianma;
};

#endif // BACK_COOK_H
