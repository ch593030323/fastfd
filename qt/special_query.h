#ifndef SPECIAL_QUERY_H
#define SPECIAL_QUERY_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class special_query;
}

class special_query : public QDialog
{
    Q_OBJECT
    
public:
    explicit special_query(QWidget *parent = 0);
    ~special_query();
    void init_control();
private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::special_query *ui;
    QStandardItemModel *model;
};

#endif // SPECIAL_QUERY_H
