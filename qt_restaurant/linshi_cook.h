#ifndef LINSHI_COOK_H
#define LINSHI_COOK_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include "gooditem_zilei.h"
#include <QPointer>
#include <QGridLayout>

namespace Ui {
class linshi_cook;
}

class linshi_cook : public QDialog
{
    Q_OBJECT
    
public:
    explicit linshi_cook(QWidget *parent = 0);
    ~linshi_cook();
    void init_control();
    QString no;
    bool btn_t_f;
private slots:
    bool showGoods_zilei(QString kindId, int index = 0);
    void dis_xiaolei_str(const QString & barcode);
    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::linshi_cook *ui;
    QSqlQueryModel *model_xiaolei;
    QStringList first_in1;
    QList<goodItem_zilei*> itemList_zilei;
    QPointer<goodItem_zilei> currentGoodItem_zilei;
    int item_count_zilei;
    int item_count_row_zilei;
    QGridLayout *layout_zilei;
    int goodIndex_xiaolei;
    QString goodKind_xiaolei;
};

#endif // LINSHI_COOK_H
