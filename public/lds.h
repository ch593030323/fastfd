#ifndef LDS_H
#define LDS_H

#include <QWidget>

class lds
{
public:
    lds();
    static QPoint getPopPoint(QWidget *parent, QWidget *popwidget);
};

#endif // LDS_H
