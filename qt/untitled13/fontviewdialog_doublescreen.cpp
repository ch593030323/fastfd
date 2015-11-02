#include "fontviewdialog_doublescreen.h"
#include "auto_expand_image.h"
#include <QGraphicsProxyWidget>
#include <QSettings>
#include "public_sql.h"
#include "trans_u2g.h"
#include "fast_order_2.h"

fontviewDialog_doublescreen::fontviewDialog_doublescreen(QWidget *parent) :
    frontViewDialog_virtual(parent),
    ui(new Ui::fontviewDialog_doublescreen)
{
    ui->setupUi(this);
    QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    expand=new auto_expand_image(conf.value("system_setting/guanggaolujing").toString(),
                                 conf.value("system_setting/bofanglujing").toString());
    expand->reset_runningtext(conf.value("system_setting/bofangg_runningtext").toString());
    expand->resize(expand->image_width, expand->image_height);
    QGraphicsProxyWidget *backitem=new QGraphicsProxyWidget;
    backitem->setWidget(expand);
    backitem->setCacheMode(QGraphicsItem::ItemCoordinateCache);
    QGraphicsScene *scene=new QGraphicsScene;
    scene->setSceneRect(scene->itemsBoundingRect());
    scene->addItem(backitem);
    ui->graphicsView->setScene(scene);
    expand->run();

    timer.setSingleShot(true);
    timer.setInterval(60*1000);
    connect(&timer,SIGNAL(timeout()),this,SLOT(toback_guanggao()));
    ui->widget_detail->hide();

    ui->pushButton->setIcon(QPixmap(public_sql::get_logo_path()));
    ui->tableView->setModel(&fast_order_2::modelBase);
}

fontviewDialog_doublescreen::~fontviewDialog_doublescreen()
{
    delete ui;
}

void fontviewDialog_doublescreen::to_show_and_hide()
{
    ui->widget_detail->show();
    expand->stop();
    timer.start();
}

void fontviewDialog_doublescreen::toback_guanggao()
{
    expand->start();
    ui->widget_detail->hide();
}
