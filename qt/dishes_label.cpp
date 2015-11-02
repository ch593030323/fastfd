#include "dishes_label.h"
#include <QDebug>

extern int label_press = 0;
extern int label_dish = 0;

Base_label::Base_label(int x, int y, const QString &text,
                       const QString & m1,const QString & m2, QWidget *parent):
    QLabel(parent),p1(m1),p2(m2)
{
    this->setFixedSize(105,60);
    //"行号"<<"品码"<<"品名"<<"数量"<<"价格"<<"折扣"<<"折扣(%)"<<"金额"<<"厨打说明"<<"点菜行号"<<"套菜"<<"特价"<<"赠送"<<"优惠券";
#ifdef OFFLINE//若是offline
    info/*offline*/<<"0202"<<"2 0 5 "<<"苹果"<<"1.00"<<"36.0"<<"8.00"<<"80.0"<<"35.0"<<"??"<<"1.00"  <<"N"  <<"N"  <<"N"  <<"N";
#endif
    xx = x;
    yy = y;
    QFont font;
    lable_bottom = new QLabel(this);
    lable_between = new QLabel(this);
    lable_between->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    //lable_top = new QLabel(this);
    lable_bottom->setAutoFillBackground(true);
    // lable_bottom->setPixmap(QPixmap(p1));
    lable_bottom->setStyleSheet(p1);
    //    lable_bottom->setStyleSheet("background-color:rgb(0,255,255)");
    lable_bottom->setGeometry(QRect(0, 0, 100, 100));
    lable_bottom->setScaledContents(true);

    // lable_between->textsetFont(font);
    lable_between->setGeometry(QRect(0, 0, 100, 100));
    lable_between->setStyleSheet("color:white");
#ifdef OFFLINE//若是offline
    lable_between->setText(QString("%1\n%2\n%3").arg(info[1],info[2],info[4]));
#else
    lable_between->setText(text);
#endif
    font.setPointSize(20);
    //    lable_top->setFont(font);
    //    lable_top->setGeometry(0,0,20,lable_top->height());
    //    lable_top->setStyleSheet("color:rgb(255, 0, 0)");

}

void Base_label::mousePressEvent(QMouseEvent *)
{
    if(yy == 0){/*发送信号给type up*/ emit typeup();}
    if(yy == 1){/*发送信号给type down*/emit typedown();}
}

QPoint Type_label::flag= QPoint(0,0);
Type_label *Type_label::lable1 = NULL;

Type_label::Type_label(int x, int y,
                       const QString &text,
                       const QString & m1,const QString & m2,
                       QWidget *parent):
    Base_label(x, y, text, m1,m2,parent)
{
    if(QPoint(x,y) == flag) {
        lable_bottom->setStyleSheet(p2);
        lable1 = this;
    }

}

void Type_label::mousePressEvent(QMouseEvent *ev)
{
    /*控件只能点一次*/
    if(QPoint(xx,yy) != flag){
        lable_bottom->setStyleSheet(p2);
        lable1->lable_bottom->setStyleSheet(p1);
        lable1 = this;
        flag =  QPoint(xx,yy);
        label_press = 0;
        label_dish = 0;
        }
     emit toType(xx*3 + yy);
}

QPoint Dish_label::flag= QPoint(0,0);
Dish_label *Dish_label::lable1 = NULL;

Dish_label::Dish_label(int x, int y,
                       const QString &text,
                       const QString & m1,const QString & m2,
                       QWidget *parent):
    Base_label(x, y, text, m1,m2,parent)
{
    if(QPoint(x,y) == flag) {
        lable_bottom->setStyleSheet(p2);
        lable1 = this;
    }

    dish_point = QPoint(-1,-1);
}

void Dish_label::mousePressEvent(QMouseEvent *ev)
{
    //    /*控件只能点一次*/
    if(dish_point != flag)
    {
        if(QPoint(xx,yy) != flag || label_dish == 0)
        {
            //lable_bottom->setPixmap(QPixmap(p2));
            lable_bottom->setStyleSheet(p2);
            if(lable1)
                // lable1->lable_bottom->setPixmap(QPixmap(p1));
                lable1->lable_bottom->setStyleSheet(p1);
            lable1 = this;
            flag =  QPoint(xx,yy);
            emit toDish(xx*3 + yy);
            label_dish++;
            label_press = 0;
            dish_point = flag;
        }
    }
}




QPoint Cookie_label::flag= QPoint(0,0);
Cookie_label *Cookie_label::lable1 = NULL;
Cookie_label::Cookie_label(int x, int y,
                           const QString &text,
                           const QString & m1,const QString & m2,
                           QWidget *parent):
    Base_label(x, y, text, m1,m2,parent)
{

    //   if(QPoint(x,y) == flag) {
    //       // lable_bottom->setPixmap(QPixmap(p2));
    // lable1 = this;
    // emit toType(xx + yy * 2);//此时connect未完全建立
    //    }

}

