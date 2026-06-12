#ifndef CORSI_H
#define CORSI_H

#include <mariadb/mysql.h>

void visualizza_corsi(MYSQL *conn);
void crea_corso(MYSQL *conn, int docente_id);
void iscrivi_corso(MYSQL *conn, int studente_id);
void visualizza_corsi_iscritti(MYSQL *conn, int studente_id);
void cerca_corsi_categoria(MYSQL *conn);
void cerca_corsi_livello(MYSQL *conn);
void visualizza_corsi_docente(MYSQL *conn, int docente_id);
void visualizza_iscritti_corso(MYSQL *conn);

#endif