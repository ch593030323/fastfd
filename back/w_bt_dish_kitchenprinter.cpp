#include "w_bt_dish_kitchenprinter.h"
#include "ui_w_bt_dish_dialog.h"
#include <QDebug>
#include "backheader.h"
#include "w_bt_dish.h"
#include "lds_tableview_delegate_check.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include "lds_messagebox.h"
#include <QSqlError>
#include "lds_combobox.h"
#include "n_func.h"
#include "public_logindialog.h"
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QtConcurrent>
#else
#include <QtCore>
#endif
#include"public_logindialog.h"
#include "asyn_printer.h"

/** 厨打策略
 * 1.将对应的厨打单，整理号，一单有多个菜品的，以第一个菜品的ip为准；
 * 2.厨打单整理好后，对其按ip进行分类；
 * 3.分类好后，进入线程，对每个ip分配tcp套接字，进行厨打；
 * 4.一套厨打单，对应的ip线程里，进行每单的写入和回读，然后依次判定厨打成功，或厨打失败，将厨打的结果记录到厨打单里；
 * 5.厨打判断结束后，关闭tcp，根据厨打单内的信息，处理厨打结果（漏打，还是成功）
 * 6.本次厨打结束
 * 7.若是打印期间，有其他出品打印，一律被锁挡住，当正常厨打结束后，才可以继续经行
 * 8.出品同厨打
*/
int w_bt_dish_Kitchenprinter::k_data_content_printer_width=48;
K_DATA_CONTENT w_bt_dish_Kitchenprinter::k_data_content_mode;

//菜品厨打设置
w_bt_dish_Kitchenprinter::w_bt_dish_Kitchenprinter(QWidget *parent) :
    QDialog(parent),ui(new Ui_w_bt_dish_Dialog)
{
    ui->setupUi(this);
    //! treewidget
    ui->treeWidget->header()->hide();
    ui->treeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->rootitem=new lds_treeWidgetItem(QStringList()
                                                    <<tr("全部类型"));
    ui->treeWidget->addTopLevelItem(ui->treeWidget->rootitem);
    tablemodel=new lds_model_sqlrelationaltablemodel(this);
    tablemodel->setTable("cey_bt_dish");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);tablemodel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    tablemodel->setRelation(4/*ch_dish_typeno*/, QSqlRelation("cey_bt_dish_type", "ch_dish_typeno","vch_dish_typename"));
    tablemodel->setRelation(5/*ch_unitno*/, QSqlRelation("cey_bt_unit", "ch_unitno","vch_unitname"));
    tablemodel->setRelation(14/*ch_sub_typeno*/, QSqlRelation("cey_bt_dish_subtype", "ch_sub_typeno","vch_sub_typename"));
    tablemodel->setRelation(28/*vch_pur_unitno*/, QSqlRelation("cey_bt_unit", "ch_unitno","vch_unitname"));
    tablemodel->setRelation(34/*vch_outprint*/, QSqlRelation("cey_bt_kitchen_print", "vch_printip","vch_printname"));
    tablemodel->setRelation(13/*vch_printer*/, QSqlRelation("cey_bt_kitchen_print", "vch_printip","vch_printname"));

    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel->fpmodel);
    //! refresh
    torefresh();

    tablemodel->save_set_header("ch_dishno", tr("品码"));
    tablemodel->save_set_header("vch_dishname", tr("品名"));
    tablemodel->save_set_header("vch_spell", tr("拼音简码"));
    tablemodel->save_set_header("ch_printflag", tr("是否厨打"), true);
    tablemodel->save_set_header("cey_bt_kitchen_print_vch_printname_2", tr("厨房打印机"), true);
    tablemodel->save_set_header("ch_outflag", tr("是否出品打印"), true);
    tablemodel->save_set_header("vch_printname", tr("出品打印机"), true);
    QList<int> virtualheades=ui->tableView->rePecifyHeader(tablemodel->save_set_header_saver);
    w_bt_dish_Kitchenprinter_check_com *delegate=
            new w_bt_dish_Kitchenprinter_check_com(
                QVector<int>() << virtualheades[3] << virtualheades[5] ,
            "vch_printname",
            ui->tableView);
    delegate->tablemodel=tablemodel;
    delegate->setClickable();
    ui->tableView->setItemDelegate(delegate) ;


    //! toolbar
    ui->widget->setup(QStringList()
                      << tr("打印机管理") << tr("批处理") << tr("参数设置")<< tr("刷新")
                      << tr("保存") << tr("退出"));
    connect(ui->widget->index_widget(tr("打印机管理")), SIGNAL(clicked()),this,SLOT(tokitchenprint()));
    connect(ui->widget->index_widget(tr("参数设置")), SIGNAL(clicked()),this,SLOT(toparamset()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->widget->index_widget(tr("保存")), SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("批处理")), SIGNAL(clicked()),this,SLOT(tonews()));
    connect(ui->widget->index_widget(tr("刷新")), SIGNAL(clicked()),this,SLOT(torefresh()));

    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(treeselect()));
    connect(ui->treeWidget,SIGNAL(pressed(QModelIndex)),this,SLOT(treeclicked()));


    //! after
    ui->checkBox->setVisible(false);
    //    k_data_content_example();
}

