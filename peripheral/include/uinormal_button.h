#ifndef UINORMAL_BUTTON_H
#define UINORMAL_BUTTON_H

#include <QPushButton>

class uinormal_button : public QPushButton
{
    Q_OBJECT
public:
    explicit uinormal_button(QWidget *parent = 0);

    QSize sizeHint() const;
signals:

public slots:

};

#endif // UINORMAL_BUTTON_H
