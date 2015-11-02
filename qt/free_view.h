#ifndef FREE_VIEW_H
#define FREE_VIEW_H

#include <QDialog>

namespace Ui {
class free_view;
}

class free_view : public QDialog
{
    Q_OBJECT
    
public:
    explicit free_view(const QString &title,const QString & str,QWidget *parent = 0);
    ~free_view();
    bool btn_t_f;
     QString oper_id;
     float line2_num;
     QString beizhu;
     QString time;
    void init_control();
private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::free_view *ui;
    QString num;
    QString _title;
};

#endif // FREE_VIEW_H
