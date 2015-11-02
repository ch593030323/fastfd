#ifndef BACK_VIEW_2_BOTTON_H
#define BACK_VIEW_2_BOTTON_H

#include <QToolButton>

class back_view_2_botton : public QToolButton
{
public:
    back_view_2_botton(QWidget *parent = 0);
    static QList<QToolButton *> toollist;
protected:
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void paintEvent(QPaintEvent *e);
    bool isfocus;
};

#endif // BACK_VIEW_2_BOTTON_H
