#ifndef lds_toolbar_H
#define lds_toolbar_H

#include <QWidget>
#include <QToolButton>
#include <QMap>
#include <QLineEdit>
#include <QTime>
#include "lds_selectgoods.h"



//多个toolbutton的布局





class lds_toolbar : public QFrame
{
    Q_OBJECT
public:
    explicit lds_toolbar(QWidget *parent = 0);
    void setup(const QStringList &names,Qt::Alignment flag=Qt::AlignLeft);
    QAbstractButton *index_widget(const QString &name);
    void setselectgoodsenable_itemflag(const QString &name);//这种方法目前只是适用于一个按钮
    void setselectgoodsenable_check_sheetno(const QString &name);
signals:
    void okstrs(const QStringList &text);//这种方法目前只是适用于一个按钮
private slots:
    void labelclear();
    void starttime();
    void updatetime();
private:
    void layoutaddwidget(QLayout *layout, const QStringList &names, Qt::Alignment flag);
private:
    QMap<QString,QAbstractButton *> nameTool;
    QLabel *label;
    QTime *time;
};

#endif // lds_toolbar_H
