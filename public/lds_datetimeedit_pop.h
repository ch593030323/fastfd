#ifndef LDS_DATETIMEEDIT_POP_H
#define LDS_DATETIMEEDIT_POP_H

#include <QFrame>
#include <QDateTime>
#include "lds_roundedwidget.h"

namespace Ui {
class lds_datetimeEdit_pop;
}
class QDateTimeEdit;
class lds_datetimeEdit_pop : public lds_roundedwidget
{
    Q_OBJECT

public:
    explicit lds_datetimeEdit_pop(QDateTimeEdit *parent = 0);
    ~lds_datetimeEdit_pop();
    void setDateTime(const QDateTime &dt);
    void setDate(const QDate &dt);
    QDateTime getDateTime();
signals:
    void dtchanged(const QDateTime &dt);
private slots:

    void setTime(const QTime &time);

    void on_pushButton_backtoday_clicked();

    void on_calendarWidget_clicked(const QDate &date);

    void on_comboBox_month_currentIndexChanged(const QString &arg1);

    void on_comboBox_year_currentIndexChanged(const QString &arg1);

    void on_dateEdit_dateChanged(const QDate &date);

    void on_pushButton_clicked();

private:
    Ui::lds_datetimeEdit_pop *ui;
    QDateTime _dt;
    QDateTimeEdit *_parent;
};

#endif // LDS_DATETIMEEDIT_POP_H
