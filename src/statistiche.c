#include <stdio.h>
#include <mariadb/mysql.h>
#include "statistiche.h"

void mostra_statistiche(MYSQL *conn)
{
    MYSQL_RES *result;
    MYSQL_ROW row;

    printf("\n=============================\n");
    printf("      STATISTICHE LMS\n");
    printf("=============================\n");

    /* Numero utenti */

    if (mysql_query(conn,
        "SELECT COUNT(*) FROM utenti") == 0)
    {
        result = mysql_store_result(conn);

        if (result != NULL)
        {
            row = mysql_fetch_row(result);

            printf("Numero utenti: %s\n", row[0]);

            mysql_free_result(result);
        }
    }

    /* Numero corsi */

    if (mysql_query(conn,
        "SELECT COUNT(*) FROM corsi") == 0)
    {
        result = mysql_store_result(conn);

        if (result != NULL)
        {
            row = mysql_fetch_row(result);

            printf("Numero corsi: %s\n", row[0]);

            mysql_free_result(result);
        }
    }

    /* Numero iscrizioni */

    if (mysql_query(conn,
        "SELECT COUNT(*) FROM iscrizioni") == 0)
    {
        result = mysql_store_result(conn);

        if (result != NULL)
        {
            row = mysql_fetch_row(result);

            printf("Numero iscrizioni: %s\n", row[0]);

            mysql_free_result(result);
        }
    }

    /* Corso più frequentato */

    if (mysql_query(conn,
        "SELECT c.titolo, COUNT(i.id) AS totale "
        "FROM corsi c "
        "LEFT JOIN iscrizioni i ON c.id = i.corso_id "
        "GROUP BY c.id "
        "ORDER BY totale DESC "
        "LIMIT 1") == 0)
    {
        result = mysql_store_result(conn);

        if (result != NULL)
        {
            row = mysql_fetch_row(result);

            if (row != NULL)
            {
                printf("\nCorso piu' frequentato:\n");
                printf("%s (%s iscritti)\n",
                       row[0],
                       row[1]);
            }

            mysql_free_result(result);
        }
    }

    /* Numero corsi completati */

    if (mysql_query(conn,
        "SELECT COUNT(*) "
        "FROM iscrizioni "
        "WHERE completato = TRUE") == 0)
    {
        result = mysql_store_result(conn);

        if (result != NULL)
        {
            row = mysql_fetch_row(result);

            printf("Corsi completati: %s\n", row[0]);

            mysql_free_result(result);
        }
    }

    /* Tasso completamento */

    if (mysql_query(conn,
        "SELECT ROUND("
        "100.0 * SUM(completato) / NULLIF(COUNT(*), 0),"
        "2)"
        "FROM iscrizioni") == 0)
    {
        result = mysql_store_result(conn);

        if (result != NULL)
        {
            row = mysql_fetch_row(result);

            printf("Tasso completamento: %s%%\n",
               row[0] ? row[0] : "0");

            mysql_free_result(result);
        }
    }

    printf("=============================\n");
}