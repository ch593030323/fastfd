#include "lds_tableview_sum.h"
#include <QTableView>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QDebug>
#include <QHeaderView>
#include <QStandardItemModel>
#include "lds_tableview.h"
lds_tableview_sum::lds_tableview_sum(QWidget *parent) :
    QWidget(parent)
{
    //tableView
    tableView = new lds_tableView(this);
    tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //lineEdit
    lineView = new QTableView(this);
    lineView->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    lineView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    lineView->horizontalHeader()->hide();
    lineView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lineView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lineView->setFixedHeight(30);
    lineView->setShowGrid(false);

    //layout
    QGridLayout *gridLayout = new QGridLayout();
    QVBoxLayout *verticalLayout = new QVBoxLayout();
    QHBoxLayout *hblayout=new QHBoxLayout;
    verticalLayout->addWidget(tableView);
    verticalLayout->addWidget(lineView);
    verticalLayout->addLayout(hblayout);
    gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);
    gridLayout->addWidget(tableView->verticalScrollBar(), 0, 1, 1, 1);
    gridLayout->addWidget(tableView->horizontalScrollBar(), 1, 0, 1, 1);

    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    hblayout->setSpacing(0);
    hblayout->setContentsMargins(0,0,0,0);
    setLayout(gridLayout);

    //bar
    tableView->horizontalScrollBar()->hide();
    tableView->verticalScrollBar()->hide();
    //lineView
    linemodel=new RedModel;
    lineView->setModel(linemodel);
    lineView->verticalHeader()->setModel(linemodel);
    lineView->setItemDelegate(new ItemDelegate(lineView));

    //这里是 两个table的同步section 宽度
    connect(tableView->horizontalHeader(),SIGNAL(sectionResized(int,int,int)),this,SLOT(update_lineview_header(int,int,int)),Qt::QueuedConnection);
    //这里是 两个table的水平滚动条 同步
    connect(tableView->horizontalScrollBar(),SIGNAL(valueChanged(int)),lineView->horizontalScrollBar(),SLOT(setValue(int)),Qt::QueuedConnection);
    //这里是 水平滚动条与数据内容同步
    connect(tableView->horizontalScrollBar(),SIGNAL(rangeChanged(int,int)),this,SLOT(update_tableview_bar(int,int)),Qt::QueuedConnection);
    //这里是 垂直滚动条与数据内容同步
    connect(tableView->verticalScrollBar(),SIGNAL(rangeChanged(int,int)),this,SLOT(update_tableview_bar(int,int)),Qt::QueuedConnection);
    connect(tableView->horizontalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(scrollbarinfo(int)));
}

void lds_tableview_sum::setModel(QAbstractItemModel *model)
{
    //table setmodel
    parentmodel=model;
    tableView->setModel(model);
    //QItemSelectionModel
    QItemSelectionModel *selectionModel;
    if(tableView->selectionModel()){
        selectionModel=tableView->selectionModel();
    } else {
        selectionModel=new QItemSelectionModel(parentmodel);
        tableView->setSelectionModel(selectionModel);
    }
    //同步选区的
    //更新lineview的垂直header宽度
    connect(model,SIGNAL(rowsInserted(QModelIndex, int, int)),this,SLOT(update_tableview_verticalheader()));
    connect(model,SIGNAL(rowsRemoved(QModelIndex, int, int)),this,SLOT(update_tableview_verticalheader()));

    //更新lineview的数据，只保留一行
    linemodel->removeRows(0, linemodel->rowCount());
    QList<QStandardItem *> items;
    for(int c=0,c_count=parentmodel->columnCount();c<c_count;c++){
        QStandardItem *item=new QStandardItem("");
        if(c==0){
            item->setText(tr("合计"));
        }
        items<<item;
    }
    linemodel->appendRow(items);
    lineView->verticalHeader()->setHidden(!parentmodel->rowCount());

}

void lds_tableview_sum::update_tableview_bar( int min, int max)
{
    //更新两个滚动条显示情况
    Q_UNUSED(min)
    QScrollBar *bar=qobject_cast<QScrollBar *>(sender());
    if(!bar) return;
    bar->setVisible(max>0);
}

void lds_tableview_sum::update_lineview_header(int logicalIndex, int oldSize, int newSize)
{
    //同步sectionwidth
    Q_UNUSED(oldSize)
    lineView->horizontalHeader()->resizeSection(logicalIndex, newSize);
}

QString lds_tableview_sum::updateSum(int column, const QString &value)
{
    //更新lineview的数据，只保留一行
    if(linemodel->rowCount()<=0
            ||linemodel->columnCount()<=column) {
        qDebug() << tr("lds_tableview_sum::updateSum:超出范围");
        return "";
    }

    int vitual_column=column;
    int actual_column=0;
    for(int c=0,c_count=tableView->horizontalHeader()->count();c<c_count;c++){
        if(vitual_column<=0)break;
        actual_column++;
        if(tableView->isColumnHidden(c)){
        } else {
            vitual_column--;
        }
    }

    qDebug() << value;
    linemodel->item(0, actual_column)->setText(value);
    return linemodel->item(0, actual_column)->text();

}

