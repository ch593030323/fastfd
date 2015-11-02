#ifndef KINDINDEXWIDGE_RES_H
#define KINDINDEXWIDGE_RES_H

#include <QWidget>
#include "sortitem_restaurant.h"
#include <QPointer>
#include <QHBoxLayout>

class kindindexwidge_res : public QWidget
{
    Q_OBJECT
public:
    explicit kindindexwidge_res(QWidget *parent = 0);
    ~kindindexwidge_res();

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
    QPointer<sortitem_restaurant> _lastItem;
    QHBoxLayout *horizontalLayout;
private:
    QStringList kindList;
    QStringList nameList;
    QList<sortitem_restaurant*> itemList;
    
};

#endif // KINDINDEXWIDGE_RES_H