void w_bt_dish_Kitchenprinter::k_data_content_mode_init()
{
    k_data_content_mode=K_DATA_CONTENT();
    QSettings backsetting(public_sql::settings_ini_dir_file, QSettings::IniFormat);
    if(backsetting.value("w_bt_dish_Kitchenprinter_param/tablename_check").toString() == "1"){
        k_data_content_mode.about_title.append(K_TITLE(tr("餐桌名称"), "",
                                                       backsetting.value("w_bt_dish_Kitchenprinter_param/tablename_check_size").toString()));
    }
    if(backsetting.value("w_bt_dish_Kitchenprinter_param/openbillno_check").toString() == "1"){
        k_data_content_mode.about_title.append(K_TITLE(tr("开台单号"), "",
                                                       backsetting.value("w_bt_dish_Kitchenprinter_param/openbillno_check_size").toString()));
    }
    if(backsetting.value("w_bt_dish_Kitchenprinter_param/dishopertor_check").toString() == "1"){
        k_data_content_mode.about_title.append(K_TITLE(tr("点菜人"), "",
                                                       backsetting.value("w_bt_dish_Kitchenprinter_param/dishopertor_check_size").toString()));
    }
    if(backsetting.value("w_bt_dish_Kitchenprinter_param/dishtime_check").toString() == "1"){
        k_data_content_mode.about_title.append(K_TITLE(tr("点菜时间"), "",
                                                       backsetting.value("w_bt_dish_Kitchenprinter_param/dishtime_check_size").toString()));
    }
    if(backsetting.value("w_bt_dish_Kitchenprinter_param/printtime_check").toString() == "1"){
        k_data_content_mode.about_title.append(K_TITLE(tr("打印时间"), "",
                                                       backsetting.value("w_bt_dish_Kitchenprinter_param/printtime_check_size").toString()));
    }
    if(backsetting.value("w_bt_dish_Kitchenprinter_param/person_check").toString() == "1"){
        k_data_content_mode.about_title.append(K_TITLE(tr("人数"), "",
                                                       backsetting.value("w_bt_dish_Kitchenprinter_param/person_check_size").toString()));
    }
    if(backsetting.value("w_bt_dish_Kitchenprinter_param/serialno_check").toString() == "1"){
        k_data_content_mode.about_title.append(K_TITLE(tr("流水号"), "",
                                                       backsetting.value("w_bt_dish_Kitchenprinter_param/serialno_check_size").toString()));
    }

    QVector<DATA_CONTENT> vdatac;
    if(backsetting.value("w_bt_dish_Kitchenprinter_param/ch_dishno_check").toString() == "1"){
        vdatac<< DATA_CONTENT(tr("品名"),tr("品名"),
                              backsetting.value("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin").toInt(),
                              backsetting.value("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin").toInt()
                              );
    }
    if(backsetting.value("w_bt_dish_Kitchenprinter_param/vch_dishname_check").toString() == "1"){
        vdatac<< DATA_CONTENT(tr("品码"),tr("品码"),
                              backsetting.value("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin").toInt(),
                              backsetting.value("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin").toInt()
                              );
    }
    if(backsetting.value("w_bt_dish_Kitchenprinter_param/num_num_check").toString() == "1"){
        vdatac<< DATA_CONTENT(tr("数量"),tr("数量"),
                              backsetting.value("w_bt_dish_Kitchenprinter_param/num_num_size_spin").toInt(),
                              backsetting.value("w_bt_dish_Kitchenprinter_param/num_num_hang_spin").toInt()
                              );
    }
    if(backsetting.value("w_bt_dish_Kitchenprinter_param/num_price_check").toString() == "1"){
        vdatac<< DATA_CONTENT(tr("价格"),tr("价格"),
                              backsetting.value("w_bt_dish_Kitchenprinter_param/num_price_size_spin").toInt(),
                              backsetting.value("w_bt_dish_Kitchenprinter_param/num_price_hang_spin").toInt()
                              );
    }
    if(backsetting.value("w_bt_dish_Kitchenprinter_param/vch_unit_check").toString() == "1"){
        vdatac<< DATA_CONTENT(tr("单位"),tr("单位"),
                              backsetting.value("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin").toInt(),
                              backsetting.value("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin").toInt()
                              );
    }
    k_data_content_mode.about_content.content.append(
                INDEX_DATA_CONTENT(tr(""),tr(""),0.0,vdatac));
    k_data_content_mode.about_content.size=
            backsetting.value("w_bt_dish_Kitchenprinter_param/printer_content_size").toString();

    if(backsetting.value("w_bt_dish_Kitchenprinter_param/dishsum_numprice_check").toString() == "1"){
        k_data_content_mode.about_tail.append(
                    K_TAIL(tr("菜品金额合计"), "",
                           backsetting.value("w_bt_dish_Kitchenprinter_param/printer_tail_size").toString()));
    }

    switch(backsetting.value("w_bt_dish_Kitchenprinter_param/paperwidth", 0).toInt()){
    case 0: //58
        w_bt_dish_Kitchenprinter::k_data_content_printer_width=32;
        break;
    case 1: //80
        w_bt_dish_Kitchenprinter::k_data_content_printer_width=48;
        break;
    }
}

void w_bt_dish_Kitchenprinter::k_data_content_example()
{

}

