#ifndef LDS_TIMEEDIT_H
#define LDS_TIMEEDIT_H

#include <QWidget>

namespace Ui {
class lds_timeEdit;
}
class QRadioButton;
class lds_timeEdit : public QWidget
{
    Q_OBJECT

public:
    explicit lds_timeEdit(QWidget *parent = 0);
    ~lds_timeEdit();

    void setTime(const QTime &t);

    void setShowVisiable(const QString &df);
signals:
    void timechanged(const QTime &t);
private slots:

    void tocenter();

    void on_radioButton_hour_clicked(bool checked);

    void on_radioButton_minute_clicked(bool checked);

    void on_radioButton_second_clicked(bool checked);

    void totimechanged();
private:
    Ui::lds_timeEdit *ui;
    QRadioButton *checkedrb;
};

#endif // LDS_TIMEEDIT_H