void Cookie_label::mousePressEvent(QMouseEvent *)//奉送时MODELBASE改
{
    /*控件只能点一次*/

    if(QPoint(xx,yy) != flag ||  label_press == 0 /*|| label_dish == 0*/)
    {
        //lable_bottom->setPixmap(QPixmap(p2));
        lable_bottom->setStyleSheet(p2);
        if(lable1)
            lable1->lable_bottom->setStyleSheet(p1);
        lable1 = this;
        flag =  QPoint(xx,yy);

        label_press++;
        label_dish = 0;
    }
    emit tocook(xx * 4 + yy);

    //    p_sum = 0;
    //    p_row = 0;
    //    cur_sum = 0;
    //    cur_price.clear();

    //    msgBox.setText("特价菜品价格不能小于等于0!");
    //    msgBox.setStandardButtons(QMessageBox::Ok);
    //    msgBox.setButtonText(QMessageBox::Ok,"是");

    //    QStandardItemModel *& model = Main_left::modelBase;
    //    System_Tabview *&tabview = Main_left::tabview;
    //    QHeaderView* headerView = tabview->verticalHeader();
    //    headerView->setHidden(true);
    //    class QSqlQueryModel *&  model_cookie = Main_right::get_model();
    //    int i;
    //    //ch_dishno,vch_dishname,num_price,ch_suitflag,ch_discount,ch_curprice,ch_deductflag
    //    if(model_cookie->rowCount() <= 0) return;//表示没数据;
    //    cur_shijia.clear();
    //    if(model_cookie->rowCount() == 1)
    //    {
    //        cur_shijia << Main_right::one_cur;
    //    }
    //    else
    //    {
    //        for(int i = 0; i < model_cookie->rowCount(); i++)
    //        {
    //            cur_shijia << model_cookie->record(i).value(7).toString().trimmed();
    //            //        for(int k = 0; k < model_cookie->columnCount(); k++)
    //            //        {
    //            //          QMessageBox::warning(this, tr("111"),QString::number(k)+model_cookie->record(i).value(k).toString().trimmed(),QMessageBox::Yes);
    //            //        }

    //        }
    //    }


    //    if(Widget::twidth > 800)
    //    {
    //        row = xx*4 + yy;
    //    }
    //    else
    //    {
    //        row = xx*3 + yy;
    //    }
    //    QString no = model_cookie->index(row, 0).data().toString().trimmed();//若去cookie的行号
    //    cur_flag = cur_shijia[row];/*model_cookie->index(row, 7).data().toString().trimmed().toUpper();*/
    //    suit_flag = model_cookie->index(row, 3).data().toString().trimmed().toUpper();
    //    cur_price = model_cookie->index(row,2).data().toString();
    //    //根据品码，在表格中判断是追加还是加一
    //    //追加 1.普通(一行)；2.实价；3.套菜(多行)
    //    //1.普通 查找 若是子套菜，则跳过
    //    //2.{有提示窗的查找}


    //    switch(0){
    //    case 0://若 没有找到
    //        if(!0/*findItem(no)*/){
    //            if(cur_flag == "Y"){//实价(提示窗)
    //                cur_price = show_Cruprice_VIew();
    //                if(cur_price.isEmpty())return;
    //                Tabpp(model->rowCount());
    //                break;
    //            }
    //            if(suit_flag == "Y"){//套菜(多行)
    //                Tabpp(model->rowCount());
    //                break;
    //            }
    //            Tabpp(model->rowCount());//普通(一行)
    //            break;
    //        }
    //    case 1://若 找到
    //        if(cur_flag == "Y"){//若是实价，则获取输入价格
    //            cur_price = show_Cruprice_VIew();
    //            if(cur_price.isEmpty())return;
    //        }
    //        for(int i = 0; i < model->rowCount(); i++){
    //            QModelIndex index = model->index(i,1);
    //            //品吗不相同
    //            if(index.data(Qt::DisplayRole).toString().trimmed() != no.trimmed())
    //                continue;
    //            //子套菜
    //            if(index.data(Qt::TextColorRole) == Qt::green)
    //                continue;
    //            //或奉送，或优惠
    //            if(model->item(i,11)->text() == "Y" || model->item(i,12)->text() == "Y")
    //                continue;
    //            //实时价
    //            if(cur_flag == "Y"){
    //                //价格相等
    //                if(model->item(i,4)->text().trimmed().toFloat() == cur_price.toFloat())
    //                    p_sum++;
    //                continue;
    //            }
    //            //特价
    //            QString special_price = get_special_price(no);
    //            if(!special_price.isEmpty()){
    //                //价格相等
    //                cur_price = special_price;
    //                if(model->item(i,4)->text().trimmed().toFloat() == cur_price.toFloat())
    //                    p_sum++;
    //                continue;
    //            }
    //            //普通
    //            p_sum++;
    //        }
    //        //追加
    //        if(p_sum == 0){
    //            if(cur_flag == "Y"){//实价(提示窗)
    //                Tabpp(model->rowCount());
    //                break;
    //            }
    //            if(suit_flag == "Y"){//套菜(多行)
    //                Tabpp(model->rowCount());
    //                break;
    //            }
    //            //普通的
    //            Tabpp(model->rowCount());//普通(一行)
    //            break;
    //        }
    //    case 2://到底哪一行需要数量+1
    //        for(int i = 0; i < model->rowCount(); i++){
    //            QModelIndex index = model->index(i,1);
    //            if(index.data().toString().trimmed() != no.trimmed())
    //                continue;
    //            if(model->item(i,11)->text() == "Y" || model->item(i,12)->text() == "Y")
    //                continue;
    //            if(index.data(Qt::TextColorRole) == Qt::green)
    //                continue;
    //            if(cur_flag == "Y"){
    //                if(model->item(i,4)->text().trimmed().toFloat() != cur_price.toFloat())
    //                    continue;
    //                p_row = i;
    //                break;
    //            }
    //            if(suit_flag == "Y"){
    //                if(model->item(i,4)->text().trimmed().toFloat() != cur_price.toFloat())
    //                    continue;
    //                p_row = i;
    //                break;
    //            }
    //            //普通
    //            p_row = i;
    //        }
    //        if(cur_flag == "Y"){//实价(提示窗)
    //            Tabpp(p_row);
    //            break;
    //        }
    //        if(suit_flag == "Y"){//套菜(多行)
    //            Tabpp(p_row);
    //            break;
    //        }
    //        //普通
    //        Tabpp(p_row);//普通(一行)
    //        break;
    //    default:break;
    //    }

    //    QSqlQueryModel givemodel;

    //    givemodel.setQuery(QString("SELECT cyms_bt_dish_present.int_flowID,cyms_bt_dish_present.ch_dishno, cyms_bt_dish_present.ch_presentno,cyms_bt_dish_present.num_base,cyms_bt_dish_present.num_num FROM cyms_bt_dish_present WHERE cyms_bt_dish_present.ch_dishno in ('%1')  ").arg(no),Widget::get_db());
    //    int hanghao = model->rowCount();
    //    int zengsong_f = 0;//设置的赠送菜
    //    int zhudian = 0;
    //    for(int i = 0; i < model->rowCount(); i++)
    //    {
    //        //qDebug() << model->item(i,1)->text().trimmed() << "ls" << no << model->item(i,11)->text();
    //        if(model->item(i,1)->text().trimmed() == no && model->item(i,11)->text() == "N")
    //        {
    //            zhudian++;
    //        }
    //    }
    //    if(givemodel.rowCount() > 0)
    //    {
    //        for(int i = 0; i < givemodel.rowCount(); i++)
    //        {
    //            int yisong = 0;//以点的赠送菜
    //            zengsong_f = givemodel.record(i).value(3).toInt();
    //            QString presentno = givemodel.record(i).value(2).toString().trimmed();
    //            for(int k = 0; k < model->rowCount(); k++)
    //            {
    //                if(model->item(k,1)->text().trimmed() == presentno && model->item(k,11)->text().trimmed() == "Y")
    //                {
    //                    yisong++;
    //                }
    //            }
    //            //qDebug() << zhudian/zengsong_f  << "lllll" << yisong;
    //            if(zhudian/zengsong_f > 0 && zhudian/zengsong_f > yisong )
    //            {
    //                QString pinma_g = givemodel.record(i).value(2).toString();
    //                QList<QStandardItem*>qitem;
    //                hanghao++;
    //                QSqlQueryModel givemodel1;
    //                givemodel1.setQuery(QString("select vch_dishname, num_price from cyms_bt_dish where ch_dishno = '%1'").arg(pinma_g),Widget::get_db());
    //                QString pinming_g = givemodel1.record(0).value(0).toString();
    //                QString pinming_j = QString().sprintf("%0.2f",givemodel1.record(0).value(1).toFloat());
    //                QString hang = QString::number(hanghao);
    //                qitem.insert(0, new QStandardItem(hang));//0行号
    //                qitem.insert(1, new QStandardItem(pinma_g));//品码
    //                qitem.insert(2, new QStandardItem(pinming_g));//品名
    //                qitem.insert(3, new QStandardItem("1.00"));//数量
    //                qitem.insert(4, new QStandardItem(pinming_j));//价格
    //                qitem.insert(5, new QStandardItem("100"));//折扣
    //                qitem.insert(6, new QStandardItem(pinming_j));//金额
    //                qitem.insert(7, new QStandardItem(hang));//点菜行号
    //                qitem.insert(8, new QStandardItem(""));//厨打说明
    //                qitem.insert(9, new QStandardItem("N"));//套菜
    //                qitem.insert(10, new QStandardItem("N"));//特价
    //                qitem.insert(11, new QStandardItem("Y"));//赠送
    //                qitem.insert(12, new QStandardItem("N"));//优惠
    //                qitem.insert(13, new QStandardItem(""));//优惠
    //                qitem.insert(14, new QStandardItem(""));//优惠
    //                qitem.insert(15, new QStandardItem(""));//优惠
    //                //  qitem.insert(16, new QStandardItem(""));//优惠
    //                foreach(QStandardItem *k,qitem){
    //                    k->setTextAlignment(Qt::AlignCenter);
    //                }
    //                model->appendRow(qitem);
    //            }
    //        }
    //        //tabview->selectRow(model->rowCount());
    //    }
}

