#ifndef SORTITEM_H
#define SORTITEM_H

#include <QWidget>

class SortItem : public QWidget
{
    Q_OBJECT
public:
    explicit SortItem(QString kindId,QString name,bool select,QWidget *parent = 0);
    
    void mousePressEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void setSelect(bool bo);
    void setRect(QRect rect);
    void setName(QString name);
signals:
    void touch(QString);
public:
    QString _kindId;
    QString _name;
    bool _isSelect;
    QRect _rect;
    bool _hasInit;
};

#endif // SORTITEM_H