QList<BINS_IP> w_bt_dish_Kitchenprinter::k_data_print(K_DATA_CONTENT &kdcontent, const QString &key, const QString &_dayindanhao)
{
    QMap<QString/*ip*/, BINS_IP> ret_bin_ip_map;
    QMap<QString/*ip*/, BINS_IP>::iterator ret_bin_ip_map_itr;
    blockIns bins;
    blockIns bins_title;
    INDEX_DATA_CONTENT dcp;
    INDEX_DATA_CONTENT dcp2;
    QSqlQuery query;
    bins.append(backheader::printer->drawText("1B40 1B6101 1D2101",key.isEmpty()?tr("厨房打印单\n"):tr("出品打印单\n")));//居中放大
    bins.append(backheader::printer->drawText("1B40", ""));

    for(int k0=0,k0_count=k_data_content_mode.about_title.count();k0<k0_count;k0++){
        K_TITLE ssp0=k_data_content_mode.about_title[k0];
        for(int k=0,k_count=kdcontent.about_title.count();k<k_count;k++){
            K_TITLE ssp=kdcontent.about_title[k];
            if(ssp.pre==ssp0.pre){
                QString fontsize=ssp0.size;
                if(fontsize.count()!=2){//修改字体大小,版本号4.00小票打印机支持只00 10 01 11四种情况
                    fontsize="00";
                }
                bins.append(backheader::printer->drawText(
                                QByteArray("1D21").append(fontsize),
                                ssp.pre+":"+ssp.content+"\n")
                            );
                bins.append(backheader::printer->drawText(
                                "1D2100")
                            );
            }
        }
    }//if(isempty)
    bins.append(backheader::printer->filldata_n("",'-', k_data_content_printer_width));
    QMap<int, QPair<bool, int> >dcp_ibmap;
    //!  1 begin
    dcp=kdcontent.about_content.content[0];
    dcp2=k_data_content_mode.about_content.content[0];
    QMap<QString , int> dcp2_strs;
    foreach(DATA_CONTENT dcp, dcp2.datas){
        dcp2_strs.insert(dcp.vch_dishname, dcp.value_width);
    }

    for(int sqr=0;sqr<dcp.datas.count();sqr++){
        dcp_ibmap.insert(sqr, QPair<bool, int>(
                             dcp2_strs.keys().contains(dcp.datas[sqr].vch_dishname),
                             dcp2_strs.value(dcp.datas[sqr].vch_dishname))
                         );
    }
    //!  1 end
    int r=0;
    int r_count=kdcontent.about_content.content.count();
    if(r_count>0){
        dcp=kdcontent.about_content.content[r];
        for(int sqr=0;sqr<dcp.datas.count();sqr++){
            if(!dcp_ibmap.value(sqr).first)continue;
            QString fontsize=k_data_content_mode.about_content.size;
            if(fontsize.count()!=2){//修改字体大小,版本号4.00小票打印机支持只00 10 01 11四种情况
                fontsize="00";
            }
            bins.append(backheader::printer->drawText(
                            QByteArray("1D21").append(fontsize),
                            dcp.datas[sqr].vch_dishname,
                            Qt::AlignLeft,
                            dcp_ibmap.value(sqr).second)
                        );
            bins.append(backheader::printer->drawText(
                            "1D2100")
                        );
        }
        bins.append(backheader::printer->n_filldata_n("",'-', k_data_content_printer_width));
        bins_title.append(bins);
        bins.clear();
        r++;
    }
    for(;r<r_count;r++){///for
        dcp=kdcontent.about_content.content[r];
        if(!dcp.type.isEmpty()){
            bins.append(backheader::printer->drawText(
                            "",
                            "---------"+dcp.type+"---------",
                            Qt::AlignCenter, k_data_content_printer_width)
                        );
            bins.append(backheader::printer->drawText("", "\n"));
            bins_title.append(bins);
            bins.clear();
        }
        for(int sqr=0;sqr<dcp.datas.count();sqr++){
            if(!dcp_ibmap.value(sqr).first)continue;
            QString fontsize=k_data_content_mode.about_content.size;
            if(fontsize.count()!=2){//修改字体大小,版本号4.00小票打印机支持只00 10 01 11四种情况
                fontsize="00";
            }
            bins.append(backheader::printer->drawText(
                            QByteArray("1D21").append(fontsize),
                            dcp.datas[sqr].vch_dishname,
                            Qt::AlignLeft,
                            dcp_ibmap.value(sqr).second)
                        );
            bins.append(backheader::printer->drawText(
                            "1D2100")
                        );
        }
        bins.append(backheader::printer->drawText("", "\n"));

        query.exec(QString(key.isEmpty()?
                               "select ch_printflag,vch_printer from cey_bt_dish where ch_dishno = '%1'":
                               "select ch_outflag,vch_outprint from cey_bt_dish where ch_dishno = '%1'").arg(dcp.ch_dishno));
        query.next();
        if(query.record().value(0).toString()=="Y"){
            ret_bin_ip_map_itr=ret_bin_ip_map.find(query.record().value(1).toString()+":9100");
            if(ret_bin_ip_map_itr!=ret_bin_ip_map.end()){
                //找到了
                ret_bin_ip_map_itr.value().bins.append(bins);
                ret_bin_ip_map_itr.value().price+=dcp.price;
            } else {
                //没找到
                BINS_IP bin_ip_tmp;
                bin_ip_tmp.bins.append(bins_title+bins);
                bin_ip_tmp.ip=query.record().value(1).toString()+":9100";
                bin_ip_tmp.billno=_dayindanhao;
                bin_ip_tmp.datetime=n_func::f_get_sysdatetime();
                bin_ip_tmp.key=key;
                bin_ip_tmp.price=dcp.price;
                ret_bin_ip_map.insert(bin_ip_tmp.ip, bin_ip_tmp);
            }
            bins.clear();
        }
    }///for

    for(ret_bin_ip_map_itr=ret_bin_ip_map.begin();
        ret_bin_ip_map_itr!=ret_bin_ip_map.end();
        ret_bin_ip_map_itr++){
        if(kdcontent.about_tail.count()>0
                &&k_data_content_mode.about_tail.count()>0){
            ret_bin_ip_map_itr.value().bins.append(backheader::printer->filldata_n("",'-', k_data_content_printer_width));
            QString fontsize=k_data_content_mode.about_tail[0].size;
            if(fontsize.count()!=2){//修改字体大小,版本号4.00小票打印机支持只00 10 01 11四种情况
                fontsize="00";
            }
            bins.append(backheader::printer->drawText(
                            QByteArray("1D21").append(fontsize),
                            kdcontent.about_tail[0].pre+":"+QString().sprintf("%.02f", ret_bin_ip_map_itr.value().price)+"\n"
                        ));
            bins.append(backheader::printer->drawText(
                            "1D2100"));
        }
        bins.append(backheader::printer->drawText("1B6405 1D5601", ""));//走纸5x， 切纸
        ret_bin_ip_map_itr.value().bins.append(bins);
        bins.clear();
    }
    return ret_bin_ip_map.values();
}

void w_bt_dish_Kitchenprinter::one_kdata_stragety_print(const QList<BINS_IP> &bins_ip_list)
{
    QSqlQuery query(QSqlDatabase::database("db_kitchen"));
    QList<BINS_IP> bins_ip_list0=bins_ip_list;
    QString vch_billno;
    while(bins_ip_list0.count()!=0){
        QString ip=bins_ip_list0[0].ip;
        for(int index=0;
            index<bins_ip_list0.count();){
            if(ip==bins_ip_list0[index].ip){
                //1
                QByteArray byte;
                QDataStream in(&byte, QIODevice::WriteOnly);
                foreach(pieceIns ins, bins_ip_list0[index].bins){
                    in<<ins.first;
                    in<<ins.second;
                }
                //2
                int int_id;
                query.exec("select max(int_id) from loss_form_table");
                query.next();
                int_id = query.record().value(0).toInt()+1;
                //3
                query.prepare("insert into loss_form_table(int_id, vch_billno,vch_ip,vch_content, dt_date, vch_type) values(?, ?, ?, ?, ?, ?)");
                query.addBindValue(int_id);
                query.addBindValue(bins_ip_list0[index].billno);
                query.addBindValue(bins_ip_list0[index].ip);
                query.addBindValue(byte);
                query.addBindValue(bins_ip_list0[index].datetime);
                query.addBindValue(bins_ip_list0[index].key);
                query.exec();
                //4
                if(vch_billno.isEmpty())vch_billno=bins_ip_list0[index].billno;
                bins_ip_list0.removeAt(index);
            }
            index++;//没找到的话菜++;否则remove相当于++了
        }
    }

    //5
    w_bt_dish_Kitchenprinter_thread *t=new w_bt_dish_Kitchenprinter_thread;
    t->vch_billno=vch_billno;
    connect(t,SIGNAL(printererr(QString, QString)),public_sql::save_login,SLOT(showprinterr(QString, QString)));
    connect(t,SIGNAL(finished()),t,SLOT(deleteLater()));
    QObject().moveToThread(t);
    t->start();
}

QString w_bt_dish_Kitchenprinter::value_of_kitchenkey(const QString &key)
{
    if(key.isEmpty()) return tr("厨打");
    return tr("出品");
}

void w_bt_dish_Kitchenprinter::tokitchenprint()
{
    if(tablemodel->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?")+tablemodel->dirty_err_string(), tr("确认"),tr("取消"))){
            if(!tosave()){
                return;
            }
        }
    }
    //    if(tablemodel->model_is_dirty()){
    //        if(!tosave()) return;
    //    }
    w_bt_dish_Kitchenprinter_printermanager dialog(this);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}


void w_bt_dish_Kitchenprinter::toparamset()
{
    w_bt_dish_Kitchenprinter_param dialog(this);
    dialog.resize(backheader::mainwindowsize);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

void w_bt_dish_Kitchenprinter::toexit()
{
    if(tablemodel->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?")+tablemodel->dirty_err_string(), tr("确认"),tr("取消"))){
            if(!tosave()){
                ui->treeWidget->select_beforerow();
                return;
            }
        }
    }
    //    if(tablemodel->model_is_dirty()){
    //        if(!tosave()) return;
    //    }
    this->reject();
}