//void Cookie_label::mousePressEvent(QMouseEvent *)
//{
//    /*控件只能点一次*/

//    // lable_bottom->setPixmap(QPixmap(p2));

//}

//void Cookie_label::Tabpp(int f)
//{
//    QStandardItemModel *& model = Main_left::modelBase;
//    System_Tabview *&tabview = Main_left::tabview;

//    //数量+
//    if(f < model->rowCount()){//找到
//        if(f >= 0){//数量+1
//            if(suit_flag == "Y"){
//                int f1 = f;
//                f1++;
//                while(model->index(f1, 0).data(Qt::TextColorRole) == Qt::green){
//                    model->item(f1, 3)->setText(QString().sprintf("%.2f",model->item(f1, 3)->text().toFloat() + 1));
//                    f1++;
//                    if(f1 >= model->rowCount())
//                        break;
//                }
//            }
//            model->item(f, 3)->setText(QString().sprintf("%.2f",model->item(f, 3)->text().toFloat() + 1));
//            model->item(f,6)->setText(QString().sprintf("%.2f",model->item(f, 3)->text().toFloat()  * model->item(f, 4)->text().toFloat() / 100 * model->item(f, 5)->text().toFloat()));
//            float count_money = 0.00;
//            for(int k = 0; k < model->rowCount() ; k++){
//                count_money +=  model->item(k, 6)->text().toFloat();
//            }
//            Main_left::table_edit->setText(QString("小计:               ")+QString().sprintf("%0.2f",count_money));
//            tabview->selectRow(f);

