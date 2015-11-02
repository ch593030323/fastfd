#ifndef TABLE_PAY_2_H
#define TABLE_PAY_2_H

#include <QWidget>
#include "fast_pay.h"
class frontViewDialog_virtual;
class table_pay_2 : public fast_pay
{
    Q_OBJECT
public:
    explicit table_pay_2(frontViewDialog_virtual *parent = 0);

    void jiuloushow();
public slots:
    virtual void to_pay();
    virtual void tokitchenprint(const QString &key/*"", out_*/, const QString &_dayindanhao, const QString &serial_no);
    virtual void tolabelprint();

signals:
    void change();
    void yudadan();//预打单
};

#endif // TABLE_PAY_2_H
