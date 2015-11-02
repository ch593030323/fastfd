#ifndef LDS_KEYTOOLBUTTON_H
#define LDS_KEYTOOLBUTTON_H

#include <QToolButton>
class QLineEdit;
class lds_keytoolbutton : public QToolButton
{
    Q_OBJECT
public:
    explicit lds_keytoolbutton(QWidget *parent = 0);
private slots:
    void tocenter(const QString &value);
    void showpop();
    void showpop_time();
    void closepop(QObject *w);
private:
    QLineEdit*parentl;
};

#endif // LDS_KEYTOOLBUTTON_H
