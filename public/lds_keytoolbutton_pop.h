#ifndef LDS_KEYTOOLBUTTON_POP_H
#define LDS_KEYTOOLBUTTON_POP_H

#include <QWidget>
#include "lds_roundedwidget.h"

namespace Ui {
class lds_keytoolbutton_pop;
}

class lds_keytoolbutton_pop : public lds_roundedwidget
{
    Q_OBJECT
public:
    explicit lds_keytoolbutton_pop(QWidget *parent = 0);
    ~lds_keytoolbutton_pop();
signals:
    void signal_click(const QString &value);
private slots:
    void tocenter();
private:
    Ui::lds_keytoolbutton_pop *ui;
};

#endif // LDS_KEYTOOLBUTTON_POP_H