//            if(Main_Widget::double_srceen_view == true)
//            {
//                QTableView *&tabview_t = Double_Display_View::table_view;
//                tabview_t->selectRow(f);
//                getImages();
//                Main_Widget::cc->xiaoji->setText(QString("小计:  ")+QString().sprintf("%0.2f",count_money));
//            }
//            tabview->selectRow(f);
//        }
//        return;
//    }

//    //追加+
//    singleApp(f);
//    if(suit_flag == "Y"){
//        if(main_suitno.isEmpty())return;
//        QSqlQueryModel m;
//        m.setQuery(QString("Select int_flowID , num_num *1.00 from cyms_bt_dish_suit where ch_suitno ='%1'").arg(main_suitno),Widget::get_db());
//        for(int i = 0; i < m.rowCount(); i++){
//            QString id = m.record(i).value("int_flowID").toString();
//            singleApp(f,id);
//        }
//    }

//    //客显
//    float aa;
//    aa = cur_price.toFloat();

//    //    QSettings *configIniRead = new QSettings("usr/local/qt.ini", QSettings::IniFormat);
//    //    QString tab4_port1box_text = configIniRead->value("/pos/custdisp_port").toString();
//    //    if(tab4_port1box_text != "NONE"  && !tab4_port1box_text.isEmpty())
//    //    {
//    //        GScreen().writeCom(1,QString().sprintf("%0.2f",aa));
//    //    }
//    //    delete configIniRead;
//    Main_left::tabview->setColumnHidden(7,true);
//    Main_left::tabview->setColumnHidden(8,true);
//    Main_left::tabview->setColumnHidden(10,true);
//    Main_left::tabview->setColumnHidden(11,true);
//    Main_left::tabview->setColumnHidden(12,true);
//    Main_left::tabview->setColumnHidden(13,true);
//    Main_left::tabview->setColumnHidden(14,true);
//    Main_left::tabview->setColumnHidden(15,true);
//    Main_left::tabview->setColumnHidden(16,true);

//}


////f 选中 主套菜
////choice =-1 不是子套菜
////！=-1 子套菜
//void Cookie_label::singleApp(int f,QString choice)
//{
//    QStandardItemModel *& model = Main_left::modelBase;
//    System_Tabview *&tabview = Main_left::tabview;


//    QList<QStandardItem*>qitem;//存入tabview的行列数
//    n_func *gfunc = new n_func;
//    QString index_str;
//    QString ls_dishno;
//    float ldec_price_spec;
//    QString ls_special;
//    float ldec_price;
//    QModelIndex index;
//    //没找到
//    class QSqlQueryModel *&  model_cookie = Main_right::get_model();