bool w_bt_dish_Kitchenprinter::tosave()
{
    //! <--transaction begin-->
    QSqlDatabase::database().transaction();
    if(tablemodel->submitAll()){
        QSqlDatabase::database().commit();
        treeselect();
        lds_messagebox::warning(this, tr("消息提示"),tr("保存成功!"));
        return true;
    } else {
        QSqlDatabase::database().rollback();
        treeselect();
        lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败!"));
        return false;
    }
}

void w_bt_dish_Kitchenprinter::torefresh()
{
    //treewidget
    ui->treeWidget->deleteAllItem();
    QSqlQuery query;
    query.exec(
                "select nametype from("
                "select concat(vch_dish_typename,'[',ch_dish_typeno,']') as 'nametype' , ch_dish_typeno from cey_bt_dish_type"
                ")t  group by nametype order by ch_dish_typeno asc"
                );
    while(query.next()){
        lds_treeWidgetItem *item=new lds_treeWidgetItem(QStringList()
                                                        <<QString("%1")
                                                        .arg(query.record().value("nametype").toString())
                                                        );
        ui->treeWidget->rootitem->addChild(item);
        //获取[]的内容
        QSqlQuery query_1;
        QString no=query.record().value("nametype").toString();
        no=no.split("[")[1];
        no.chop(1);
        query_1.exec(QString(
                         "select concat(vch_sub_typename,'[',ch_sub_typeno,']') as 'nametype' from cey_bt_dish_subtype where ch_dish_typeno = '%1' order by ch_sub_typeno asc;"
                         )
                     .arg(no)
                     );
        while(query_1.next()){
            lds_treeWidgetItem *item1=new lds_treeWidgetItem(QStringList()
                                                             <<QString("%1")
                                                             .arg(query_1.record().value("nametype").toString())
                                                             );
            item->addChild(item1);
        }
    }

    ui->treeWidget->restore();
    //tableview
    tablemodel->select();
    tablemodel->relationModel(4)->select();
    tablemodel->relationModel(5)->select();
    tablemodel->relationModel(14)->select();
    tablemodel->relationModel(28)->select();
    tablemodel->relationModel(34)->select();
    tablemodel->relationModel(13)->select();
}

void w_bt_dish_Kitchenprinter::treeselect()
{
    QString sqlselect;
    //tree检索
    if(ui->treeWidget->currentItem()){
        int index=ui->treeWidget->deepofitem(ui->treeWidget->currentItem());
        if(index>0){
            QString text=ui->treeWidget->currentItem()->text(0);
            sqlselect=text.split("[")[1];
            sqlselect.chop(1);
            if(index==1){
                sqlselect=QString("cey_bt_dish.ch_dish_typeno='%1'").arg(sqlselect);
            } else if(index==2){
                sqlselect=QString("cey_bt_dish.ch_sub_typeno='%1'").arg(sqlselect);
            }
        }
    }
    if(sqlselect.isEmpty()){

    } else {
        sqlselect +=" and ";
    }
    //是否停用
    sqlselect +=QString("cey_bt_dish.ch_stopflag like '%1' ").arg("N");
    //过滤
    sqlselect += QString(" and (cey_bt_dish.ch_dishno like '%1%' or cey_bt_dish.vch_dishname like '%1%' or cey_bt_dish.vch_spell like '%1%')")
            .arg(ui->lineEdit->text());
    tablemodel->setFilter(sqlselect);
    //检索
    tablemodel->select();
    tablemodel->relationModel(4)->select();
    tablemodel->relationModel(5)->select();
    tablemodel->relationModel(14)->select();
    tablemodel->relationModel(28)->select();
    tablemodel->relationModel(34)->select();
    tablemodel->relationModel(13)->select();
}

void w_bt_dish_Kitchenprinter::treeclicked()
{
    if(tablemodel->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?")+tablemodel->dirty_err_string(), tr("确认"),tr("取消"))){
            if(!tosave()){
                return;
            }
        }
    }
    //    if(tablemodel->model_is_dirty()
    //            &&!tosave())return;
    treeselect();
}

void w_bt_dish_Kitchenprinter::tonews()
{
    if(tablemodel->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?")+tablemodel->dirty_err_string(), tr("确认"),tr("取消"))){
            if(!tosave()){
                return;
            }
        }
    }
    //    if(tablemodel->model_is_dirty()
    //            &&!tosave()) return;

    w_bt_dish_Kitchenprinter_printermanager_news dialog(this);
    backheader::static_dialogexec(dialog,qobject_cast<QAbstractButton* >(sender())->text());
}

//=================================
#include "ui_w_bt_dish_Kitchenprinter_param_dialog.h"
#include <QSettings>
w_bt_dish_Kitchenprinter_param::w_bt_dish_Kitchenprinter_param(QWidget *parent):QDialog(parent)
  ,ui(new Ui_w_bt_dish_Kitchenprinter_param_Dialog)
{
    ui->setupUi(this);
    backsetting=new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);

    //! toolbar
    ui->widget_1->setup(QStringList()
                        << tr("保存") << tr("退出"), Qt::AlignRight);
    connect(ui->widget_1->index_widget(tr("保存")), SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget_1->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->checkBox_2,SIGNAL(clicked(bool)),this,SLOT(updatecheck4(bool)));
    connect(ui->checkBox_4,SIGNAL(clicked(bool)),this,SLOT(updatecheck2(bool)));

    connect(ui->checkBox_76,SIGNAL(clicked(bool)),this,SLOT(updatecheck77(bool)));
    connect(ui->checkBox_77,SIGNAL(clicked(bool)),this,SLOT(updatecheck76(bool)));

    loadini();

    //! after
    ui->tabWidget->setObjectName("");
    ui->checkBox_5->hide();
    ui->checkBox_6->hide();
    ui->label_8->hide();
    ui->label_9->hide();
    ui->comboBox_4->hide();
    ui->comboBox_7->hide();
}

w_bt_dish_Kitchenprinter_param::~w_bt_dish_Kitchenprinter_param()
{
    delete backsetting;
}

