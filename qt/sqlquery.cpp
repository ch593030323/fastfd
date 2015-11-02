#include "sqlquery.h"
#include "../public_object/sqlconvert.h"

bool SqlQuery::exec(const QString &query)
{
    return SqlExec(query, *this);
}
