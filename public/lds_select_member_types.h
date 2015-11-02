#ifndef LDS_SELECT_MEMBER_TYPES_H
#define LDS_SELECT_MEMBER_TYPES_H

#include <QWidget>
class QLineEdit;
class lds_menu;

class lds_Select_member_types : public QWidget
{
    Q_OBJECT
public:
    explicit lds_Select_member_types(QWidget *parent = 0);
    QString gettext();
    void settext(const QString & text);
private slots:
    void show_member_types();
signals:
    void okstr1(const QString &text1);
private:
    QLineEdit *line;
};

#include <QDialog>
#include "lds_model_sqlrelationaltablemodel.h"

class Ui_selectgoods_Widget;
class lds_model_sqlrelationaltablemodel;
class Select_member_types:public QDialog{
    Q_OBJECT
public:
    class RelationalTableModel:public lds_model_sqlrelationaltablemodel{
    public:
        RelationalTableModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase()):lds_model_sqlrelationaltablemodel(parent, db){}
        QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;

    };
public:
    explicit Select_member_types(QWidget *parent = 0);
signals:
    void okstr(const QString &text);
private slots:
    void toselect(const QString &filter=QString());//tableview刷新

    void took();
private:
    Ui_selectgoods_Widget *ui;
    lds_model_sqlrelationaltablemodel *tablemodel;
    QString sql_select;
    QString _item_flag;
    QString _check_sheetno;
};

#endif // LDS_SELECT_MEMBER_TYPES_H
