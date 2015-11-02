#ifndef TABLE_NUMBER_MESSAGE_H
#define TABLE_NUMBER_MESSAGE_H

#include <QObject>
#include <QLabel>

class Base_label1 : public QLabel//基本，只有三次lable结构
{
    Q_OBJECT
public:
    explicit Base_label1(int x, int y, const QString & text,int type, QWidget *parent = 0);
protected:
    QLabel *lable_bottom;

    QLabel *lable_between;

    QLabel *lable_top;

    qint32 xx,yy;//一下标的形式记录
    //"行号"<<"品码"<<"品名"<<"数量"<<"价格"<<"折扣"<<"折扣(%)"<<"金额"<<"厨打说明"<<"点菜行号"<<"套菜"<<"特价"<<"赠送"<<"优惠券";

    QStringList info;
signals:
    void typeup();
    void typedown();
private:
    virtual void mousePressEvent (QMouseEvent * ev);

};

class Table_Number_Message : public Base_label1
{
    Q_OBJECT
public:
public:
    explicit Table_Number_Message(int x, int y, const QString & text,int type,QWidget *parent = 0);
    static QPoint flag;//第几个标记

    static Table_Number_Message *lable1;//标记上一个label

    void  StoType(){emit toType(0);}
signals:
    void toType(int order);

private:
    virtual void mousePressEvent (QMouseEvent * ev);
    
};

#endif // TABLE_NUMBER_MESSAGE_H
