#include "w_sys_manage.h"
#include <QDir>

QStandardItemModel w_sys_manage::MODEL_SYS_DISH_CONTROL;

w_sys_manage::w_sys_manage(QObject *parent) :
    QObject(parent)
{
}

int w_sys_manage::MODEL_SYS_INIT()
{
    qDebug() << "MODEL_SYS_INIT...";
    //! init
    QList<QStandardItem *> items;
    if(MODEL_SYS_DISH_CONTROL.rowCount() == 0){
        items.clear();items<<new QStandardItem("1")<<new QStandardItem(tr("单品"));MODEL_SYS_DISH_CONTROL.appendRow(items);
        items.clear();items<<new QStandardItem("2")<<new QStandardItem(tr("单桌"));MODEL_SYS_DISH_CONTROL.appendRow(items);
        items.clear();items<<new QStandardItem("3")<<new QStandardItem(tr("单营业日"));MODEL_SYS_DISH_CONTROL.appendRow(items);
    }
}

QStringList w_sys_manage::get_flilename_path(const QString &path, const QStringList &filter)
{
    QStringList filenames;
    QDir *dir=new QDir(path);
    //filter<<"*.dat";
    //dir->setNameFilters(filter);
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));

    //注释掉的两行是用来文件过滤用的，如果你要指定文件后缀名在 filter<<"*.dat"  写就好了
    QString fileName;
    for(int i = 0; i < fileInfo->count(); ++i)
    {
        fileName = fileInfo->at(i).fileName();
        if(fileName=="."
                || fileName=="..")continue;
        filenames << fileName;

    }
    return filenames;
}

QString w_sys_manage::set_flilename_path(const QString &flilename, const QString &path)
{

}

