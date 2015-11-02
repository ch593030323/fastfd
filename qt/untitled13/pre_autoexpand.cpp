#include "pre_autoexpand.h"
#include <QMouseEvent>
#include <QApplication>

QImage scale(const QString &imageFileName)
{
    QImage image(imageFileName);
    if(image.isNull()) return QImage();
    return image.scaled(pre_autoexpand_rb::iconsize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

pre_autoexpand::pre_autoexpand(QWidget *parent) :
    QFrame(parent),
    help(new pre_autoexpand_help),
    saverb(0),
    isshowpixmap(false)
{
    btn_up=new QPushButton(this);
    btn_down=new QPushButton(this);
    btn_up->setFocusPolicy(Qt::NoFocus);
    btn_down->setFocusPolicy(Qt::NoFocus);
    btn_up->setProperty("outer_stylesheet", "pushbutton_up");
    btn_down->setProperty("outer_stylesheet", "pushbutton_down");

    QVBoxLayout *vlayout=new QVBoxLayout;
    QHBoxLayout *hlayout=new QHBoxLayout;
    vlayout->setMargin(help->space);
    hlayout->setMargin(help->space);
    hlayout->addStretch(1);
    hlayout->addWidget(btn_up);
    hlayout->addWidget(btn_down);
    vlayout->addLayout(help->glayout);
    vlayout->addLayout(hlayout);
    this->setLayout(vlayout);
    imagewatcher=new QFutureWatcher<QImage>(this);
    connect(imagewatcher,SIGNAL(resultReadyAt(int)),this,SLOT(updateImage(int)));
    connect(imagewatcher,SIGNAL(finished()),this,SLOT(finished()));
    connect(btn_up,SIGNAL(clicked()),this,SLOT(toprevious()));
    connect(btn_down,SIGNAL(clicked()),this,SLOT(tonext()));
}

void pre_autoexpand::setsql(const QString &value)
{
    help->sql=value;
}

void pre_autoexpand::setpixmappath(const QString &value)
{
    help->pixmappath=value;
}

void pre_autoexpand::sethcount(int value)
{
    help->hcount=value;
}

int pre_autoexpand::gethcount()
{
    return qMax(2,help->hcount);
}

void pre_autoexpand::setvcount(int value)
{
    qDebug() << "vcount" << value;
    help->vcount=value;
}

int pre_autoexpand::getvcount()
{
    return qMax(2,help->vcount);
}

int pre_autoexpand::getspace()
{
    return help->space;
}

void pre_autoexpand::setshowpixmapEnable(bool enabled)
{
    isshowpixmap=enabled;
    pre_autoexpand_rb::setshowpixmapEnabel(isshowpixmap);
}

bool pre_autoexpand::isshowpixmapEnable()
{
    return isshowpixmap;
}

void pre_autoexpand::seticonsize(const QSize &size)
{
    pre_autoexpand_rb::iconsize0=size.expandedTo(QSize(20,20));//最小是20x20
}

QSize pre_autoexpand::geticonsize()
{
    return pre_autoexpand_rb::iconsize0;
}

void pre_autoexpand::start()
{
    help->pvnlist_clear();
    tonext();
}

void pre_autoexpand::item_event_click()
{
    if(saverb){
        emit signal_text(saverb->index);
    }
}

void pre_autoexpand::tonext()
{
    pre_autoexpand_help::PVN pagepvn=help->get_next_actualcount();
    toconduct(pagepvn);
}

void pre_autoexpand::toprevious()
{
    pre_autoexpand_help::PVN pagepvn=help->get_previous_actualcount();
    toconduct(pagepvn);
}

void pre_autoexpand::toclicktext()
{
    pre_autoexpand_rb *btn=qobject_cast<pre_autoexpand_rb*>(this->sender());
    if(btn&&btn->isChecked()){
        emit signal_text(btn->index);
        saverb=btn;
    }
}

void pre_autoexpand::toconduct(const pre_autoexpand_help::PVN &pagepvn)
{
    saverb=0;
    help->glayout_deleteall();
    int step=0;
    int stepcount=help->vcount*help->hcount;
    QStringList filepaths;
    if(pagepvn.v>0){
        help->query.exec(pagepvn.sql);
        btn_up->setEnabled(pagepvn.p==-1);
        while(help->query.next()){
            pre_autoexpand_rb *btn=new pre_autoexpand_rb(isshowpixmap);
            btn->index=help->query.record().value(0).toString();
            btn->setText(help->query.record().value(1).toString());
            qDebug() << "------------------"<<btn->text();
            help->glayout->addWidget(btn, step/help->hcount, step%help->hcount);
            step++;
            connect(btn,SIGNAL(clicked()),this,SLOT(toclicktext()));
            //!添加图片路径和保存对应按钮的指针
            filepaths.append(help->pixmappath+"/"+btn->index);
            help->pagekeylist.append(btn);
        }
        while(step<stepcount){
            pre_autoexpand_label *btn=new pre_autoexpand_label;
            help->glayout->addWidget(btn, step/help->hcount, step%help->hcount);
            step++;
        }
        btn_down->setEnabled(pagepvn.n==1);

        if(imagewatcher->isRunning()){
            imagewatcher->cancel();
        }
        //!开始future获取文件名
        imagewatcher->setFuture(QtConcurrent::mapped(filepaths, scale));
    }
}

void pre_autoexpand::finished()
{

}

void pre_autoexpand::updateImage(int step)
{
    pre_autoexpand_rb *btn=help->pagekeylist.value(step);
    if(btn)
        btn->setIcon(QPixmap::fromImage(imagewatcher->resultAt(step)));
}
