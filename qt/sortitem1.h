#ifndef SORTITEM1_H
#define SORTITEM1_H
#include <QWidget>

class sortItem1: public QWidget
{
    Q_OBJECT
public:
    explicit sortItem1(QString kindId,QString name,bool select,QWidget *parent = 0);

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
#endif // SORTITEM1_H
