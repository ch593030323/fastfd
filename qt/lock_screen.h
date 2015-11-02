#ifndef LOCK_SCREEN_H
#define LOCK_SCREEN_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class lock_screen;
}

class lock_screen : public QDialog
{
    Q_OBJECT
    
public:
    explicit lock_screen(const QString &str,QWidget *parent = 0);
    ~lock_screen();
    
private slots:
    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_11_clicked();

protected:
    void closeEvent(QCloseEvent *e);
private:
    Ui::lock_screen *ui;
    QString type;
};

#endif // LOCK_SCREEN_H
