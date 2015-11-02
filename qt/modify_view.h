#ifndef MODIFY_VIEW_H
#define MODIFY_VIEW_H

#include <QDialog>

namespace Ui {
class modify_view;
}

class modify_view : public QDialog
{
    Q_OBJECT
    
public:
    explicit modify_view(const QString &str,QWidget *parent = 0);
    ~modify_view();
    bool btn_t_f;
    QString jiage_m;
private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::modify_view *ui;
    QString _str;
};

#endif // MODIFY_VIEW_H