//    /*获取一条数据，detail后处理*/
//    //品码 品名 价格 ch_suitflag,ch_discount,ch_curprice,ch_deductflag 1-7
//    QDateTime ldt_datetime = gfunc->f_get_sysdatetime();
//    for(int x = 0; x < model_cookie->columnCount(); x++){

//        index_str = model_cookie->index(row, x, QModelIndex()).data().toString();
//        qitem.push_back(new QStandardItem(index_str));
//    }
//    ls_dishno = qitem[0]->text();//品码
//    qitem.insert(0, new QStandardItem("1"));//0行号
//    qitem.insert(3, new QStandardItem("1.00"));//3数量
//    qitem.insert(5, new QStandardItem("100"));//5折扣

//    qitem.insert(6, new QStandardItem(QString().sprintf("%.2f", qitem[4]->text().toFloat())));//7金额
//    qitem.insert(7, new QStandardItem);//9点菜行号
//    qitem.insert(8, new QStandardItem);//8厨打说明
//    // qitem.insert(9, new QStandardItem("N"));
//    // qitem.insert(10, new QStandardItem("N"));
//    qitem.insert(11, new QStandardItem("N"));
//    qitem.insert(12, new QStandardItem("N"));
//    qitem[4]->setText(QString::number(qitem[4]->text().toFloat(),'f',2));
//    //后处理
//    //实价
//    if(/*qitem[13]->text()*/cur_flag == "Y"){
//        qitem[4]->setText(QString().sprintf("%0.2f",cur_price.toFloat()));;//(cur_price
//    }
//    //套菜
//    if(qitem[9]->text() == "Y"){//ch_suitflag
//        qitem[9]->setText("P");
//        for(int i = 0;i<qitem.count();i++){
//            qitem.value(i)->setData(Qt::blue,Qt::TextColorRole);
//        }
//        main_suitno = qitem[1]->text().trimmed();
//    }
//    //子套菜
//    if(!choice.isEmpty()){
//        for(int i = 0;i<qitem.count();i++){
//            qitem.value(i)->setData(Qt::green,Qt::TextColorRole);
//        }
//        QSqlQueryModel m1;
//        m1.setQuery(QString("Select a.ch_dishno from cyms_bt_dish_suit a , cyms_bt_dish b where a.ch_dishno =b.ch_dishno and b.ch_stopflag ='N' and a.int_flowID = '%1' ").arg(choice),Widget::get_db());
//        QString no = m1.record(0).value("ch_dishno").toString();
//        m1.clear();
//        m1.setQuery(QString("select vch_dishname from cyms_bt_dish where cyms_bt_dish.ch_dishno = '%1'").arg(no),Widget::get_db());
//        QString name = m1.record(0).value("vch_dishname").toString();

//        //0行号自增不管 3数量不管  5折扣不管 7点菜行号不管 6总价不管
//        qitem[1]->setText(no);
//        qitem[2]->setText(name);

//        qitem[4]->setText("0.00");//价格
//        qitem[8]->setText("");//厨打
//        qitem[9]->setText("Y");
//        qitem[10]->setText("N");
//        qitem[11]->setText("N");
//        qitem[12]->setText("N");
//    }else

