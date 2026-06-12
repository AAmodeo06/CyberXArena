#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <mariadb/mysql.h>

void lascia_feedback(MYSQL *conn, int studente_id);
void visualizza_feedback_corso(MYSQL *conn);

#endif