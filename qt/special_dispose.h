#ifndef SPECIAL_DISPOSE_H
#define SPECIAL_DISPOSE_H

#include <QDialog>

namespace Ui {
class special_dispose;
}

class special_dispose : public QDialog
{
    Q_OBJECT
    
public:
    explicit special_dispose(QWidget *parent = 0);
    ~special_dispose();
    void init_control();
    bool btn_t_f;
    int curindex_id;
    QString beizhu;
private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::special_dispose *ui;
};

#endif // SPECIAL_DISPOSE_H
