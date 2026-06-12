#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mariadb/mysql.h>
#include "feedback.h"

static void leggi_stringa(const char *messaggio, char *buffer, int dimensione)
{
    printf("%s", messaggio);

    if (fgets(buffer, dimensione, stdin) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

void lascia_feedback(MYSQL *conn, int studente_id)
{
    int corso_id;
    int valutazione;
    char commento[500];
    char query[2000];

    printf("\n--- FEEDBACK CORSO ---\n");

    printf("ID corso: ");
    scanf("%d", &corso_id);
    getchar();

    printf("Valutazione (1-5): ");
    scanf("%d", &valutazione);
    getchar();

    if (valutazione < 1 || valutazione > 5)
    {
        printf("Valutazione non valida.\n");
        return;
    }

    leggi_stringa("Commento: ", commento, sizeof(commento));

    snprintf(query, sizeof(query),
        "INSERT INTO feedback "
        "(corso_id, studente_id, valutazione, commento) "
        "VALUES (%d, %d, %d, '%s')",
        corso_id,
        studente_id,
        valutazione,
        commento
    );

    if (mysql_query(conn, query))
    {
        printf("Errore inserimento feedback: %s\n",
               mysql_error(conn));
        return;
    }

    printf("Feedback salvato con successo.\n");
}

void visualizza_feedback_corso(MYSQL *conn)
{
    int corso_id;
    char query[2000];

    printf("\nID corso: ");
    scanf("%d", &corso_id);
    getchar();

    snprintf(query, sizeof(query),
        "SELECT u.nome, f.valutazione, f.commento "
        "FROM feedback f "
        "INNER JOIN utenti u ON f.studente_id = u.id "
        "WHERE f.corso_id = %d",
        corso_id
    );

    if (mysql_query(conn, query))
    {
        printf("Errore visualizzazione feedback: %s\n",
               mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (result == NULL)
    {
        printf("Errore lettura risultati.\n");
        return;
    }

    MYSQL_ROW row;

    printf("\n--- FEEDBACK ---\n");

    if (mysql_num_rows(result) == 0)
    {
        printf("Nessun feedback disponibile.\n");
        mysql_free_result(result);
        return;
    }

    while ((row = mysql_fetch_row(result)))
    {
        printf("\nStudente: %s\n", row[0]);
        printf("Valutazione: %s/5\n", row[1]);
        printf("Commento: %s\n", row[2]);
    }

    mysql_free_result(result);
}