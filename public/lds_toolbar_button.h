#ifndef LDS_TOOLBAR_BUTTON_H
#define LDS_TOOLBAR_BUTTON_H
#include <QPushButton>

class lds_toolbar_button:public QPushButton
{
public:
    lds_toolbar_button(QWidget *parent=0);

    QSize sizeHint()const;

    void updateText(const QString &text);
};

#endif // LDS_TOOLBAR_BUTTON_H
