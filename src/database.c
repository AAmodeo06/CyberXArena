#include <stdio.h>
#include <mariadb/mysql.h>
#include "database.h"

#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS ""
#define DB_NAME "cyberxarena"
#define DB_PORT 3307

MYSQL *db_connect(void)
{
    printf("Inizializzazione connessione al database...\n");
    fflush(stdout);

    MYSQL *conn = mysql_init(NULL);

    if (conn == NULL)
    {
        printf("Errore: inizializzazione MySQL fallita.\n");
        return NULL;
    }

    unsigned int timeout = 5;
    mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);

    printf("Connessione al database in corso...\n");
    fflush(stdout);

    if (mysql_real_connect(
            conn,
            DB_HOST,
            DB_USER,
            DB_PASS,
            DB_NAME,
            DB_PORT,
            NULL,
            0) == NULL)
    {
        printf("Errore connessione al database: %s\n",
               mysql_error(conn));
        fflush(stdout);

        mysql_close(conn);
        return NULL;
    }

    printf("Connessione al database riuscita.\n");
    fflush(stdout);

    return conn;
}

void db_close(MYSQL *conn)
{
    if (conn != NULL)
    {
        mysql_close(conn);

        printf("Connessione al database chiusa.\n");
        fflush(stdout);
    }
}