#ifndef LEZIONI_H
#define LEZIONI_H

#include <mariadb/mysql.h>

void aggiungi_lezione(MYSQL* conn);
void visualizza_lezioni(MYSQL* conn, int corso_id);
void modifica_lezione(MYSQL* conn);

#endif