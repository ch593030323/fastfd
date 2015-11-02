#ifndef LABLE_LIST_H
#define LABLE_LIST_H

#include <QLabel>
#include <QFont>
#include <QString>

class lable_list : public QLabel//基本，只有三次lable结构
{
    Q_OBJECT
public:
    explicit lable_list(int x, int y, const QString & text,
                        const QString & m1,const QString & m2, QWidget *parent = 0);
protected:
    QLabel *lable_bottom;

    QLabel *lable_between;
    //QLabel *lable_top;

    qint32 xx,yy;//一下标的形式记录
    //"行号"<<"品码"<<"品名"<<"数量"<<"价格"<<"折扣"<<"折扣(%)"<<"金额"<<"厨打说明"<<"点菜行号"<<"套菜"<<"特价"<<"赠送"<<"优惠券";

    QStringList info;
    QString p1, p2;

signals:
    void typeup();
    void typedown();
private:
    virtual void mousePressEvent (QMouseEvent * ev);

};

class lsit_label_info : public lable_list
{

    Q_OBJECT
public:
    explicit lsit_label_info(int x, int y, const QString & text,
                             const QString & m1,const QString &m2,
                             bool first_is_selected=true,
                             QWidget *parent = 0);
    static QPoint flag;//第几个标记

    static lsit_label_info *lable1;//标记上一个label

    void  StoType(){emit toType(0);}

signals:
    void toType(int order);

private:
    virtual void mousePressEvent (QMouseEvent * ev);
    QPoint type_point;

};

#endif // LABLE_LIST_H
