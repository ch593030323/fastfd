#ifndef W_M_MEMBER_PARAMSET_H
#define W_M_MEMBER_PARAMSET_H

#include <QDialog>
class Ui_w_m_member_paramset_Dialog;
class w_m_member_paramset : public QDialog
{
    Q_OBJECT
public:
    explicit w_m_member_paramset(QWidget *parent = 0);
    
private slots:
    void ue_get1();
    QString ue_set1();

    void ue_get2();
    QString ue_set2();

    void toisrecharge(bool flag);
    void inputmemberno(const QString &text);

    void took();
    void tocancel();
private:
    Ui_w_m_member_paramset_Dialog *ui;
};

#endif // W_M_MEMBER_PARAMSET_H
