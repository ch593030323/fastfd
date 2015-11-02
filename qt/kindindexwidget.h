#ifndef KINDINDEXWIDGET_H
#define KINDINDEXWIDGET_H

#include <QWidget>
#include "sortitem.h"
#include <QPointer>
#include <QHBoxLayout>
class KindIndexWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit KindIndexWidget(QWidget *parent = 0);
    ~KindIndexWidget();

    void clear();
    void addSort(QString kind,QString name);
    void showSort();
    void clearSelected();
private:
    void preSort();
    void nextSort();
signals:
    void kindSelect(QString kindId);
    void putongcai(QString kind);
public slots:
    void itemClicked(QString kindId);
private:
    int index;
    QPointer<SortItem> _lastItem;
    QHBoxLayout *horizontalLayout;
private:
    QStringList kindList;
    QStringList nameList;
    QList<SortItem*> itemList;
};

#endif // KINDINDEXWIDGET_H
