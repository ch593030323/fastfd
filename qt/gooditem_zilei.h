#ifndef GOODITEM_ZILEI_H
#define GOODITEM_ZILEI_H

#include <QWidget>

class goodItem_zilei : public QWidget
{
    Q_OBJECT
public:
    explicit goodItem_zilei(QString barcode,QString name,QWidget *parent = 0);
    void setName(QString name);
    void mousePressEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void setSelect(bool bo);
signals:
    void touch_zilei(QString);
public:
    QString _barcode;
    bool _isSelect;
    QRect _rect;
    QString _name;
    
};

#endif // GOODITEM_ZILEI_H
