#ifndef LDS_CHECKBOX_H
#define LDS_CHECKBOX_H

#include <QCheckBox>




//数据库mapper时，需要使用





class lds_CheckBox : public QCheckBox
{
    Q_OBJECT
    Q_PROPERTY(QString checked1 READ isChecked1 WRITE setChecked1 USER true)

public:
    explicit lds_CheckBox(QWidget *parent = 0);
    void setChecked1(QString c);
    QString isChecked1();

};

#endif // LDS_CHECKBOX_H
