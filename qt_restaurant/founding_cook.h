#ifndef FOUNDING_COOK_H
#define FOUNDING_COOK_H

#include <QDialog>

namespace Ui {
class founding_cook;
}

class founding_cook : public QDialog
{
    Q_OBJECT
    
public:
    explicit founding_cook(const QString &danhao,const QString &zhuohao,const QString &renshu,const QString &kaitairen,const QString &time,const QString & str,QWidget *parent = 0);
    ~founding_cook();
    QString _danhao;
    QString _zhuohao;
    QString _renshu;
    QString _kaitairen;
    QString _time;
    bool wangluo;
    bool dianccai;
    bool btn_t_f;
    QString songcanren;
    QString beizhu;
    QString _str;
private slots:
    void on_pushButton_14_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::founding_cook *ui;
};

#endif // FOUNDING_COOK_H