//        //特价
//    {
//        ldec_price = qitem[4]->text().toFloat();
//        int KenShuChooseCase_1 = ldt_datetime.date().dayOfWeek();
//        QSqlQueryModel m;
//        //qDebug() << KenShuChooseCase_1;
//        switch(KenShuChooseCase_1){
//        case 1:
//            m.setQuery(QString("Select b.num_price from cyms_bt_dish a , cyms_bt_dish_special b where a.ch_dishno =b.ch_dishno and b.ch_sun ='Y' and a.ch_dishno = '%1' and(convert(char(8), GetDate() , 108)between convert(char(8), b.dt_starttime , 108)and convert(char(8), b.dt_endtime , 108)) and(convert(char(10), GetDate() , 120)between convert(char(10), b.dt_begin , 120)and convert(char(10), b.dt_end , 120));").arg(ls_dishno),Widget::get_db());
//            break;
//        case 2:
//            m.setQuery(QString("Select b.num_price from cyms_bt_dish a , cyms_bt_dish_special b where a.ch_dishno =b.ch_dishno and b.ch_mon ='Y' and a.ch_dishno = '%1' and(convert(char(8), GetDate() , 108)between convert(char(8), b.dt_starttime , 108)and convert(char(8), b.dt_endtime , 108)) and(convert(char(10), GetDate() , 120)between convert(char(10), b.dt_begin , 120)and convert(char(10), b.dt_end , 120));").arg(ls_dishno),Widget::get_db());
//            break;
//        case 3:
//            m.setQuery(QString("Select b.num_price from cyms_bt_dish a , cyms_bt_dish_special b where a.ch_dishno =b.ch_dishno and b.ch_tues ='Y' and a.ch_dishno = '%1' and(convert(char(8), GetDate() , 108)between convert(char(8), b.dt_starttime , 108)and convert(char(8), b.dt_endtime , 108)) and(convert(char(10), GetDate() , 120)between convert(char(10), b.dt_begin , 120)and convert(char(10), b.dt_end , 120));").arg(ls_dishno),Widget::get_db());
//            break;
//        case 4:
//            m.setQuery(QString("Select b.num_price from cyms_bt_dish a , cyms_bt_dish_special b where a.ch_dishno =b.ch_dishno and b.ch_wed ='Y' and a.ch_dishno = '%1' and(convert(char(8), GetDate() , 108)between convert(char(8), b.dt_starttime , 108)and convert(char(8), b.dt_endtime , 108)) and(convert(char(10), GetDate() , 120)between convert(char(10), b.dt_begin , 120)and convert(char(10), b.dt_end , 120));").arg(ls_dishno),Widget::get_db());
//            break;
//        case 5:
//            m.setQuery(QString("Select b.num_price from cyms_bt_dish a , cyms_bt_dish_special b where a.ch_dishno =b.ch_dishno and b.ch_thurs ='Y' and a.ch_dishno = '%1' and(convert(char(8), GetDate() , 108)between convert(char(8), b.dt_starttime , 108)and convert(char(8), b.dt_endtime , 108)) and(convert(char(10), GetDate() , 120)between convert(char(10), b.dt_begin , 120)and convert(char(10), b.dt_end , 120));").arg(ls_dishno),Widget::get_db());
//            break;
//        case 6:
//            m.setQuery(QString("Select b.num_price from cyms_bt_dish a , cyms_bt_dish_special b where a.ch_dishno =b.ch_dishno and b.ch_friday ='Y' and a.ch_dishno = '%1' and(convert(char(8), GetDate() , 108)between convert(char(8), b.dt_starttime , 108)and convert(char(8), b.dt_endtime , 108)) and(convert(char(10), GetDate() , 120)between convert(char(10), b.dt_begin , 120)and convert(char(10), b.dt_end , 120));").arg(ls_dishno),Widget::get_db());
//            break;
//        case 7:
//            m.setQuery(QString("Select b.num_price from cyms_bt_dish a , cyms_bt_dish_special b where a.ch_dishno =b.ch_dishno and b.ch_sat ='Y' and a.ch_dishno = '%1' and(convert(char(8), GetDate() , 108)between convert(char(8), b.dt_starttime , 108)and convert(char(8), b.dt_endtime , 108)) and(convert(char(10), GetDate() , 120)between convert(char(10), b.dt_begin , 120)and convert(char(10), b.dt_end , 120));").arg(ls_dishno),Widget::get_db());
//            break;
//        default:break;
//        }
//        // INTO :ldec_price_spec
//        ldec_price_spec = m.record(0).value("num_price").toFloat();
//        // qDebug() << m.rowCount() << ldec_price_spec << ls_dishno;
//        if(m.rowCount() == 0 || ldec_price_spec == 0){ //53
//            ls_special = "N";
//        }else{ //53
//            ldec_price = ldec_price_spec;
//            // qDebug() << ldec_price << ldec_price_spec;
//            ls_special = "Y";
//        }
//        qitem[10]->setText(ls_special);
//        qitem[4]->setText(QString().sprintf("%.02f", ldec_price));
//    }
//    //行号，点菜行号，居中
//    qitem[0]->setText(QString::number(model->rowCount() + 1));//行号自增
//    qitem[7]->setText(qitem[0]->text());/*将行号赋给点菜行*/
//    foreach(QStandardItem *i,qitem){
//        i->setTextAlignment(Qt::AlignCenter);
//    }
//    /*update 总价*/
//    //   QSettings *configIniRead = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
//    //  QString tab4_port1box_text = configIniRead->value("/pos/custdisp_port").toString();
//    //    if(tab4_port1box_text != "NONE"  && !tab4_port1box_text.isEmpty())
//    //    {
//    //        if(qitem[9]->text() == "N")
//    //        {
//    //            GScreen().writeCom(1,qitem[4]->text());
//    //            qDebug() << "ccccasfd" << qitem[4]->text() << qitem[9]->text();
//    //        }
//    //    }

//    QSettings *configIniRead = new QSettings(public_sql::settings_ini_dir_file, QSettings::IniFormat);
//    QString tab4_port1box_text = configIniRead->value("/pos/custdisp_port").toString();

//    if(tab4_port1box_text != "NONE"  && !tab4_port1box_text.isEmpty())
//    {
//        if(qitem[9]->text() != "Y")
//        {
//            GScreen().writeCom(1,QString().sprintf("%0.2f",qitem[4]->text().toFloat()));
//        }

