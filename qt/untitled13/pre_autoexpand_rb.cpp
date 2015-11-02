#include "pre_autoexpand_rb.h"
#include <QPaintEvent>
#include <QStylePainter>
#include <QStyleOptionButton>
#include <QtDebug>
int pre_autoexpand_rb::border;
QSize pre_autoexpand_rb::iconsize;
int pre_autoexpand_rb::border0=10;
int pre_autoexpand_rb::textheight=35;
QSize pre_autoexpand_rb::iconsize0=QSize(120,120);


static QSize getsizehint(){
    return pre_autoexpand_rb::iconsize+
            QSize(pre_autoexpand_rb::border,pre_autoexpand_rb::border+pre_autoexpand_rb::textheight);
}

pre_autoexpand_rb::pre_autoexpand_rb(bool isshowpixmap,QWidget *parent) :
    QRadioButton(parent)
{
    setshowpixmapEnabel(isshowpixmap);
    this->setIconSize(iconsize);
    this->setStyleSheet(
                "QRadioButton:indicator{width:0px;background:transparent;}"
                );
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    this->setFocusPolicy(Qt::NoFocus);
}

void pre_autoexpand_rb::setText(const QString &text)
{
    _text=text;
}

QString pre_autoexpand_rb::text() const
{
    return _text;
}

void pre_autoexpand_rb::setshowpixmapEnabel(bool enabled)
{
    if(enabled){
        iconsize=iconsize0;
        border=border0;
    } else {
        iconsize=QSize(iconsize0.width(),0);
        border=0;
    }
}

QSize pre_autoexpand_rb::sizeHint() const
{
    return getsizehint();
}

void pre_autoexpand_rb::paintEvent(QPaintEvent *e)
{
    QStylePainter p(this);
    QStyleOptionButton opt;
    initStyleOption(&opt);
    //保存
//    border-radius:3px; border:2px solid #dadadb;background:#f3f5f5; font:14pt;color:#7b7878;
    QPixmap pixmap=opt.icon.pixmap(opt.iconSize);
    QRect parentrect(QPoint(0,0), this->sizeHint()-QSize(0,textheight));
    QRect imagerect=QStyle::alignedRect(this->layoutDirection(), Qt::AlignCenter,iconsize,parentrect);
    QRect textrect(QRect(0,parentrect.height()/*+border/2*/,parentrect.width(),
                         this->height()-(parentrect.height()/*+border/2*/)));
    //清除
    opt.icon=QIcon();
    //绘制
    p.drawControl(QStyle::CE_RadioButton, opt);
    //重写
    p.fillRect(parentrect, QColor("#e7edf3"));p.setPen(QColor("#dbd8d9"));p.drawRect(parentrect.adjusted(0,0,0,-1));
    p.fillRect(textrect, QColor("#e7edf3"));p.drawRect(textrect.adjusted(0,0,0,-1));
    p.drawPixmap(imagerect, pixmap);
    if(opt.state & QStyle::State_On){
        p.setPen(Qt::red);
        QFont f=this->font();
        f.setBold(true);
        p.setFont(f);
    } else {
        p.setPen(QColor("black"));
    }
    p.drawText(textrect,int(Qt::AlignCenter|Qt::TextWordWrap), _text);
}

pre_autoexpand_label::pre_autoexpand_label(QWidget *parent):
    QLabel(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    this->setFocusPolicy(Qt::NoFocus);
}

QSize pre_autoexpand_label::sizeHint() const
{
    return getsizehint();
}
