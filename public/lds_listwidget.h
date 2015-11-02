#ifndef LDS_LISTWIDGET_H
#define LDS_LISTWIDGET_H

#include <QListWidget>

class lds_listwidget : public QListWidget
{
public:
    lds_listwidget(QWidget *parent=0);
    ~lds_listwidget();

public:
    bool ismoving;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    QPoint pressp;
    int pressh,pressv;
};

#endif // LDS_LISTWIDGET_H
