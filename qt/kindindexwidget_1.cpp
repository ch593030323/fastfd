#include "kindindexwidget_1.h"
#include "sortitem.h"
#include <QDebug>

#define kind_count 4//一页显示大类个数

KindIndexWidget1::KindIndexWidget1(QWidget *parent) :
    QWidget(parent),index(0)
{
    // this->setAttribute(Qt::WA_TranslucentBackground);
    this->resize(QSize(456,45));
    //this->setFixedSize(456,55);
    this->setFixedHeight(60);
    this->setMinimumWidth(456);
    horizontalLayout = new QHBoxLayout();
    for(int i = 0; i < kind_count; ++i)
    {
        if(i == 0)
        {
            SortItem *item = new SortItem("","",true,this);
            connect(item,SIGNAL(touch(QString)),this,SLOT(itemClicked(QString)));
            itemList.append(item);
            horizontalLayout->addWidget(item);
            _lastItem = item;//需要初始化
        }
        else
        {
            SortItem *item = new SortItem("","",false,this);
            connect(item,SIGNAL(touch(QString)),this,SLOT(itemClicked(QString)));
            itemList.append(item);
            horizontalLayout->addWidget(item);
        }
    }
    horizontalLayout->setSpacing(1);
    horizontalLayout->setContentsMargins(0,0, 0,0);
    this->setLayout(horizontalLayout);
}

KindIndexWidget1::~KindIndexWidget1()
{

}

void KindIndexWidget1::preSort()
{
    if(index <= 0)
        return;
    --index;
    showSort();
}

void KindIndexWidget1::nextSort()
{
    if(kindList.count() <= kind_count-1 + index*(kind_count-2))
        return;

    ++index;
    showSort();
}

void KindIndexWidget1::clearSelected()
{
    if(!_lastItem.isNull())
        _lastItem->setSelect(false);
}

void KindIndexWidget1::showSort()
{
    int count = kindList.count();
    if(0 == index)
    {
        //是第一页
        if(count > kind_count)
        {
            for(int i = 0; i < kind_count-1; ++i)
            {
                itemList.value(i)->_kindId = kindList.value(i);
                itemList.value(i)->setName(nameList.value(i));
            }
            itemList.value(kind_count-1)->_kindId = "next";
            itemList.value(kind_count-1)->setName("下一页");
        }
        else
        {
            for(int i = 0; i < kind_count; ++i)
            {
                if(i < count)
                {
                    itemList.value(i)->_kindId = kindList.value(kind_count*index + i);
                    itemList.value(i)->setName(nameList.value(kind_count*index + i));
                }
                else
                {
                    itemList.value(i)->_kindId = "";
                    itemList.value(i)->setName("");
                }
            }
        }
    }
    else
    {
        //不是第一页
        int preCount = kind_count-1 + (index-1)*(kind_count-2);
        if(count-preCount > kind_count-1)
        {
            //超过这一页最高kind_count-1个
            itemList.value(0)->_kindId = "pre";
            itemList.value(0)->setName("上一页");
            for(int i = 0; i < kind_count-2; ++i)
            {
                itemList.value(i + 1)->_kindId = kindList.value(preCount + i);
                itemList.value(i + 1)->setName(nameList.value(preCount + i));
            }
            itemList.value(kind_count -1)->_kindId = "next";
            itemList.value(kind_count -1)->setName("下一页");
        }
        else
        {
            itemList.value(0)->_kindId = "pre";
            itemList.value(0)->setName("上一页");
            for(int i = 0; i < kind_count-1; ++i)
            {
                if(i < count-preCount)
                {
                    itemList.value(i + 1)->_kindId = kindList.value(preCount + i);
                    itemList.value(i + 1)->setName(nameList.value(preCount + i));
                }
                else
                {
                    itemList.value(i + 1)->_kindId = "";
                    itemList.value(i + 1)->setName("");
                }
            }
        }
    }
}

void KindIndexWidget1::clear()
{
    kindList.clear();
    nameList.clear();
    index = 0;
    _lastItem = NULL;
}

void KindIndexWidget1::addSort(QString kind, QString name)
{
    kindList.append(kind);
    nameList.append(name);//大类的名字
}

void KindIndexWidget1::itemClicked(QString kindId)
{
    if(kindId == "pre")//上一页
    {
        if(!_lastItem.isNull())
            _lastItem->setSelect(false);
        static_cast<SortItem*>(sender())->setSelect(false);
        _lastItem = NULL;
        preSort();
        return;
    }
    else if(kindId == "next")//下一页
    {
        if(!_lastItem.isNull())
            _lastItem->setSelect(false);
        static_cast<SortItem*>(sender())->setSelect(false);
        _lastItem = NULL;
        nextSort();
        return;
    }

    if(_lastItem != sender())//本页
    {
        if(!_lastItem.isNull())
            _lastItem->setSelect(false);
        _lastItem = static_cast<SortItem*>(sender());
    }

    emit kindSelect1(kindId);//发送点的是哪一个大类，从而显示小累
}

