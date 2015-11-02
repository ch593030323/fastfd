#include "lds_tablewidget.h"
#include <QDebug>
#include <QIcon>

lds_tableWidget::lds_tableWidget(QWidget *parent) :
    QTableWidget(parent)
{
    delegate=new ItemDelegate(this);
    setItemDelegate(delegate);
    setStyleSheet(QString("QTableWidget{border:%1px solid %2;}").arg(delegate->gridwidth/2).arg(delegate->gridcolor));
}


lds_tableWidget::ItemDelegate::ItemDelegate(QWidget *parent):
    QStyledItemDelegate(parent),

    borderwidth(2),//必须为偶数
    focusbordercolor("black"),
    notfocusbordercolor("gray"),

    margin(5),

    gridwidth(2),
    gridcolor("white"),

    _parent(static_cast<QTableWidget *>(parent)){}

void lds_tableWidget::ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //       QStyledItemDelegate::paint(painter, option, index);
    int r=index.row();
    int c=index.column();
    QPixmap pixmap=qvariant_cast<QIcon>(index.data(Qt::DecorationRole)).pixmap(option.decorationSize);//icon
    QString text=qvariant_cast<QString>(index.data());//text
    QBrush backgroundcolor=qvariant_cast<QBrush>(index.data(Qt::BackgroundRole));//背景色
    QFont font=qvariant_cast<QFont>(index.data(Qt::FontRole));//字体大小
    QColor textcolor=qvariant_cast<QColor>(index.data(Qt::TextColorRole));//文本颜色
    int rspan=_parent->rowSpan(r, c);//行span
    int cspan=_parent->columnSpan(r, c);//列span

    painter->save();
    QRect rect=option.rect;
    painter->fillRect(rect,QColor(gridcolor));
    rect.setTopLeft(rect.topLeft()+QPoint(gridwidth/2, gridwidth/2));
    rect.setBottomRight(rect.bottomRight()-QPoint(gridwidth/2,gridwidth/2));

    //设置边框
    QColor bordercolor=QColor("transparent");
    if(option.state&QStyle::State_Selected){
        //若有焦点
        if(option.state&QStyle::State_HasFocus){
            bordercolor = focusbordercolor;
        } else {//若没有焦点
            bordercolor = notfocusbordercolor;
        }
    }
    painter->fillRect(rect, QColor(bordercolor));
    rect.setTopLeft(rect.topLeft()+QPoint(borderwidth, borderwidth));
    rect.setBottomRight(rect.bottomRight()-QPoint(borderwidth,borderwidth));

    painter->fillRect(rect, backgroundcolor);
    //span后的 rect
    rect=QRect(rect.topLeft(),QSize(rect.width(),
                                    (rect.height()-(rspan-1)*(gridwidth+(option.state&QStyle::State_Selected?1:0)*2*borderwidth))/rspan));

    //内容如下
    //item设置的字体大小
    painter->setFont(font);
    //item设置的字体颜色
    painter->setPen(textcolor);

    //item设置的icon
    QPoint ppoint=rect.topLeft()+QPoint(margin, (rect.height()-pixmap.height())/2);
    painter->drawPixmap(ppoint,pixmap);

    //item设置的text
    rect.setTopLeft(rect.topLeft() + QPoint(margin*3+option.decorationSize.width(), margin));
    rect.setBottomRight(rect.bottomRight() -QPoint(margin, margin));
    painter->drawText(rect, int(Qt::AlignLeft|Qt::AlignVCenter|Qt::TextWrapAnywhere),text);

    painter->restore();
}
