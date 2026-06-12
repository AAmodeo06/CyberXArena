#ifndef DATABASE_H
#define DATABASE_H

#include <mariadb/mysql.h>

MYSQL *db_connect(void);
void db_close(MYSQL *conn);

#endif