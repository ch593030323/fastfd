#ifndef LDS_DATETIMEEDIT_H
#define LDS_DATETIMEEDIT_H


#include <QDateTimeEdit>
class lds_datetimeEdit_pop;
class lds_datetimeEdit : public QDateTimeEdit
{
    Q_OBJECT
public:
    lds_datetimeEdit(QWidget *parent=0);
    ~lds_datetimeEdit();
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    lds_datetimeEdit_pop *popwidget;bool subcontrol_ismouseover;
};

#endif // LDS_DATETIMEEDIT_H
