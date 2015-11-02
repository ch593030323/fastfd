#ifndef LDS_TREEWIDGET_H
#define LDS_TREEWIDGET_H

#include <QTreeWidget>
#include <QStack>
//0 一级
    //00 二级
    //01
        //010 三级
        //011
            //。。。
    //02
#include "lds_treewidgetitem.h"
class lds_treeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit lds_treeWidget(QWidget *parent = 0);
    void restore();
    lds_treeWidgetItem *rootitem;
    int deepofitem(QTreeWidgetItem *item);

    void set_0_Index(int index);//特指深度为0 的对应的index的位置即0index
    int get_0_Index();//特指深度为0 的对应的index的位置即0index

    void deleteAllItem();

    void select_beforerow();
    int get_beforerow();
private slots:
    void savedeep();
protected:
    void mousePressEvent(QMouseEvent *event);
private:
    void setcurdeep();
    QVector<int> curdeep(){return treedeepvect;}
    QVector<int> treedeepvect;
    QStack<int> stack_row;

};

#endif // LDS_TREEWIDGET_H