void lds_tableview_sum::setlineAlignment(Qt::AlignmentFlag flag)
{
    linemodel->lineflag=flag;
    linerefresh();
}

Qt::AlignmentFlag lds_tableview_sum::LineAlignment()
{
    return linemodel->lineflag;
}

void lds_tableview_sum::setEditTriggers(QAbstractItemView::EditTriggers triggers)
{
    tableView->setEditTriggers(triggers);
}

QString lds_tableview_sum::updateSum(int column)
{
    //更新lineview的数据，只保留一行
    if(linemodel->rowCount()<=0
            ||linemodel->columnCount()<=column) {
        qDebug() << tr("lds_tableview_sum::updateSum:超出范围")<<linemodel->columnCount() << column;
        return "";
    }

    double f_sum=0.0;
    for(int r=0,r_count=parentmodel->rowCount();r<r_count;r++){
        f_sum+=parentmodel->index(r, column).data().toFloat();
    }

    linemodel->item(0, column)->setText(QString().sprintf("%.02f", f_sum));
    return linemodel->item(0, column)->text();
}

void lds_tableview_sum::linerefresh()
{
    if(parentmodel->columnCount() != linemodel->columnCount()){
        //更新行数和列数
        linemodel->removeRows(0, linemodel->rowCount());
        QList<QStandardItem *> items;
        for(int c=0,c_count=parentmodel->columnCount();c<c_count;c++){
            QStandardItem *item=new QStandardItem("");
            if(c==0){
                item->setText(tr("合计"));
            }
            items<<item;
        }
        linemodel->appendRow(items);

        linemodel->setColumnCount(parentmodel->columnCount());
        linemodel->setRowCount(1);
        //更新section的宽度，所有;更新隐藏列
        for(int c=0,c_count=tableView->horizontalHeader()->count();c<c_count;c++){
            lineView->horizontalHeader()->resizeSection(c, tableView->horizontalHeader()->sectionSize(c));
            lineView->setColumnHidden(c, tableView->isColumnHidden(c));
        }
    }
}

void lds_tableview_sum::scrollbarinfo(int step)
{
}

void lds_tableview_sum::update_lineview_selection(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected)
    //有选中
    if(selected.indexes().count()>0){

        //选中最后一列
        int e=selected.indexes().last().isValid()?selected.indexes().last().column():-1;
        lineView->selectColumn(e);
        //更新水平滚动条
        lineView->horizontalScrollBar()->setValue(tableView->horizontalScrollBar()->value());
    }
}

void lds_tableview_sum::update_tableview_verticalheader()
{
    if(linemodel->columnCount() != parentmodel->columnCount()){
        linerefresh();
    }
    linemodel->setVerticalHeaderLabels(QStringList() << QString("%1").arg(parentmodel->rowCount()));
    lineView->verticalHeader()->setHidden(!parentmodel->rowCount());
}

QVariant lds_tableview_sum::RedModel::data(const QModelIndex &index, int role) const
{
    QVariant _d=QStandardItemModel::data(index,role);

    if(role==Qt::TextColorRole){
        return QColor("red");
    }
    if(role==Qt::TextAlignmentRole){
        return lineflag;
    }
    if(index.column()%2==0&&role==Qt::BackgroundColorRole){
        return QPalette().alternateBase();
    }
    if(role==Qt::DisplayRole){
        bool ok;
        float f=_d.toFloat(&ok);
        if(ok){
            return QString().sprintf("%.02f",f);
        }
    }
    return QStandardItemModel::data(index,role);
}

void lds_tableview_sum::ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPen pen;
    pen.setColor(QColor("red"));
    //默认的背景色
    QColor color_2=qvariant_cast<QColor>(index.data(Qt::BackgroundRole));
    if(color_2.isValid()){
        painter->save();
        painter->setPen(Qt::NoPen);
        painter->setBrush(color_2);
        painter->drawRect(option.rect);
        painter->restore();
    }

    //选中状态的背景色
    if(option.state & QStyle::State_Selected){
        QColor color=option.palette.highlight().color();
        painter->save();
        painter->setPen(Qt::NoPen);
        painter->setBrush(color);
        painter->drawRect(option.rect);
        painter->restore();
        pen.setColor(QColor("white"));
    }
    //绘制文字
    painter->setPen(pen);
    int flags=Qt::AlignVCenter|Qt::AlignLeft;
    painter->drawText(option.rect, flags, index.data().toString());
}


