#ifndef GOODITEM_H
#define GOODITEM_H

#include <QWidget>

class GoodItem : public QWidget
{
    Q_OBJECT
public:
    explicit GoodItem(QString barcode,QString name,QWidget *parent = 0);
    
    void setName(QString name);
    void mousePressEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void setSelect(bool bo);
signals:
    void touch(QString);
public:
    QString _barcode;
    bool _isSelect;
    QRect _rect;
    QString _name;
};

#endif // GOODITEM_H
