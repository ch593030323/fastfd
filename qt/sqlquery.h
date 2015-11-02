#ifndef SQLQUERY_H
#define SQLQUERY_H

#include <QSqlQuery>
//重写QSqlQuery对象 实现汉字的插入和更新
class SqlQuery : public QSqlQuery
{
private:
    QSqlDatabase _db;
public:
    explicit SqlQuery(QSqlDatabase db) : QSqlQuery(db),_db(db){}
    bool exec(const QString& query);
    inline QSqlDatabase database(){return _db;}
};
#endif // SQLQUERY_H