void w_bt_dish_Kitchenprinter_param::loadini()
{
    //ta1
    //转台厨打  checkBox
    widgetfromini("w_bt_dish_Kitchenprinter_param/kitchen_turn_check", ui->checkBox);
    //套菜厨打方式  comboBox
    widgetfromini("w_bt_dish_Kitchenprinter_param/suit_printtype_com", ui->comboBox);
    //菜品打印方式  comboBox_3
    widgetfromini("w_bt_dish_Kitchenprinter_param/dish_printtype_com", ui->comboBox_3);
    //厨打份数  comboBox_2
    widgetfromini("w_bt_dish_Kitchenprinter_param/kitchen_num_spin", ui->spinBox);
    //一桌一单时打印菜品大类  checkBox_2
    widgetfromini("w_bt_dish_Kitchenprinter_param/table_bill_printseries_check", ui->checkBox_2);
    //一桌一单时打印菜品小类  checkBox_4
    widgetfromini("w_bt_dish_Kitchenprinter_param/table_bill_printtype_check", ui->checkBox_4);
    //一菜一单时打印菜品小类  checkBox_3
    widgetfromini("w_bt_dish_Kitchenprinter_param/dish_bill_printtype_check", ui->checkBox_3);
    //ta2
    //打印菜品类型(厨打单第一个菜品的类型)  checkBox_5
    widgetfromini("w_bt_dish_Kitchenprinter_param/print_dishtype_check", ui->checkBox_5);
    //打印菜单类型(如点菜,加菜单等)  checkBox_6
    widgetfromini("w_bt_dish_Kitchenprinter_param/print_dishbill_check", ui->checkBox_6);
    //字体  comboBox_4
    widgetfromini("w_bt_dish_Kitchenprinter_param/printer_fontsize_com", ui->comboBox_4);
    //打印机  comboBox_7
    widgetfromini("w_bt_dish_Kitchenprinter_param/printer_type_com", ui->comboBox_7);
    //餐桌名称  checkBox_8
    widgetfromini("w_bt_dish_Kitchenprinter_param/tablename_check", ui->checkBox_8);
    //开台单号  checkBox_10
    widgetfromini("w_bt_dish_Kitchenprinter_param/openbillno_check", ui->checkBox_10);
    //点菜人  checkBox_12
    widgetfromini("w_bt_dish_Kitchenprinter_param/dishopertor_check", ui->checkBox_12);
    //点菜时间  checkBox_7
    widgetfromini("w_bt_dish_Kitchenprinter_param/dishtime_check", ui->checkBox_7);
    //打印时间  checkBox_9
    widgetfromini("w_bt_dish_Kitchenprinter_param/printtime_check", ui->checkBox_9);
    //人数  checkBox_11
    widgetfromini("w_bt_dish_Kitchenprinter_param/person_check", ui->checkBox_11);
    //流水号  checkBox_16
    widgetfromini("w_bt_dish_Kitchenprinter_param/serialno_check", ui->checkBox_16);

    //本单序号  checkBox_13
    widgetfromini("w_bt_dish_Kitchenprinter_param/selfbillno_check", ui->checkBox_13);
    //赠送标识  checkBox_14
    widgetfromini("w_bt_dish_Kitchenprinter_param/presentflag_check", ui->checkBox_14);

    //品名  checkBox_15 字符,第 spinBox_3  行 spinBox_4
    widgetfromini("w_bt_dish_Kitchenprinter_param/ch_dishno_check", ui->checkBox_15);
    widgetfromini("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin", ui->spinBox_3);
    widgetfromini("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin", ui->spinBox_4);
    //品码  checkBox_31 字符,第 spinBox_10 行 spinBox_9
    widgetfromini("w_bt_dish_Kitchenprinter_param/vch_dishname_check", ui->checkBox_31);
    widgetfromini("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin", ui->spinBox_10);
    widgetfromini("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin", ui->spinBox_9);
    //数量  checkBox_32 字符,第 spinBox_13 行 spinBox_11 小数位  spinBox_17
    widgetfromini("w_bt_dish_Kitchenprinter_param/num_num_check", ui->checkBox_32);
    widgetfromini("w_bt_dish_Kitchenprinter_param/num_num_size_spin", ui->spinBox_13);
    widgetfromini("w_bt_dish_Kitchenprinter_param/num_num_hang_spin", ui->spinBox_11);
    widgetfromini("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin", ui->spinBox_17);
    //价格  checkBox_33 字符,第 spinBox_12 行 spinBox_14 小数位  spinBox_18
    widgetfromini("w_bt_dish_Kitchenprinter_param/num_price_check", ui->checkBox_33);
    widgetfromini("w_bt_dish_Kitchenprinter_param/num_price_size_spin", ui->spinBox_12);
    widgetfromini("w_bt_dish_Kitchenprinter_param/num_price_hang_spin", ui->spinBox_14);
    widgetfromini("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin", ui->spinBox_18);

    //单位  checkBox_34 字符,第 spinBox_16 行 spinBox_15 小数位  spinBox_18
    widgetfromini("w_bt_dish_Kitchenprinter_param/vch_unit_check", ui->checkBox_34);
    widgetfromini("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin", ui->spinBox_16);
    widgetfromini("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin", ui->spinBox_15);

    //菜品金额合计 checkBox_35
    widgetfromini("w_bt_dish_Kitchenprinter_param/dishsum_numprice_check", ui->checkBox_35);
    ui->comboBox_paperwidth->addItems(QStringList()<<"58"<<"80");
    ui->comboBox_paperwidth->setCurrentIndex(backsetting->value("w_bt_dish_Kitchenprinter_param/paperwidth", 0).toInt());
    //tab3
    //转台出品打印 checkBox_79
    widgetfromini("w_bt_dish_Kitchenprinter_param/out_kitchen_turn_check", ui->checkBox_79);
    //套菜出品打印方式 comboBox_25
    widgetfromini("w_bt_dish_Kitchenprinter_param/out_suit_printtype_com", ui->comboBox_25);
    //菜品打印方式 comboBox_26
    widgetfromini("w_bt_dish_Kitchenprinter_param/out_dish_printtype_com", ui->comboBox_26);
    //出品打印份数 spinBox_2
    widgetfromini("w_bt_dish_Kitchenprinter_param/out_kitchen_num_spin", ui->spinBox_2);

    //一桌一单时打印菜品大类  checkBox_77
    widgetfromini("w_bt_dish_Kitchenprinter_param/out_table_bill_printseries_check", ui->checkBox_77);
    //一桌一单时打印菜品小类  checkBox_76
    widgetfromini("w_bt_dish_Kitchenprinter_param/out_table_bill_printtype_check", ui->checkBox_76);
    //一菜一单时打印菜品小类  checkBox_78
    widgetfromini("w_bt_dish_Kitchenprinter_param/out_dish_bill_printtype_check", ui->checkBox_78);

    //餐桌名称
    ui->lineEdit_6->setText(backsetting->value("w_bt_dish_Kitchenprinter_param/tablename_check_size", "00").toString());
    //开台单号
    ui->lineEdit_5->setText(backsetting->value("w_bt_dish_Kitchenprinter_param/openbillno_check_size", "00").toString());
    //点菜人
    ui->lineEdit->setText(backsetting->value("w_bt_dish_Kitchenprinter_param/dishopertor_check_size", "00").toString());
    //点菜时间
    ui->lineEdit_7->setText(backsetting->value("w_bt_dish_Kitchenprinter_param/dishtime_check_size", "00").toString());
    //打印时间
    ui->lineEdit_4->setText(backsetting->value("w_bt_dish_Kitchenprinter_param/printtime_check_size", "00").toString());
    //人数
    ui->lineEdit_3->setText(backsetting->value("w_bt_dish_Kitchenprinter_param/person_check_size", "00").toString());
    //流水号
    ui->lineEdit_2->setText(backsetting->value("w_bt_dish_Kitchenprinter_param/serialno_check_size", "00").toString());
    //内容
    ui->lineEdit_8->setText(backsetting->value("w_bt_dish_Kitchenprinter_param/printer_content_size", "00").toString());
    //表尾
    ui->lineEdit_9->setText(backsetting->value("w_bt_dish_Kitchenprinter_param/printer_tail_size", "00").toString());

}

