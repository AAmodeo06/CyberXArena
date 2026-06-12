#ifndef AVANZAMENTO_H
#define AVANZAMENTO_H

#include <mariadb/mysql.h>

void visualizza_avanzamento(MYSQL *conn, int studente_id);
void aggiorna_avanzamento(MYSQL *conn, int studente_id);

#endif