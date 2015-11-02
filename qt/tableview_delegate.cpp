#include "tableview_delegate.h"
#include "fast_order.h"
#include <QPainter>
#include <QDebug>
#include "table_order.h"
#include <QSettings>

void Tableview_delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // QItemDelegate::paint(painter,option,index);
    int column = index.column();
    int r = index.row();
    //显示奉送，优惠，特价，做法
    QRect kuang =  QRect(2,option.rect.bottomLeft().y()-50,288,45);
    QRect textRect = QRect(40,option.rect.bottomLeft().y()-25,220,20);
    QRect textRect1 = QRect(220,option.rect.bottomLeft().y()-50,80,20);
    QRect textRect2 = QRect(240,option.rect.bottomLeft().y()-25,80,20);
    QRect yuan = QRect(8,option.rect.bottomLeft().y()-33,18,15);
    // QRect textRect3 = QRect(24,option.rect.bottomLeft().y()-60,200,20);
    //QRect textRect3 = QRect(340,option.rect.bottomLeft().y()-18,50,15);
    QFont font;
    font.setPointSize(13);
    painter->setFont(font);

    QSettings *configIniRead = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    QString str = configIniRead->value("system_setting/yingyemoshi").toString();
    if(str == "0" || str.isEmpty())
    {
        QRect rectUp(option.rect.topLeft().x()+5,
                     option.rect.topLeft().y(),
                     option.rect.width()-10,
                     option.rect.height()-25);

        if(r != fast_order::row_n)
        {
            if(column == 0)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(QColor("#FFFFFF"));
                painter->save();
                painter->drawRoundedRect(kuang,5,5);
                painter->restore();
            }

            if(column == 1)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(QColor("#CCCCCC"));
                painter->save();
                painter->drawEllipse(option.rect.center(),15,15);
                painter->restore();
            }

            if(column == 2)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(QColor("#D1D1D1"));
                painter->save();
                painter->drawRoundedRect(rectUp,10,10);
                painter->restore();
            }


            if(fast_order::modelBase->item(r,9)->text() == "P")
            {
                painter->setPen(QColor(255,0,0));
            }
            else if(fast_order::modelBase->item(r,9)->text() == "Y")
            {
                painter->setPen(QColor(0,128,255));
            }
            else
            {
                painter->setPen(Qt::black);
            }

            if(column == 0)
            {
                painter->drawText(yuan,Qt::AlignCenter,fast_order::modelBase->item(r,0)->text());
            }

            if(column == 2)
            {
                painter->drawText(rectUp,Qt::AlignCenter,fast_order::modelBase->item(r,2)->text());
            }

            painter->setPen(QColor("#7A7979"));
            QString str;
            str = "数量:" + fast_order::modelBase->item(r,3)->text() + fast_order::modelBase->item(r,12)->text()  + "  " + "小计:" +  fast_order::modelBase->item(r,6)->text();
            painter->drawText(textRect, str);

            if(!fast_order::modelBase->item(r,7)->text().isEmpty())//特殊做法
            {
                QString str1 = fast_order::modelBase->item(r,7)->text();
                painter->drawText(textRect1, str1);
            }
            painter->setPen(Qt::black);
            if(fast_order::modelBase->item(r,8)->text() == "Y")
            {
                QString zeng = tr("赠送");
                painter->drawText(textRect2,zeng);
            }
        }

        if(r == fast_order::row_n)
        {
            if(column == 1)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(QColor("#FFF4D6"));
                painter->drawRoundedRect(kuang,5,5);
            }
            if(column == 0)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(QColor("#F54180"));
                painter->drawEllipse(option.rect.center(),15,15);
            }


            if(column == 2)
            {
                QRect rectUp(option.rect.topLeft().x()+5,
                             option.rect.topLeft().y(),
                             option.rect.width()-10,
                             option.rect.height()-25);


                painter->setPen(Qt::NoPen);
                painter->setBrush(QColor("#F54180"));
                painter->drawRoundedRect(rectUp,10,10);
            }

            painter->setBrush(Qt::NoBrush);
            painter->setPen(Qt::black);

            if(column == 0)
            {
                painter->drawText(yuan,Qt::AlignCenter,fast_order::modelBase->item(r,0)->text());
            }

            if(column == 2)
            {
                painter->drawText(rectUp,Qt::AlignCenter,fast_order::modelBase->item(r,2)->text());
            }
            painter->setPen(QColor("#7A7979"));
            QString str;
            str = "数量:" + fast_order::modelBase->item(r,3)->text() + fast_order::modelBase->item(r,12)->text()  + "  " + "小计:" +  fast_order::modelBase->item(r,6)->text();
            painter->drawText(textRect, str);
            // painter->drawText(textRect3,fast_order::modelBase->item(r,2)->text());
            if(!fast_order::modelBase->item(r,7)->text().isEmpty())//特殊做法
            {
                QString str1 = fast_order::modelBase->item(r,7)->text();
                painter->drawText(textRect1, str1);
            }
            painter->setPen(Qt::black);
            if(fast_order::modelBase->item(r,8)->text() == "Y")
            {
                QString zeng = tr("赠送");
                painter->drawText(textRect2,zeng);
            }
        }
    }
    else
    {
        QRect rectUp(option.rect.topLeft().x()+5,
                     option.rect.topLeft().y(),
                     option.rect.width()-10,
                     option.rect.height()-25);

        if(r != table_order::row_n)
        {
            if(column == 0)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(QColor("#FFFFFF"));
                painter->save();
                painter->drawRoundedRect(kuang,5,5);
                painter->restore();
            }

            if(column == 1)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(QColor("#CCCCCC"));
                painter->save();
                painter->drawEllipse(option.rect.center(),15,15);
                painter->restore();
            }

            if(column == 2)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(QColor("#D1D1D1"));
                painter->save();
                painter->drawRoundedRect(rectUp,10,10);
                painter->restore();
            }


            if(table_order::modelBase->item(r,9)->text() == "P")
            {
                painter->setPen(QColor(255,0,0));
            }
            else if(table_order::modelBase->item(r,9)->text() == "Y")
            {
                painter->setPen(QColor(0,128,255));
            }
            else
            {
                painter->setPen(Qt::black);
            }

            if(column == 0)
            {
                painter->drawText(yuan,Qt::AlignCenter,table_order::modelBase->item(r,0)->text());
            }

            if(column == 2)
            {
                painter->drawText(rectUp,Qt::AlignCenter,table_order::modelBase->item(r,2)->text());
            }

            painter->setPen(QColor("#7A7979"));
            QString str;
            str = "数量:" + table_order::modelBase->item(r,3)->text() + table_order::modelBase->item(r,12)->text()  + "  " + "小计:" +  table_order::modelBase->item(r,6)->text();
            painter->drawText(textRect, str);

            if(!table_order::modelBase->item(r,7)->text().isEmpty())//特殊做法
            {
                QString str1 = table_order::modelBase->item(r,7)->text();
                painter->drawText(textRect1, str1);
            }
            painter->setPen(Qt::black);
            if(table_order::modelBase->item(r,8)->text() == "Y")
            {
                QString zeng = tr("赠送");
                painter->drawText(textRect2,zeng);
            }
        }

        if(r == table_order::row_n)
        {
            if(column == 1)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(QColor("#FFF4D6"));
                painter->drawRoundedRect(kuang,5,5);
            }
            if(column == 0)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(QColor("#F54180"));
                painter->drawEllipse(option.rect.center(),15,15);
            }


            if(column == 2)
            {
                QRect rectUp(option.rect.topLeft().x()+5,
                             option.rect.topLeft().y(),
                             option.rect.width()-10,
                             option.rect.height()-25);


                painter->setPen(Qt::NoPen);
                painter->setBrush(QColor("#F54180"));
                painter->drawRoundedRect(rectUp,10,10);
            }

            painter->setBrush(Qt::NoBrush);
            painter->setPen(Qt::black);

            if(column == 0)
            {
                painter->drawText(yuan,Qt::AlignCenter,table_order::modelBase->item(r,0)->text());
            }

            if(column == 2)
            {
                painter->drawText(rectUp,Qt::AlignCenter,table_order::modelBase->item(r,2)->text());
            }
            painter->setPen(QColor("#7A7979"));
            QString str;
            str = "数量:" + table_order::modelBase->item(r,3)->text() + table_order::modelBase->item(r,12)->text()  + "  " + "小计:" +  table_order::modelBase->item(r,6)->text();
            painter->drawText(textRect, str);
            if(!table_order::modelBase->item(r,7)->text().isEmpty())//特殊做法
            {
                QString str1 = table_order::modelBase->item(r,7)->text();
                painter->drawText(textRect1, str1);
            }
            painter->setPen(Qt::black);
            if(table_order::modelBase->item(r,8)->text() == "Y")
            {
                QString zeng = tr("赠送");
                painter->drawText(textRect2,zeng);
            }
        }
    }
}