//    }

//    delete configIniRead;


//    qitem[6]->setText(QString().sprintf("%.2f",qitem[3]->text().toFloat()  * qitem[4]->text().toFloat() / 100 * qitem[5]->text().toFloat()));
//    model->appendRow(qitem);

//    float count_money = 0.00;
//    for(int k = 0; k < model->rowCount() ; k++){
//        if(model->item(k,11)->text() == "N")
//        {
//            count_money +=  model->item(k, 6)->text().toFloat();
//        }
//    }
//    Main_left::table_edit->setText(QString("小计:               ")+QString().sprintf("%0.2f",count_money));
//    tabview->selectRow(f);

//    if(Main_Widget::double_srceen_view == true)
//    {
//        QTableView *&tabview_t = Double_Display_View::table_view;
//        tabview_t->selectRow(f);
//        getImages();
//        Main_Widget::cc->xiaoji->setText(QString("小计:  ")+QString().sprintf("%0.2f",count_money));
//    }
//    //delete configIniRead;

//}

//QString Cookie_label::show_Cruprice_VIew()
//{
//    class QSqlQueryModel *&  model_cookie = Main_right::get_model();
//    Cruprice_VIew aa;
//    aa.modify_edit->setText(QString().sprintf("%0.2f",model_cookie->index(row, 2).data().toString().toFloat()));
//    aa.cur_edit->setText(QString().sprintf("%0.2f",model_cookie->index(row, 2).data().toString().toFloat()));
//    aa.exec();
//    if(aa.yes_no){
//        if(aa.modify_edit->text().toFloat() <= 0){
//            int ret = 
//            switch(ret){
//            case QMessageBox::Ok:
//                msgBox.close();
//                break;
//            default:break;
//            }
//            return "";
//        }
//        else{
//            // qDebug() << "cccccc"<< aa.modify_edit->text();
//            return aa.modify_edit->text();

//        }
//    }else{
//        return "";
//    }
//}

//int Cookie_label::findItem(const QString &no)
//{
//    QStandardItemModel *& model = Main_left::modelBase;
//    for(int i = 0; i < model->rowCount(); i++){
//        QVariant color = model->index(i,1).data(Qt::TextColorRole);
//        QVariant data = model->index(i,1).data(Qt::DisplayRole);
//        if(color == Qt::green)continue;
//        if(data.toString().trimmed() == no.trimmed())
//            return 1;
//    }
//    return 0;
//}

//QString Cookie_label::get_special_price(const QString &no)
//{
//    n_func *gfunc = new n_func;
//    QDateTime ldt_datetime = gfunc->f_get_sysdatetime();
//    int KenShuChooseCase_1 = ldt_datetime.date().day();
//    QSqlQueryModel m;
//    switch(KenShuChooseCase_1){
//    case 1:
//        m.setQuery(QString("Select b.num_price from cyms_bt_dish a , cyms_bt_dish_special b where a.ch_dishno =b.ch_dishno and b.ch_sun ='Y' and a.ch_dishno = '%1' and(convert(char(8), GetDate() , 108)between convert(char(8), b.dt_starttime , 108)and convert(char(8), b.dt_endtime , 108)) and(convert(char(10), GetDate() , 120)between convert(char(10), b.dt_begin , 120)and convert(char(10), b.dt_end , 120));").arg(no));
//        break;
//    case 2:
//        m.setQuery(QString("Select b.num_price from cyms_bt_dish a , cyms_bt_dish_special b where a.ch_dishno =b.ch_dishno and b.ch_mon ='Y' and a.ch_dishno = '%1' and(convert(char(8), GetDate() , 108)between convert(char(8), b.dt_starttime , 108)and convert(char(8), b.dt_endtime , 108)) and(convert(char(10), GetDate() , 120)between convert(char(10), b.dt_begin , 120)and convert(char(10), b.dt_end , 120));").arg(no));
//        break;
//    case 3:
//        m.setQuery(QString("Select b.num_price from cyms_bt_dish a , cyms_bt_dish_special b where a.ch_dishno =b.ch_dishno and b.ch_tues ='Y' and a.ch_dishno = '%1' and(convert(char(8), GetDate() , 108)between convert(char(8), b.dt_starttime , 108)and convert(char(8), b.dt_endtime , 108)) and(convert(char(10), GetDate() , 120)between convert(char(10), b.dt_begin , 120)and convert(char(10), b.dt_end , 120));").arg(no));
//        break;
//    case 4:
//        m.setQuery(QString("Select b.num_price from cyms_bt_dish a , cyms_bt_dish_special b where a.ch_dishno =b.ch_dishno and b.ch_wed ='Y' and a.ch_dishno = '%1' and(convert(char(8), GetDate() , 108)between convert(char(8), b.dt_starttime , 108)and convert(char(8), b.dt_endtime , 108)) and(convert(char(10), GetDate() , 120)between convert(char(10), b.dt_begin , 120)and convert(char(10), b.dt_end , 120));").arg(no));
//        break;
//    case 5:
//        m.setQuery(QString("Select b.num_price from cyms_bt_dish a , cyms_bt_dish_special b where a.ch_dishno =b.ch_dishno and b.ch_thurs ='Y' and a.ch_dishno = '%1' and(convert(char(8), GetDate() , 108)between convert(char(8), b.dt_starttime , 108)and convert(char(8), b.dt_endtime , 108)) and(convert(char(10), GetDate() , 120)between convert(char(10), b.dt_begin , 120)and convert(char(10), b.dt_end , 120));").arg(no));
//        break;
//    case 6:
//        m.setQuery(QString("Select b.num_price from cyms_bt_dish a , cyms_bt_dish_special b where a.ch_dishno =b.ch_dishno and b.ch_friday ='Y' and a.ch_dishno = '%1' and(convert(char(8), GetDate() , 108)between convert(char(8), b.dt_starttime , 108)and convert(char(8), b.dt_endtime , 108)) and(convert(char(10), GetDate() , 120)between convert(char(10), b.dt_begin , 120)and convert(char(10), b.dt_end , 120));").arg(no));
//        break;
//    case 7:
//        m.setQuery(QString("Select b.num_price from cyms_bt_dish a , cyms_bt_dish_special b where a.ch_dishno =b.ch_dishno and b.ch_sat ='Y' and a.ch_dishno = '%1' and(convert(char(8), GetDate() , 108)between convert(char(8), b.dt_starttime , 108)and convert(char(8), b.dt_endtime , 108)) and(convert(char(10), GetDate() , 120)between convert(char(10), b.dt_begin , 120)and convert(char(10), b.dt_end , 120));").arg(no));
//        break;
//    default:break;
//    }

