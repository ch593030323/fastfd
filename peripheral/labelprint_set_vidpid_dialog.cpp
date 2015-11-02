#include "labelprint_set_vidpid_dialog.h"
#include "ui_labelprint_set_vidpid_dialog.h"
#include"commonheader.h"
#include "printer_pos_vidpid.h"
#include "uinormal_message.h"
#include <QPainter>
#include <QFile>

labelprint_set_vidpid_dialog::labelprint_set_vidpid_dialog(labelprint_set_dialog *parent) :
    QDialog(parent),
    ui(new Ui::labelprint_set_vidpid_dialog),
    _parent(parent)
{
    ui->setupUi(this);setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);this->setLayoutDirection(CommonHeader::com_widget_dir);
    //1
    ui->tableView->setModel(&CommonHeader::creat_print_vidpidmodel());
    ui->tableView->setEditTriggers(QTableView::SelectedClicked);
    ui->tableView->setItemDelegate(new labelprint_set_vidpid_delegate(this));
    ui->tableView->setColumnWidth(1, 140);
    ui->tableView->verticalHeader()->setDefaultSectionSize(50);

    //2
    QStandardItemModel &model=CommonHeader::creat_print_vidpidmodel();
    QFile file(CommonHeader::com_root_dirpath+"/"+"vidpid.save");
    if(file.open(QFile::ReadOnly)) {
        model.removeRows(0, model.rowCount());
        QByteArray ba = file.readAll();
        QDataStream out(&ba, QIODevice::ReadOnly);
        while(!out.atEnd()){
            QList<QStandardItem *> items;
            QString p1, p2, p3, p4;
            out >> p1 >> p2 >> p3 >> p4;
            items <<new QStandardItem(p1);
            items <<new QStandardItem(p2);
            items <<new QStandardItem(p3);
            items <<new QStandardItem(p4);
            model.appendRow(items);
        }
    }
    file.close();
    //3
}

labelprint_set_vidpid_dialog::~labelprint_set_vidpid_dialog()
{
    delete ui;
}

void labelprint_set_vidpid_dialog::on_pushButton_add_clicked()
{
    QStandardItemModel &model=CommonHeader::creat_print_vidpidmodel();
    model.appendRow(
                QList<QStandardItem*>()
                <<new QStandardItem("")
                <<new QStandardItem("")
                <<new QStandardItem("")
                <<new QStandardItem("")
                );
    model.setData(model.index(model.rowCount()-1, 0), QString("p%1").arg(model.rowCount()));
}

void labelprint_set_vidpid_dialog::on_pushButton_del_clicked()
{
    int row=ui->tableView->currentIndex().row();
    if(row<0){
        uinormal_message::warning(this,tr("标签机vidpid设置"),tr("没有任何打印机选中"));
        return;
    }
    QStandardItemModel &model=CommonHeader::creat_print_vidpidmodel();

    model.removeRow(row);
}

void labelprint_set_vidpid_dialog::on_pushButton_test_clicked()
{
    int row=ui->tableView->currentIndex().row();
    if(row<0){
        uinormal_message::warning(this,tr("标签机vidpid设置"),tr("没有任何打印机选中"));
        return;
    }
    QAbstractItemModel *model=ui->tableView->model();
    _parent->ui->comboBox_labelprint_printpath->setEditText(model->index(row, 2).data().toString());
    _parent->ui->comboBox_labelprint_printtype->setCurrentIndex(
                CommonHeader::creat_printtypes().indexOf(model->index(row, 1).data().toString()));
    _parent->ui->pushButton_labelprint_test->animateClick();
}

void labelprint_set_vidpid_dialog::on_pushButton_app_clicked()
{
    int row=ui->tableView->currentIndex().row();
    if(row<0){
        uinormal_message::warning(this,tr("标签机vidpid设置"),tr("没有任何打印机选中"));
        return;
    }
    QAbstractItemModel *model=ui->tableView->model();
    _parent->ui->comboBox_labelprint_printpath->setEditText(model->index(row, 2).data().toString());
    _parent->ui->comboBox_labelprint_printtype->setCurrentIndex(
                CommonHeader::creat_printtypes().indexOf(model->index(row, 1).data().toString()));

}

void labelprint_set_vidpid_dialog::on_pushButton_exit_clicked()
{
    //1
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    QStandardItemModel &model=CommonHeader::creat_print_vidpidmodel();
    QFile file(CommonHeader::com_root_dirpath+"/"+"vidpid.save");
    for(int r=0,rcount=model.rowCount();r<rcount;r++){
        out << model.item(r, 0)->text();
        out << model.item(r, 1)->text();
        out << model.item(r, 2)->text();
        out << model.item(r, 3)->text();
    }
    if(!file.open(QFile::WriteOnly)) {
        uinormal_message::warning(this, tr("标签机vidpid设置"), file.errorString());
        return;
    }
    file.write(ba);
    file.close();
    //2
    this->reject();
}


QWidget *labelprint_set_vidpid_delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int c = index.column();
    if(c == 1){
        QComboBox *editor = new QComboBox(parent);
        editor->addItems(CommonHeader::creat_printtypes());
        return editor;
    } else {
        return QItemDelegate::createEditor(parent, option, index);
    }
}

void labelprint_set_vidpid_delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int c = index.column();
    if(c == 1){
        QString value = index.model()->data(index, Qt::EditRole).toString();

        QComboBox *spinBox = static_cast<QComboBox*>(editor);
        spinBox->setCurrentIndex(CommonHeader::creat_printtypes().indexOf(value));
    } else {
        QItemDelegate::setEditorData(editor, index);
    }
}

void labelprint_set_vidpid_delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    int c = index.column();
    if(c == 1){
        QComboBox *spinBox = static_cast<QComboBox*>(editor);
        model->setData(index, spinBox->currentText(), Qt::EditRole);
    } else {
        QItemDelegate::setModelData(editor, model, index);
    }
}

void labelprint_set_vidpid_delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int c = index.column();
    if(c == 1){
        editor->setGeometry(option.rect);
    } else {
        QItemDelegate::updateEditorGeometry(editor, option, index);
    }
}

void labelprint_set_vidpid_delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int  c = index.column();
    if(c == 1){
        QString value = index.model()->data(index, Qt::EditRole).toString();
        QRect x = option.rect;
        int flag = Qt::AlignCenter | Qt::TextWordWrap;
        painter->drawText(x, flag, value);

    } else {
        QItemDelegate::paint(painter,option,index);
    }
}