void w_bt_dish_Kitchenprinter_param::saveini()
{
    //ta1
    //转台厨打  checkBox
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/kitchen_turn_check", ui->checkBox->isChecked()?"1":"0");
    //套菜厨打方式  comboBox
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/suit_printtype_com", ui->comboBox->currentIndex());
    //菜品打印方式  comboBox_3
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/dish_printtype_com", ui->comboBox_3->currentIndex());
    //厨打份数  comboBox_2
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/kitchen_num_spin", ui->spinBox->value());
    //一桌一单时打印菜品大类  checkBox_2
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/table_bill_printseries_check", ui->checkBox_2->isChecked()?"1":"0");
    //一桌一单时打印菜品小类  checkBox_4
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/table_bill_printtype_check", ui->checkBox_4->isChecked()?"1":"0");
    //一菜一单时打印菜品小类  checkBox_3
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/dish_bill_printtype_check", ui->checkBox_3->isChecked()?"1":"0");
    //ta2
    //打印菜品类型(厨打单第一个菜品的类型)  checkBox_5
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/print_dishtype_check", ui->checkBox_5->isChecked()?"1":"0");
    //打印菜单类型(如点菜,加菜单等)  checkBox_6
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/print_dishbill_check", ui->checkBox_6->isChecked()?"1":"0");
    //字体  comboBox_4
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/printer_fontsize_com", ui->comboBox_4->currentIndex());
    //打印机  comboBox_7
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/printer_type_com", ui->comboBox_7->currentIndex());
    //餐桌名称  checkBox_8
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/tablename_check", ui->checkBox_8->isChecked()?"1":"0");
    //开台单号  checkBox_10
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/openbillno_check", ui->checkBox_10->isChecked()?"1":"0");
    //点菜人  checkBox_12
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/dishopertor_check", ui->checkBox_12->isChecked()?"1":"0");
    //点菜时间  checkBox_7
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/dishtime_check", ui->checkBox_7->isChecked()?"1":"0");
    //打印时间  checkBox_9
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/printtime_check", ui->checkBox_9->isChecked()?"1":"0");
    //人数  checkBox_11
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/person_check", ui->checkBox_11->isChecked()?"1":"0");
    //流水号  checkBox_16
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/serialno_check", ui->checkBox_16->isChecked()?"1":"0");

    //本单序号  checkBox_13
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/selfbillno_check", ui->checkBox_13->isChecked()?"1":"0");
    //赠送标识  checkBox_14
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/presentflag_check", ui->checkBox_14->isChecked()?"1":"0");

    //品名  checkBox_15 字符,第 spinBox_3  行 spinBox_4
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/ch_dishno_check", ui->checkBox_15->isChecked()?"1":"0");
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/ch_dishno_size_spin", ui->spinBox_3->value());
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/ch_dishno_hang_spin", ui->spinBox_4->value());
    //品码  checkBox_31 字符,第 spinBox_10 行 spinBox_9
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/vch_dishname_check", ui->checkBox_31->isChecked()?"1":"0");
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/vch_dishname_size_spin", ui->spinBox_10->value());
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/vch_dishname_hang_spin", ui->spinBox_9->value());
    //数量  checkBox_32 字符,第 spinBox_13 行 spinBox_11 小数位  spinBox_17
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/num_num_check", ui->checkBox_32->isChecked()?"1":"0");
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/num_num_size_spin", ui->spinBox_13->value());
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/num_num_hang_spin", ui->spinBox_11->value());
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/num_num_xiaoshu_spin", ui->spinBox_17->value());
    //价格  checkBox_33 字符,第 spinBox_12 行 spinBox_14 小数位  spinBox_18
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/num_price_check", ui->checkBox_33->isChecked()?"1":"0");
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/num_price_size_spin", ui->spinBox_12->value());
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/num_price_hang_spin", ui->spinBox_14->value());
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/num_price_xiaoshu_spin", ui->spinBox_18->value());

    //单位  checkBox_34 字符,第 spinBox_16 行 spinBox_15 小数位  spinBox_18
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/vch_unit_check", ui->checkBox_34->isChecked()?"1":"0");
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/vch_unit_size_spin", ui->spinBox_16->value());
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/vch_unit_hang_spin", ui->spinBox_15->value());

    //菜品金额合计 checkBox_35
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/dishsum_numprice_check", ui->checkBox_35->isChecked()?"1":"0");
    //厨打纸张58
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/paperwidth", ui->comboBox_paperwidth->currentIndex());

    //tab3
    //转台出品打印 checkBox_79
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/out_kitchen_turn_check", ui->checkBox_79->isChecked()?"1":"0");
    //套菜出品打印方式 comboBox_25
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/out_suit_printtype_com", ui->comboBox_25->currentIndex());
    //菜品打印方式 comboBox_26
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/out_dish_printtype_com", ui->comboBox_26->currentIndex());
    //出品打印份数 spinBox_2
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/out_kitchen_num_spin", ui->spinBox_2->value());

    //一桌一单时打印菜品大类  checkBox_77
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/out_table_bill_printseries_check", ui->checkBox_77->isChecked()?"1":"0");
    //一桌一单时打印菜品小类  checkBox_76
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/out_table_bill_printtype_check", ui->checkBox_76->isChecked()?"1":"0");
    //一菜一单时打印菜品小类  checkBox_78
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/out_dish_bill_printtype_check", ui->checkBox_78->isChecked()?"1":"0");

    //餐桌名称
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/tablename_check_size", ui->lineEdit_6->text());
    //开台单号
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/openbillno_check_size", ui->lineEdit_5->text());
    //点菜人
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/dishopertor_check_size", ui->lineEdit->text());
    //点菜时间
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/dishtime_check_size", ui->lineEdit_7->text());
    //打印时间
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/printtime_check_size", ui->lineEdit_4->text());
    //人数
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/person_check_size", ui->lineEdit_3->text());
    //流水号
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/serialno_check_size", ui->lineEdit_2->text());
    //内容
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/printer_content_size", ui->lineEdit_8->text());
    //表尾
    backsetting->setValue("w_bt_dish_Kitchenprinter_param/printer_tail_size", ui->lineEdit_9->text());
}

void w_bt_dish_Kitchenprinter_param::widgetfromini(const QString &path, QCheckBox *checkbox)
{
    checkbox->setChecked(backsetting->value(path).toString()=="1");
}

