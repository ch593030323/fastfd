#ifndef RES_SPECIAL_DISPOSE_H
#define RES_SPECIAL_DISPOSE_H

#include <QDialog>

namespace Ui {
class res_special_dispose;
}

class res_special_dispose : public QDialog
{
    Q_OBJECT
    
public:
    explicit res_special_dispose(QWidget *parent = 0);
    ~res_special_dispose();
    void init_control();
    bool btn_t_f;
    int curindex_id;
    QString beizhu;
private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::res_special_dispose *ui;
};

#endif // RES_SPECIAL_DISPOSE_H
