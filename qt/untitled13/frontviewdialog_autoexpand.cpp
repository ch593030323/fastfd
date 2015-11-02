#include "frontviewdialog_autoexpand.h"
#include <qdebug.h>
#include <QLabel>
#include <QToolButton>
#include <QApplication>
#include <QMouseEvent>

void frontViewDialog_autoexpand::printcontent()
{
    foreach(const content_item_icon &c, content){
        qDebug() <<"[content_item_icon"<<c.index;
        qDebug() <<"content_item_icon"<<c.text;
        //        qDebug() <<"content_item_icon"<<c.icon;
        qDebug() <<"content_item_icon"<<c.state;
        qDebug() <<"content_item_icon"<<c.toolbtn;
    }
}

frontViewDialog_autoexpand::frontViewDialog_autoexpand(QWidget *parent) :
    QFrame(parent)
{
    horizontal_count=3;
    vertical_count=2;

    page_all=0;
    curpage=0;
    is_like_tab=true;
    is_can_released=false;
    tool_size = QSize(100,100);
    tool_icon_size = QSize(50,50);

    glayout=new QGridLayout;
    this->setLayout(glayout);
    glayout->setMargin(2);
    glayout->setSpacing(2);

    connect(this,SIGNAL(signal_next()),this,SLOT(btn_goto_next()));
    connect(this,SIGNAL(signal_pre()),this,SLOT(btn_goto_pre()));
}

void frontViewDialog_autoexpand::test()
{
    clearall_item();
    for(int x=0;x<20;x++)
        add_item(QString("%1").arg(x), " are you ready to go to library?", true, QPixmap(":/p"));
    generat_from_items();
}

void frontViewDialog_autoexpand::btn_goto_pre()
{
    curpage--;
    generat_from_items();
    emit signal_curpage(curpage, page_all);
}

void frontViewDialog_autoexpand::btn_goto_next()
{
    curpage++;
    generat_from_items();
    emit signal_curpage(curpage, page_all);
}

void frontViewDialog_autoexpand::thisischeckcenter(QString sqr)
{
    if(is_like_tab){//单个
        if(is_like_tab_save != sqr){
            for(QMap<QString, frontViewDialog_toolbtn *>::iterator itr=content_tool_save_map.begin();
                itr!=content_tool_save_map.end();
                itr++){
                if(itr.key()==is_like_tab_save){
                    if(itr.value()){
                        itr.value()->setDown(false);
                        itr.value()->ispressed=false;
                        break;
                    }
                }
            }
            is_like_tab_save=sqr;
        }
    }

    if(!is_like_tab){//多个
        foreach(const content_item_icon &c, content){
            if(c.index==sqr){
                frontViewDialog_toolbtn *btn=get_content_toolbtn(sqr);
                if(btn){
                    c.toolbtn->state=this->get_content_toolbtn(sqr)->state;
                }
                break;
            }
        }
    }
}

void frontViewDialog_autoexpand::btn_text()
{
    frontViewDialog_toolbtn *p=qobject_cast<frontViewDialog_toolbtn *>(this->sender());
    if(p){
        emit signal_text(p->index);
    }
}

void frontViewDialog_autoexpand::torestore_tab()
{
    frontViewDialog_toolbtn *toolbtn=0;
    toolbtn=get_content_toolbtn(is_like_tab_save);
    if(toolbtn){
        toolbtn->setDown(true);
        toolbtn->ispressed=true;
    }
}

void frontViewDialog_autoexpand::glayout_deleteall()
{
    for(int index=0,index_count=glayout->count();index<index_count;index++){
        QWidget* widget = glayout->itemAt(0)->widget();
        glayout->removeWidget(widget);
        //这儿不删除
        if(qobject_cast<QLabel*>(widget)) {delete widget;}
        else {widget->hide();}
    }
}

frontViewDialog_toolbtn *frontViewDialog_autoexpand::get_content_toolbtn(const QString &index)
{
    foreach(const content_item_icon &c, content){
        if(c.index==index){
            return c.toolbtn;
        }
    }
    return 0;
}

frontViewDialog_toolbtn *frontViewDialog_autoexpand::get_content_tool_save_map_toolbtn(const QString &index)
{
    return content_tool_save_map.value(index);
}

int frontViewDialog_autoexpand::calulate_page_all()
{
    int glayout_count=horizontal_count*vertical_count;
    int ret_all=0;
    int count_remain=content.count();
    int count_remain_take=0;
    bool count_remain_flag=true;
    while((count_remain_take=one_by_one_page_all(count_remain, glayout_count, count_remain_flag))!=-1){
        ret_all++;
        count_remain_flag=false;
        count_remain=count_remain-count_remain_take;
    }
    return ret_all;
}

int frontViewDialog_autoexpand::one_by_one_page_all(int content_remain, int count, bool begin)
{
    if(content_remain<=0) return -1;//结束
    if(begin&&content_remain<=count){//开头
        return content_remain;
    }
    if(begin&&content_remain>count){//开头
        return count-1;
    }
    if((content_remain+1)<=count)//尾部
        return count-1;
    if((content_remain+1)>count){//中间
        return count-2;
    }
    return -1;
}

