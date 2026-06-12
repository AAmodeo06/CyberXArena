#include <stdio.h>
#include <string.h>
#include <mariadb/mysql.h>
#include "avanzamento.h"

void visualizza_avanzamento(MYSQL *conn, int studente_id)
{
    char query[1000];

    snprintf(query, sizeof(query),
        "SELECT c.titolo, i.avanzamento, i.completato "
        "FROM iscrizioni i "
        "INNER JOIN corsi c "
        "ON i.corso_id = c.id "
        "WHERE i.studente_id = %d",
        studente_id
    );

    if (mysql_query(conn, query)) {
        printf("Errore visualizzazione avanzamento: %s\n",
               mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (result == NULL) {
        printf("Errore recupero risultati.\n");
        return;
    }

    MYSQL_ROW row;

    printf("\n--- AVANZAMENTO CORSI ---\n");

    if (mysql_num_rows(result) == 0) {
        printf("Nessun corso trovato.\n");
        mysql_free_result(result);
        return;
    }

    while ((row = mysql_fetch_row(result))) {

        printf("\nCorso: %s\n", row[0]);
        printf("Avanzamento: %s%%\n", row[1]);

        if (strcmp(row[2], "1") == 0)
            printf("Completato: SI\n");
        else
            printf("Completato: NO\n");
    }

    mysql_free_result(result);
}

void aggiorna_avanzamento(MYSQL *conn, int studente_id)
{
    int corso_id;
    int avanzamento;

    char query[1000];

    printf("\n--- AGGIORNA AVANZAMENTO ---\n");

    printf("ID corso: ");
    scanf("%d", &corso_id);
    getchar();

    printf("Percentuale avanzamento (0-100): ");
    scanf("%d", &avanzamento);
    getchar();

    if (avanzamento < 0 || avanzamento > 100) {
        printf("Valore non valido.\n");
        return;
    }

    snprintf(query, sizeof(query),
        "UPDATE iscrizioni "
        "SET avanzamento = %d, "
        "completato = %d "
        "WHERE studente_id = %d "
        "AND corso_id = %d",
        avanzamento,
        avanzamento == 100 ? 1 : 0,
        studente_id,
        corso_id
    );

    if (mysql_query(conn, query)) {
        printf("Errore aggiornamento avanzamento: %s\n",
               mysql_error(conn));
        return;
    }

    if (mysql_affected_rows(conn) == 0) {
        printf("Non risulti iscritto a questo corso.\n");
        return;
    }

    printf("Avanzamento aggiornato con successo.\n");
}