#ifndef PRE_AUTOEXPAND_3_FORM_H
#define PRE_AUTOEXPAND_3_FORM_H

#include <QWidget>
#include "frontviewdialog_autoexpand.h"

namespace Ui {
class pre_autoexpand_3_Form;
}

class pre_autoexpand_3_Form : public QWidget
{
    Q_OBJECT

public:
    explicit pre_autoexpand_3_Form(QWidget *parent = 0);
    ~pre_autoexpand_3_Form();
    void to_grade0(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list);
    void to_grade1(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list);
    void to_grade2(const QList<frontViewDialog_autoexpand::content_item_icon> &item_icon_list);

    void widget3_setsql(const  QString &sql);
    void widget3_start();
    void widget3_item_event_click();
    void widget3_tofresh();
    int getframe2_h();

    void addGoods(const QString &barcode,
                  const  bool &bianma_daicai_type,
                  const  bool &changyongcook,
                  const QString &bianma_str,
                  const QString &senderclassname,
                  bool &barscaleflag,
                  bool &isweight_flag,
                  QString &pay_barcode,//条码需要修改
                  int &row_n//对应行数需要修改
                  );//点菜
    void paybill();//付款
signals:
    void signal1_text(const QString &text);
    void signal2_text(const QString &text);
    void signal3_text(const QString &text);
    void signal3_refresh();//widget3需要厨打
private:
    Ui::pre_autoexpand_3_Form *ui;
};

#endif // PRE_AUTOEXPAND_3_FORM_H
