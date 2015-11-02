#ifndef LDS_LINEEDIT_SELECTEDALL_H
#define LDS_LINEEDIT_SELECTEDALL_H

#include <QLineEdit>





//获取焦点时，选中所有内容





class lds_lineedit_selectedall : public QLineEdit
{
    Q_OBJECT
public:
    explicit lds_lineedit_selectedall(QWidget *parent = 0);

    void focusInEvent(QFocusEvent *e);
signals:
    void getfocus();

};

#endif // LDS_LINEEDIT_SELECTEDALL_H