//-4[填充] -3[<<] -2[>>] -1[null] 0[0]
int frontViewDialog_autoexpand::calulate_content_index_from_curpage_index(int curpage_index)
{
    int count=vertical_count*horizontal_count;
    int ret_index = -1;
    switch(0){
    case 0:
        if(page_all==0){
            break;
        }
    case 1:
        if(page_all==1){
            ret_index=curpage_index;
            break;
        }
    case 2:
        if(page_all>1){
            if(curpage==-1){
                ret_index=-1;
                break;
            }

            if(curpage==0){//开头
                if(curpage_index==count-1){
                    ret_index=-2;//>>
                    break;
                }

                ret_index=curpage_index;
                break;
            }

            //2                                            4
            if(curpage>0 && curpage == page_all-1){//最后
                if(curpage_index==0){
                    ret_index=-3;//<<
                    break;
                }

                ret_index=curpage_index+curpage*count-(curpage)*2;
                break;
            }

            if(curpage>0 && curpage < page_all-1){//中间
                if(curpage_index==0){
                    ret_index=-3;//<<
                    break;
                }
                if(curpage_index==count-1){
                    ret_index=-2;//>>
                    break;
                }
                ret_index=curpage_index+curpage*count-(curpage)*2;
                break;
            }
        }
    }
    return content.count() <= ret_index ? -4 : ret_index ;
}

void frontViewDialog_autoexpand::add_item(const QString &index, const QString &text, bool _is_use_pixmap, const QPixmap &icon, frontViewDialog_toolbtn::THISSTATE state)
{
    content.append(content_item_icon(index, text, _is_use_pixmap, icon, state));
}

void frontViewDialog_autoexpand::clearall_item()
{
    content.clear();
    generat_from_items();
}

void frontViewDialog_autoexpand::item_event_click()
{
    frontViewDialog_toolbtn *toolbtn=get_content_toolbtn(is_like_tab_save);
    if(toolbtn){
        QMouseEvent event(QEvent::MouseButtonRelease, QPoint(1,1), Qt::MouseButton(0), 0, 0);
        QApplication::sendEvent(toolbtn, &event);
    }
}

void frontViewDialog_autoexpand::item_first_focus()
{
    if(content.count() > 0){
        if(is_like_tab_save!=content.first().index){
            qDebug() << is_like_tab_save << content.first().index;
            frontViewDialog_toolbtn *toolbtn=get_content_tool_save_map_toolbtn(is_like_tab_save);
            if(toolbtn){
                toolbtn->ispressed=false;
                toolbtn->setDown(false);
            }

            toolbtn=get_content_tool_save_map_toolbtn(content.first().index);
            if(toolbtn){
                toolbtn->ispressed=true;
                toolbtn->setDown(true);
            }

            is_like_tab_save=content.first().index;
        }
    }
}

void frontViewDialog_autoexpand::generat_from_items()
{
    glayout_deleteall();
    page_all=calulate_page_all();
    //    if(page_all==0) return;
    for(int index=0,actual_index=0,index_count=horizontal_count*vertical_count;index<index_count;index++){
        int typeindex=calulate_content_index_from_curpage_index(index);
        QWidget *toolButton=0;
        if(typeindex==-1){
            toolButton=new QLabel;
        } else if(typeindex==-4){
            toolButton=new QLabel;
        } else if(typeindex==-2){
            toolButton = new QPushButton(">>", this);
            connect(static_cast<QPushButton *>(toolButton),SIGNAL(clicked()),this,SIGNAL(signal_next()));
        } else if(typeindex==-3){
            toolButton = new QPushButton("<<", this);
            connect(static_cast<QPushButton *>(toolButton),SIGNAL(clicked()),this,SIGNAL(signal_pre()));
        } else {
            //1
            if(content_tool_save_map.find(content[typeindex].index)==content_tool_save_map.end()){
                content[typeindex].toolbtn=new frontViewDialog_toolbtn(this);
                content[typeindex].toolbtn->index=content[typeindex].index;
                content[typeindex].toolbtn->is_like_tab=is_like_tab;
                content[typeindex].toolbtn->is_can_released=is_can_released;
                content[typeindex].toolbtn->is_use_pixmap=content[typeindex].is_use_pixmap;
                content[typeindex].toolbtn->setIcon(content[typeindex].icon);
                content[typeindex].toolbtn->setIconSize(tool_icon_size);
                content[typeindex].toolbtn->presscolor=presscolor;
                connect(content[typeindex].toolbtn,SIGNAL(thisclicked(QString)),this,SLOT(thisischeckcenter(QString)));//用于还原
                connect(content[typeindex].toolbtn,SIGNAL(thisclicked(QString)),this,SIGNAL(signal_text(QString)));
                connect(content[typeindex].toolbtn,SIGNAL(thisdoubleclicked(QString)),this,SIGNAL(signal_double_text(QString)));

                if(is_like_tab&&is_like_tab_save.isEmpty())is_like_tab_save=content[typeindex].index;
            } else {
                toolButton=content[typeindex].toolbtn=content_tool_save_map.value(content[typeindex].index);
            }
            //2
            content[typeindex].toolbtn->state=content[typeindex].state;
            content[typeindex].toolbtn->setText(content[typeindex].text);

            //3
            toolButton =  content[typeindex].toolbtn;
            if(content_tool_save_map.find(content[typeindex].index)==content_tool_save_map.end()){
                content_tool_save_map.insert(content[typeindex].index, static_cast<frontViewDialog_toolbtn*>(toolButton));
            }
        }
        toolButton->setProperty("outer_stylesheet", btn_property);
        toolButton->setFixedSize(tool_size);//y("outer_stylesheet", property);
        glayout->addWidget(toolButton, actual_index/horizontal_count, actual_index%horizontal_count);
        if(toolButton->isHidden())toolButton->show();
        actual_index++;
    }

    if(is_like_tab)torestore_tab();
}

void frontViewDialog_autoexpand::set_btn_property(const QString &property)
{
    btn_property=property;
}

QStringList frontViewDialog_autoexpand::hasSelected()
{
    QStringList list;
    foreach(const content_item_icon &c, content){
        if(c.toolbtn->ispressed)list.append(c.toolbtn->index);
    }
    return list;
}