void w_bt_dish_Kitchenprinter_param::widgetfromini(const QString &path, QComboBox *combobox)
{
    combobox->setCurrentIndex(backsetting->value(path).toInt());
}

void w_bt_dish_Kitchenprinter_param::widgetfromini(const QString &path, QSpinBox *spinbox)
{
    spinbox->setValue(backsetting->value(path).toInt());
}

void w_bt_dish_Kitchenprinter_param::updatecheck4(bool f)
{
    if(ui->checkBox_2->isChecked() && f){
        ui->checkBox_4->setChecked(false);
    }
}

void w_bt_dish_Kitchenprinter_param::updatecheck2(bool f)
{
    if(ui->checkBox_4->isChecked() && f){
        ui->checkBox_2->setChecked(false);
    }}

void w_bt_dish_Kitchenprinter_param::updatecheck77(bool f)
{
    if(ui->checkBox_76->isChecked() && f){
        ui->checkBox_77->setChecked(false);
    }}

void w_bt_dish_Kitchenprinter_param::updatecheck76(bool f)
{
    if(ui->checkBox_77->isChecked() && f){
        ui->checkBox_76->setChecked(false);
    }
}

void w_bt_dish_Kitchenprinter_param::toexit()
{
    this->reject();
}

bool w_bt_dish_Kitchenprinter_param::tosave()
{
    saveini();
    backsetting->sync();
    w_bt_dish_Kitchenprinter::k_data_content_mode_init();
    this->accept();
}

//===================
#include"ui_w_bt_dish_unitset_dialog.h"
#include "lds_model_sqlrelationaltablemodel.h"
#include "lds_tableview_delegate_check.h"
#include "backheader.h"
#include "lds_messagebox.h"
#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>
w_bt_dish_Kitchenprinter_printermanager::w_bt_dish_Kitchenprinter_printermanager(w_bt_dish_Kitchenprinter *parent) :
    QDialog(parent),
    ui(new Ui_w_bt_dish_unitset_Dialog),
    _parent(parent)
{
    ui->setupUi(this);
    //! tableview
    tablemodel=new lds_model_sqlrelationaltablemodel(this);
    tablemodel->setTable("cey_bt_kitchen_print");
    tablemodel->setEditStrategy(QSqlTableModel::OnManualSubmit);tablemodel->setJoinMode(QSqlRelationalTableModel::LeftJoin);
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);ui->tableView->setModel(tablemodel->fpmodel);
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif
    tablemodel->save_set_header("int_id", tr("编码"),true);
    tablemodel->save_set_header("vch_printname", tr("打印机名称"),true);
    tablemodel->save_set_header("vch_printip", tr("打印机IP"),true);
    //    tablemodel->save_set_header("vch_printport", tr("打印机PORT"),true);
    //    tablemodel->save_set_header("vch_printother", tr("打印测试"));
    ui->tableView->rePecifyHeader(tablemodel->save_set_header_saver);

    //! toolbar
    ui->widget->setup(QStringList() << tr("打印测试") <<tr("新增") << tr("删除") <<  tr("刷新") << tr("保存") << tr("退出"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("新增")), SIGNAL(clicked()),this,SLOT(tonew()));
    connect(ui->widget->index_widget(tr("删除")), SIGNAL(clicked()),this,SLOT(todel()));
    connect(ui->widget->index_widget(tr("刷新")), SIGNAL(clicked()),this,SLOT(torefresh()));
    connect(ui->widget->index_widget(tr("保存")), SIGNAL(clicked()),this,SLOT(tosave()));
    connect(ui->widget->index_widget(tr("退出")), SIGNAL(clicked()),this,SLOT(toexit()));
    connect(ui->widget->index_widget(tr("打印测试")), SIGNAL(clicked()),this,SLOT(toprinttest()));

    QTimer::singleShot(100, this, SLOT(torefresh()));
}

void w_bt_dish_Kitchenprinter_printermanager::tonew()
{
    int rowcount=tablemodel->rowCount();
    tablemodel->insertRow(rowcount);
    tablemodel->setData(tablemodel->index(rowcount, 0), tablemodel->max_column_add_1("int_id"));
}

void w_bt_dish_Kitchenprinter_printermanager::todel()
{
    tablemodel->removeRow(ui->tableView->currentIndex().row());
    tablemodel->submitAll();
}

void w_bt_dish_Kitchenprinter_printermanager::torefresh()
{
    tablemodel->select();
}

bool w_bt_dish_Kitchenprinter_printermanager::tosave()
{
    for(int r=0,r_count=tablemodel->rowCount();r<r_count;r++){
        if(tablemodel->record(r).value("int_id").toString().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("打印机编号不能为空!"));
            return false;
        }
        if(tablemodel->record(r).value("vch_printname").toString().isEmpty()){
            lds_messagebox::warning(this, tr("消息提示"),tr("打印机名字不能为空!"));
            return false;
        }

        if(tablemodel->same_column_exist("int_id")){
            lds_messagebox::warning(this, tr("消息提示"),tr("打印机编号不能重复!"));
            return false;
        }
        if(tablemodel->same_column_exist("vch_printname")){
            lds_messagebox::warning(this, tr("消息提示"),tr("打印机名字不能重复!"));
            return false;
        }
        if(tablemodel->same_column_exist("vch_printip")){
            lds_messagebox::warning(this, tr("消息提示"),tr("打印IP不能重复!"));
            return false;
        }
    }
    if(tablemodel->submitAll()){
        lds_messagebox::warning(this, tr("消息提示"),tr("保存成功"));
        if(_parent)_parent->torefresh();
        return true;
    } else {
        lds_messagebox::warning(this, tr("消息提示"),tr("数据有误，操作失败"), tablemodel->lastError().text());
        return false;
    }
}

void w_bt_dish_Kitchenprinter_printermanager::toexit()
{
    if(tablemodel->model_is_dirty()){
        if(0==lds_messagebox::warning(this, tr("消息提示"),tr("有数据未保存,确认保存?")+tablemodel->dirty_err_string(), tr("确认"),tr("取消"))){
            if(!tosave()){
                return;
            }
        }
    }
    //    if(tablemodel->model_is_dirty()
    //            &&!tosave())return;
    this->reject();
}

void w_bt_dish_Kitchenprinter_printermanager::toprinttest()
{
    QString path=tablemodel->record(ui->tableView->currentIndex().row()).value(2).toString();
    if(path.isEmpty()) return;
    path=path+":9100";
    backheader::syn_dataprin_test(path);
}

