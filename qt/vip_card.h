#ifndef VIP_CARD_H
#define VIP_CARD_H

#include <QDialog>
class fast_pay;
namespace Ui {
class vip_card;
}

class vip_card : public QDialog
{
    Q_OBJECT
    
public:
    explicit vip_card(const QString & yfye,const QString & xf,const QString zk,const QString & zs,const QString & ys,const QString & ml,const QString & ss,const QString & xj,
                      fast_pay *parent = 0);
    ~vip_card();
    void init_control();
    bool btn_t_f;
    QString ch_attribute;
    float discount;
    QString _num;
    QString _xf;
    QString _zk;
    QString _zs;
    QString _ys;
    QString _ml;
    QString _ss;
    QString _xj;
    QString fukuankahao;
    QString beizhu;
    QString time;
    QString _card_pay;
    bool is_yun_member;
public slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void tomember_payall(const QString &arg1);

private:
    Ui::vip_card *ui;
    QString member_payall_save;
    fast_pay *_parent;
};

#endif // VIP_CARD_H
