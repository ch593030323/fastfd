#ifndef LDS_LINEEDIT_H
#define LDS_LINEEDIT_H

#include <QLineEdit>

class lds_lineedit:public QLineEdit
{
    Q_OBJECT
public:
    lds_lineedit(QWidget *parent=0);
    ~lds_lineedit();

    void set_ignore_keyevent(bool isignore);
public slots:
    void totextnotinput(const QString& text);
protected:
    void keyPressEvent(QKeyEvent *e);
private:
    bool _isignore;
};

#endif // LDS_LINEEDIT_H