//=====================================
#include "ui_w_bt_dish_Kitchenprinter_printermanager_news_dialog.h"
w_bt_dish_Kitchenprinter_printermanager_news::w_bt_dish_Kitchenprinter_printermanager_news(w_bt_dish_Kitchenprinter *parent):
    QDialog(parent),
    ui(new Ui_w_bt_dish_Kitchenprinter_printermanager_news_Dialog),
    _parent(parent)
{
    ui->setupUi(this);

    //! toolbar
    ui->widget->setup(QStringList() << tr("确定") << tr("取消"), Qt::AlignRight);
    connect(ui->widget->index_widget(tr("确定")), SIGNAL(clicked()),this,SLOT(took()));
    connect(ui->widget->index_widget(tr("取消")), SIGNAL(clicked()),this,SLOT(toexit()));

    QSqlTableModel *tablemodel=_parent->tablemodel->relationModel(13);
    for(int r=0,r_count=tablemodel->rowCount();r<r_count;r++){
        ui->comboBox->addItem(tablemodel->record(r).value(1).toString(),tablemodel->record(r).value(2).toString());
        ui->comboBox_2->addItem(tablemodel->record(r).value(1).toString(),tablemodel->record(r).value(2).toString());
    }
}

void w_bt_dish_Kitchenprinter_printermanager_news::took()
{
    QString ch_printflag="N";
    QString vch_printer;
    QString ch_outflag="N";
    QString vch_outprint;

    if(ui->checkBox_chuda->isChecked()){
        ch_printflag="Y";
        vch_printer=ui->comboBox->itemData(ui->comboBox->currentIndex()).toString();
    }
    if(ui->checkBox_chuping->isChecked()){
        ch_outflag="Y";
        vch_outprint=ui->comboBox_2->itemData(ui->comboBox_2->currentIndex()).toString();
    }

    for(int r=0,r_count=_parent->tablemodel->rowCount();r<r_count;r++){
        _parent->tablemodel->set_row_data(r, "ch_printflag",ch_printflag);
        _parent->tablemodel->set_row_data(r, "cey_bt_kitchen_print_vch_printname_2", vch_printer);
    }
    for(int r=0,r_count=_parent->tablemodel->rowCount();r<r_count;r++){
        _parent->tablemodel->set_row_data(r, "ch_outflag", ch_outflag);
        _parent->tablemodel->set_row_data(r, "vch_printname", vch_outprint);
    }

    this->accept();
}

void w_bt_dish_Kitchenprinter_printermanager_news::toexit()
{
    this->reject();
}


w_bt_dish_Kitchenprinter_check_com::w_bt_dish_Kitchenprinter_check_com(QVector<int> &check_pos, const QString &indexname, QObject *parent):
    lds_tableview_delegate_check(check_pos, parent)
{

}

QWidget *w_bt_dish_Kitchenprinter_check_com::createEditor(QWidget *aParent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 13
            || index.column() == 34
            ){
        lds_ComboBox *editor = new lds_ComboBox(aParent);
        editor->setModel(tablemodel->relationModel(index.column()));
        editor->setModelColumn(1);
        editor->setCurrentIndex(editor->findText(index.data().toString()));
        return editor;
    }
    return lds_tableview_delegate_check::createEditor(aParent, option, index);
}

void w_bt_dish_Kitchenprinter_check_com::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    lds_ComboBox *dateEditor = qobject_cast<lds_ComboBox *>(editor);
    if(dateEditor){
        model->setData(index, dateEditor->model()->index(dateEditor->currentIndex(), 2).data().toString());

    } else {
        lds_tableview_delegate_check::setModelData(editor, model, index);
    }
}

void w_bt_dish_Kitchenprinter_check_com::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    lds_ComboBox *dateEditor = qobject_cast<lds_ComboBox *>(editor);
    if(dateEditor){

    } else {
        lds_tableview_delegate_check::setEditorData(editor, index);
    }
}

//==================
w_bt_dish_Kitchenprinter_thread::~w_bt_dish_Kitchenprinter_thread()
{

}

void w_bt_dish_Kitchenprinter_thread::tomanageprinter(const QString &vch_billno)
{
    {
        QSettings conf(public_sql::settings_ini_dir_file, QSettings::IniFormat);
        int timeout=conf.value("system_setting/kitchen_timeout", 200).toInt();
        qDebug() << "timeout" << timeout;
        QSqlDatabase db=QSqlDatabase::cloneDatabase(QSqlDatabase::database(), "KITCHEN_NET_PRINT");
        QSqlDatabase db_local=QSqlDatabase::cloneDatabase(QSqlDatabase::database("db_kitchen"), "KITCHEN_NET_PRINT2");
        if(db.open() && db_local.open()){
            Printer_POS_NET printer;
            QSqlQuery query_local(db_local);
            QSqlQuery query(db);
            QSqlQuery query_local2(db_local);

            query_local.exec(QString("select *from loss_form_table where vch_billno like '%1%' order by int_id asc;")
                             .arg(vch_billno));
            while(query_local.next()){///while(query_local.next()){
                if(printer.getipport().isEmpty()
                        || printer.getipport()!=printer.tcpSocket->peerAddress().toString()){
                    //1
                    printer.closeconnect();
                    QEventLoop eloop;
                    QTimer::singleShot(timeout, &eloop,SLOT(quit()));
                    eloop.exec(QEventLoop::ExcludeUserInputEvents);
                    //2
                    if(false==printer.tryOpen(query_local.record().value("vch_ip").toString())){
                        emit printererr(printer.lastError(), tr("厨打信息tryopen"));
                        continue;
                    }
                }
                printer.writeHex("100402");
                if(printer.waitForReadyRead()
                        &&printer.tcpSocket->readAll().startsWith(QByteArray::fromHex("12"))){
                    QByteArray byte=query_local.record().value("vch_content").toByteArray();
                    QDataStream in(&byte, QIODevice::ReadOnly);
                    blockIns printdata;
                    while(!in.atEnd()){
                        pieceIns pins;
                        in>>pins.first;
                        in>>pins.second;
                        printdata.append(pins);
                    }
                    foreach(pieceIns ins, printdata){
                        printer.writeHex(ins.first);
                        printer.write(ins.second);
                    }
                    qDebug() << "======================================"<<printer.tcpSocket->readAll().toHex();
                    //success
                    //1
                    if(false==query.exec(QString("update cey_u_orderdish set %1 ='Y' where ch_billno = '%2' ")
                                         .arg(query_local.record().value("vch_type").toString().isEmpty()?"ch_printflag":"ch_outflag")
                                         .arg(query_local.record().value("vch_billno").toString())
                                         )){
                        emit printererr(query.lastError().text(), tr("厨打信息update"));
                        continue;
                    }
                    //2
                    query_local2.exec(QString("delete from loss_form_table where int_id='%1' ")
                                      .arg(query_local.record().value("int_id").toString()));
                }
                //                failure
            }///while(query_local.next()){
        }
    }
    QSqlDatabase::removeDatabase("KITCHEN_NET_PRINT");
    QSqlDatabase::removeDatabase("KITCHEN_NET_PRINT2");
}

void w_bt_dish_Kitchenprinter_thread::run() // 3j6hg9
{
    if(print_thread::mutex.tryLock(1000)){
        tomanageprinter(vch_billno);
        print_thread::mutex.unlock();
    }
}
