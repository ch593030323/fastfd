#ifndef KINDXIAOLEI_H
#define KINDXIAOLEI_H

#include <QWidget>
#include "sortitem.h"
#include <QPointer>
#include <QHBoxLayout>
#include "sortitem1.h"

class kindxiaolei : public QWidget
{
    Q_OBJECT
public:
    explicit kindxiaolei(QWidget *parent = 0);
    ~kindxiaolei();
    void clear();
    void addSort(QString kind,QString name);
    void showSort();
    void clearSelected();
    QList<sortItem1*> itemList;
    QPointer<sortItem1> _lastItem;
    QHBoxLayout *horizontalLayout;
private:
    void preSort();
    void nextSort();
signals:
    void kindxiao(QString kindId);
public slots:
    void itemClicked(QString kindId);
private:
    int index;
private:
    QStringList kindList;
    QStringList nameList;
    
};

#endif // KINDXIAOLEI_H
