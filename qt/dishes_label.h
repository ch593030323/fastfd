#ifndef DISHES_LABEL_H
#define DISHES_LABEL_H
#include <QLabel>
#include <QFont>
#include <QString>

class Base_label : public QLabel//基本，只有三次lable结构
{
    Q_OBJECT
public:
    explicit Base_label(int x, int y, const QString & text,
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

class Type_label : public Base_label
{

    Q_OBJECT
public:
    explicit Type_label(int x, int y, const QString & text,
                           const QString & m1,const QString &m2,
                           QWidget *parent = 0);
    static QPoint flag;//第几个标记

    static Type_label *lable1;//标记上一个label

    void  StoType(){emit toType(0);}

signals:
    void toType(int order);

private:
    virtual void mousePressEvent (QMouseEvent * ev);
    QPoint type_point;

};

class Dish_label : public Base_label
{

    Q_OBJECT
public:
    explicit Dish_label(int x, int y, const QString & text,
                           const QString & m1,const QString &m2,
                           QWidget *parent = 0);
    static QPoint flag;//第几个标记

    static Dish_label *lable1;//标记上一个label

    void  StoType(){emit toDish(0);}

signals:
    void toDish(int order);

private:
    virtual void mousePressEvent (QMouseEvent * ev);
    QPoint dish_point;

};

/*每个新建的表格都有上面的所有属性，label上显示："品码"<<"品名"<<"价格"*/
class Cookie_label : public Base_label
{
    Q_OBJECT
public:
    explicit Cookie_label(int x, int y, const QString & text,
                          const QString & m1,const QString & m2,
                        QWidget *parent = 0);

    static QPoint flag;//第几个标记
    static Cookie_label *lable1;//标记上一个label

    void  StoType(){emit tocook(0);}

signals:
    void tocook(int order);
  //  QStringList getImages();
   // void setallinfo();
private:
    virtual void mousePressEvent (QMouseEvent * ev);
    //virtual void mouseReleaseEvent (QMouseEvent * e);
//    void Tabpp(int f);
//    void singleApp(int f,QString choice = QString());
//    QString show_Cruprice_VIew();
//    int findItem(const QString &no);
//    QString get_special_price(const QString &no);
//    int p_sum,p_row,cur_sum;
//    int row;
//    QString cur_price;
//    QString main_suitno;
//    QString cur_flag;
//    QString suit_flag;
//    QMessageBox msgBox;
//    QStringList cur_shijia;
};

#endif // DISHES_LABEL_H
