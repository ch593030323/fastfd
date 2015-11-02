#ifndef BACK_VIEW_2_RADIO_H
#define BACK_VIEW_2_RADIO_H

#include <QRadioButton>

class back_view_2_radio : public QRadioButton
{
    Q_OBJECT
public:
    explicit back_view_2_radio(QWidget *parent = 0);
    static QList<QRadioButton *> rbtnlist;

signals:

public slots:

};

#endif // BACK_VIEW_2_RADIO_H
