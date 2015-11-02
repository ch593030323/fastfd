#ifndef LDS_BUTTON_TIPTEXT_H
#define LDS_BUTTON_TIPTEXT_H

#include <QPushButton>
class QLineEdit;
class lds_menu;
class QLabel;
class lds_button_tiptext : public QPushButton
{
    Q_OBJECT
public:
    explicit lds_button_tiptext(QWidget *parent = 0);

    void setLabelText(const QString &text);
private:
    QLabel *action;
};

#endif // LDS_BUTTON_TIPTEXT_H
