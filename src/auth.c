#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mariadb/mysql.h>
#include "auth.h"

static void leggi_stringa(const char *messaggio, char *buffer, int dimensione) {
    printf("%s", messaggio);
    fgets(buffer, dimensione, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

int registra_studente(MYSQL *conn) {
    char nome[100];
    char email[255];
    char password[255];
    char query[1000];

    printf("\n--- REGISTRAZIONE STUDENTE ---\n");

    leggi_stringa("Nome: ", nome, sizeof(nome));
    leggi_stringa("Email: ", email, sizeof(email));
    leggi_stringa("Password: ", password, sizeof(password));

    snprintf(query, sizeof(query),
        "INSERT INTO utenti (nome, email, password, ruolo) "
        "VALUES ('%s', '%s', '%s', 'studente')",
        nome, email, password
    );

    if (mysql_query(conn, query)) {
        printf("Errore registrazione: %s\n", mysql_error(conn));
        return 0;
    }

    printf("Registrazione completata con successo.\n");
    return 1;
}

int login_utente(MYSQL *conn, UtenteLoggato *utente) {
    char email[255];
    char password[255];
    char query[1000];

    printf("\n--- LOGIN ---\n");

    leggi_stringa("Email: ", email, sizeof(email));
    leggi_stringa("Password: ", password, sizeof(password));

    snprintf(query, sizeof(query),
        "SELECT id, nome, ruolo FROM utenti "
        "WHERE email='%s' AND password='%s'",
        email, password
    );

    if (mysql_query(conn, query)) {
        printf("Errore login: %s\n", mysql_error(conn));
        return 0;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);

    if (row == NULL) {
        printf("Credenziali non valide.\n");
        mysql_free_result(result);
        return 0;
    }

    utente->id = atoi(row[0]);
    strcpy(utente->nome, row[1]);
    strcpy(utente->ruolo, row[2]);

    printf("\nLogin riuscito.\n");
    printf("Benvenuto %s! Ruolo: %s\n", row[1], row[2]);

    mysql_free_result(result);
    return 1;
}