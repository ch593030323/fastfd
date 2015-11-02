#ifndef SQLCONVERT_H
#define SQLCONVERT_H
#include <QSqlQuery>
#include <QString>
#include <QDebug>
#include <QStringList>
#include <QVariant>

//update insert 里面的sql语句的限制:
//0.  在trimmed()以后
//1.  不能等于 ',' 2.不能等于'('
//3.  不能等于 ')' 4.不能等于';'
//5.  不能左包含 ',' 6.不能左包含'and'
//7.  不能左包含 ') '
//8.  不能包含 "update" << "set" << "where" << "insert" << "values";关键字
//9.  不能包含"   "类似的字符串
static bool iscontain(const QString &in,const QStringList &list)
{
    QString t = in.trimmed();
    if(t == ",")return true;
    if(t == ")")return true;
    if(t == "(")return true;
    if(t == ";")return true;
    if(t.startsWith(","))return true;
    if(t.startsWith("and"))return true;
    if(t.startsWith(")"))return true;

    foreach(QString str, list){
        if(in.contains(str,Qt::CaseInsensitive))
            return true;
    }
    return false;
}

static bool SqlExec(const QString &in,QSqlQuery & q)
{
    /*使用的前提是：需要用bind实现的数据，必须为 '*' 的格式*/
    /*获取插入语句，与以下类似 update insert*/
    //QString s("update cyms_m_handover_detail set vch_opertype = '你好' where num_amount  = '99'");
    //QString s("INSERT INTO cyms_m_handover_detail(int_id, vch_opertype, int_card, num_amount)VALUES('38', ' 备用金', NULL, '42.00' )");

    QString s = in.trimmed();
    QStringList l;
    QStringList l2; //bind
    QStringList isnoList;
    QString result;//prepare
    /*按 ' 来分裂 l*/
    l = s.split("'");
    /*删除 l中包含 "'(空)的str*/
    if(l.isEmpty())return false;
    if(l.last().isEmpty()){
        l.removeLast();
    }
    /*设置筛选条件*/
    isnoList << "update" << "set" << "where" << "insert" << "values";
    for(int i = 0;i < l.size(); i++){
        const QString &s =  l.value(i);
        /*判断分裂后的元素是否包含(,)来筛选 arg()的值，和将对于的值转换成 ?*/
        if(!iscontain(s,isnoList)){
            l2 << s;
            *(QString *)&s = "?";
        }
    }
    /*将分裂后得l 合并，获得数据库带？的插入语言*/
    result = l.join("");
    //设置查询语句
    q.prepare(result);
    //bind
    foreach(QString str, l2){
        q.addBindValue(str);
    }
    //执行并返回exec
    return q.exec();
}
#endif // SQLCONVERT_H
