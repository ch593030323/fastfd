#ifndef SORTITEM_RESTAURANT_H
#define SORTITEM_RESTAURANT_H

#include <QWidget>

class sortitem_restaurant : public QWidget
{
    Q_OBJECT
public:
    explicit sortitem_restaurant(QString kindId,QString name,bool select,QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void setSelect(bool bo);
    void setRect(QRect rect);
    void setName(QString name);
signals:
    void touch_restaurant(QString);
public:
    QString _kindId;
    QString _name;
    bool _isSelect;
    QRect _rect;
    bool _hasInit;
    
signals:
    
public slots:
    
};

#endif // SORTITEM_RESTAURANT_H