//    return m.record(0).value("num_price").toString();
//}

//QStringList Cookie_label::getImages()

//{
//    // QWidget * imagest =  Double_Display_View::images;
//    QStringList imageList;

//    QDirIterator it(Double_Display_View::pic_p, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
//    while(it.hasNext())

//    {

//        QString imageFile = it.next();

//        QStringList list = imageFile.split('.');



//        QString last = list.takeLast(); // 文件后缀名

//        if ("jpg"  == last || "JPG" == last || "png" == last || "PNG" == last || "bmp" == last || "BMP" == last)

//            imageList << imageFile;

//    }

//    QString image_name;
//    QString load_name;
//    load_name.clear();
//    for(int i = 0; i < imageList.count(); i++)
//    {
//        //qDebug() << "+++++++" << imageList[i];
//        image_name = imageList[i];
//        // qDebug() << image_name << Main_left::modelBase->item(Double_Display_View::table_view->currentIndex().row(),1)->text().trimmed();
//        if(image_name.contains(Main_left::modelBase->item(Double_Display_View::table_view->currentIndex().row(),1)->text().trimmed()))
//        {
//            load_name =image_name;
//        }

//    }


//    Main_Widget::cc->loadpix(load_name);

//    if( imageList.count() == 0)

//    {

//        return QStringList();

//    }

//    return imageList;

//}

//void Cookie_label::setallinfo()
//{
//    QString ch_boothid = Widget::dangkou->text(); /*"01";*///档口号不能为空
//    QString select("SELECT cyms_bt_dish_type.ch_dish_typeno,cyms_bt_dish_type.vch_dish_typename,cyms_bt_dish_type.ch_boothid  FROM cyms_bt_dish_type  WHERE ch_dish_typeno is not null   and ch_boothid = '"
//                   + ch_boothid + "'");
//    // Widget:: model_type = new QSqlQueryModel;
//    Widget::model_type->setQuery(select,Widget::get_db()) ;//type的信息
//    /*COOKIE*/
//    select.clear();
//    QString selectB = "SELECT cyms_bt_dish.ch_dishno,cyms_bt_dish.vch_dishname,cyms_bt_dish.num_price,ch_suitflag,ch_discount,ch_stopflag,ch_deductflag,ch_curprice  FROM cyms_bt_dish  WHERE ch_dishno is not null   and ch_dish_typeno = '";
//    QString ch_dish_typeno;
//    QModelIndex index;
//    for(int i = 0; i < Widget::model_type->rowCount(); i++){
//        //qDebug()<<QString::number(model_type->rowCount())<<"111111";

//        index = Widget::model_type->index(i, 0, QModelIndex());

//        ch_dish_typeno = Widget::model_type->data(index).toString();
//        select.clear();
//        select = selectB + ch_dish_typeno +"'";

//        Widget::qmodel_dish.push_back(new QSqlQueryModel);
//        Widget::qmodel_dish[i]->setQuery(select,Widget::get_db());

//    }
//}
