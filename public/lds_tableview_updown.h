#ifndef LDS_TABLEVIEW_UPDOWN_H
#define LDS_TABLEVIEW_UPDOWN_H

#include "lds_tableview.h"
#include <QPushButton>

class lds_tableView_updown : public QWidget
{
    Q_OBJECT
public:
    explicit lds_tableView_updown(QWidget *parent = 0);
    lds_tableView *tableview;
private slots:
    void todown();
    void toup();
    void toisenabled(int value);
private:
    QPushButton *up;
    QPushButton *down;
};

#endif // LDS_TABLEVIEW_UPDOWN_H
