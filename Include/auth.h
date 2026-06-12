#ifndef AUTH_H
#define AUTH_H

#include <mariadb/mysql.h>

typedef struct {
    int id;
    char nome[100];
    char ruolo[20];
} UtenteLoggato;

int registra_studente(MYSQL *conn);
int login_utente(MYSQL *conn, UtenteLoggato *utente);

#endif