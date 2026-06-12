#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mariadb/mysql.h>
#include "lezioni.h"

static void leggi_stringa(const char *messaggio, char *buffer, int dimensione) {
    printf("%s", messaggio);
    fgets(buffer, dimensione, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

void aggiungi_lezione(MYSQL *conn) {

    int corso_id;
    int ordine_lezione;

    char titolo[255];
    char contenuto[1000];
    char query[3000];
    printf("ID corso: ");
    scanf("%d", &corso_id);
    getchar();

    leggi_stringa("Titolo lezione: ", titolo, sizeof(titolo));
    leggi_stringa("Contenuto lezione: ", contenuto, sizeof(contenuto));

    printf("Ordine lezione: ");
    scanf("%d", &ordine_lezione);
    getchar();

    snprintf(query, sizeof(query),
        "INSERT INTO lezioni (corso_id, titolo, contenuto, ordine_lezione) "
        "VALUES (%d, '%s', '%s', %d)",
        corso_id, titolo, contenuto, ordine_lezione
    );

    if (mysql_query(conn, query)) {
        printf("Errore aggiunta lezione: %s\n", mysql_error(conn));
        return;
    }

    printf("Lezione aggiunta con successo.\n");
}

void visualizza_lezioni(MYSQL *conn, int corso_id) {
    char query[1000];

    snprintf(query, sizeof(query),
        "SELECT id, titolo, contenuto, ordine_lezione "
        "FROM lezioni "
        "WHERE corso_id = %d "
        "ORDER BY ordine_lezione",
        corso_id
    );

    if (mysql_query(conn, query)) {
        printf("Errore visualizzazione lezioni: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (result == NULL) {
        printf("Errore recupero lezioni.\n");
        return;
    }

    if (mysql_num_rows(result) == 0) {
        printf("Non ci sono lezioni per questo corso.\n");
        mysql_free_result(result);
        return;
    }

    MYSQL_ROW row;

    printf("\n--- LEZIONI CORSO ID %d ---\n", corso_id);

    while ((row = mysql_fetch_row(result))) {
        printf("\nLezione: %s\n", row[3]);
        printf("Titolo: %s\n", row[1]);
        printf("Contenuto: %s\n", row[2]);
    }

    mysql_free_result(result);
}

void modifica_lezione(MYSQL *conn) {

    int id_lezione;

    char titolo[255];
    char contenuto[1000];
    char query[3000];

    printf("\n--- MODIFICA LEZIONE ---\n");

    printf("ID lezione da modificare: ");
    scanf("%d", &id_lezione);
    getchar();

    leggi_stringa("Nuovo titolo: ", titolo, sizeof(titolo));
    leggi_stringa("Nuovo contenuto: ", contenuto, sizeof(contenuto));

    snprintf(query, sizeof(query),
        "UPDATE lezioni "
        "SET titolo='%s', contenuto='%s' "
        "WHERE id=%d",
        titolo,
        contenuto,
        id_lezione
    );

    if (mysql_query(conn, query)) {
        printf("Errore modifica lezione: %s\n",
               mysql_error(conn));
        return;
    }

    if (mysql_affected_rows(conn) == 0) {
        printf("Lezione non trovata.\n");
        return;
    }

    printf("Lezione modificata con successo.\n");
}