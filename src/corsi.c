#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mariadb/mysql.h>
#include "corsi.h"

static void leggi_stringa(const char *messaggio, char *buffer, int dimensione) {
    printf("%s", messaggio);

    if (fgets(buffer, dimensione, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

void visualizza_corsi(MYSQL *conn) {
    const char *query =
        "SELECT c.id, c.titolo, c.categoria, c.livello, c.durata, c.num_lezioni, u.nome "
        "FROM corsi c "
        "LEFT JOIN utenti u ON c.docente_id = u.id";

    if (mysql_query(conn, query)) {
        printf("Errore visualizzazione corsi: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (result == NULL) {
        printf("Errore lettura risultati: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_ROW row;

    printf("\n--- CATALOGO CORSI ---\n");

    if (mysql_num_rows(result) == 0) {
        printf("Nessun corso disponibile.\n");
        mysql_free_result(result);
        return;
    }

    while ((row = mysql_fetch_row(result))) {
        printf("\nID: %s\n", row[0]);
        printf("Titolo: %s\n", row[1]);
        printf("Categoria: %s\n", row[2] ? row[2] : "N/D");
        printf("Livello: %s\n", row[3]);
        printf("Durata: %s ore\n", row[4]);
        printf("Lezioni: %s\n", row[5]);
        printf("Docente: %s\n", row[6] ? row[6] : "N/D");
    }

    mysql_free_result(result);
}

void crea_corso(MYSQL *conn, int docente_id) {
    char titolo[255];
    char descrizione[500];
    char categoria[100];
    char livello[50];
    int durata;
    int num_lezioni;
    char query[2000];

    printf("\n--- CREAZIONE CORSO ---\n");

    leggi_stringa("Titolo: ", titolo, sizeof(titolo));
    leggi_stringa("Descrizione: ", descrizione, sizeof(descrizione));
    leggi_stringa("Categoria: ", categoria, sizeof(categoria));
    leggi_stringa("Livello (base/intermedio/avanzato): ", livello, sizeof(livello));

    printf("Durata ore: ");
    scanf("%d", &durata);
    getchar();

    printf("Numero lezioni: ");
    scanf("%d", &num_lezioni);
    getchar();

    snprintf(query, sizeof(query),
        "INSERT INTO corsi "
        "(titolo, descrizione, categoria, livello, durata, num_lezioni, docente_id) "
        "VALUES ('%s', '%s', '%s', '%s', %d, %d, %d)",
        titolo, descrizione, categoria, livello, durata, num_lezioni, docente_id
    );

    if (mysql_query(conn, query)) {
        printf("Errore creazione corso: %s\n", mysql_error(conn));
        return;
    }

    printf("Corso creato con successo.\n");
}

void iscrivi_corso(MYSQL *conn, int studente_id) {
    int corso_id;
    char query[500];

    visualizza_corsi(conn);

    printf("\n--- ISCRIZIONE CORSO ---\n");
    printf("ID corso a cui iscriversi: ");
    scanf("%d", &corso_id);
    getchar();

    snprintf(query, sizeof(query),
        "SELECT id FROM corsi WHERE id = %d",
        corso_id
    );

    if (mysql_query(conn, query)) {
        printf("Errore controllo corso.\n");
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (result == NULL || mysql_num_rows(result) == 0) {
        printf("Errore: il corso selezionato non esiste.\n");

        if (result != NULL) {
            mysql_free_result(result);
        }

        return;
    }

    mysql_free_result(result);

    snprintf(query, sizeof(query),
        "INSERT INTO iscrizioni (studente_id, corso_id) "
        "VALUES (%d, %d)",
        studente_id, corso_id
    );

    if (mysql_query(conn, query)) {
        printf("Errore: sei gia' iscritto a questo corso oppure l'iscrizione non e' valida.\n");
        return;
    }

    printf("Iscrizione avvenuta con successo.\n");
}

void visualizza_corsi_iscritti(MYSQL *conn, int studente_id) {
    char query[2000];

    snprintf(query, sizeof(query),
        "SELECT c.id, c.titolo, c.categoria, c.livello, c.durata, c.num_lezioni, u.nome "
        "FROM corsi c "
        "LEFT JOIN utenti u ON c.docente_id = u.id "
        "INNER JOIN iscrizioni i ON c.id = i.corso_id "
        "WHERE i.studente_id = %d",
        studente_id
    );

    if (mysql_query(conn, query)) {
        printf("Errore visualizzazione corsi iscritti: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (result == NULL) {
        printf("Errore lettura risultati: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_ROW row;

    printf("\n--- CORSI ISCRITTO ---\n");

    if (mysql_num_rows(result) == 0) {
        printf("Non sei ancora iscritto a nessun corso.\n");
        mysql_free_result(result);
        return;
    }

    while ((row = mysql_fetch_row(result))) {
        printf("\nID: %s\n", row[0]);
        printf("Titolo: %s\n", row[1]);
        printf("Categoria: %s\n", row[2] ? row[2] : "N/D");
        printf("Livello: %s\n", row[3]);
        printf("Durata: %s ore\n", row[4]);
        printf("Lezioni: %s\n", row[5]);
        printf("Docente: %s\n", row[6] ? row[6] : "N/D");
    }

    mysql_free_result(result);
}

void cerca_corsi_categoria(MYSQL *conn) {
    char categoria[100];
    char query[1000];

    printf("\nCategoria da cercare: ");
    fgets(categoria, sizeof(categoria), stdin);
    categoria[strcspn(categoria, "\n")] = '\0';

    snprintf(query, sizeof(query),
        "SELECT id, titolo, livello "
        "FROM corsi "
        "WHERE categoria = '%s'",
        categoria
    );

    if (mysql_query(conn, query)) {
        printf("Errore ricerca corsi: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (result == NULL) {
        printf("Errore recupero risultati.\n");
        return;
    }

    MYSQL_ROW row;

    printf("\n--- RISULTATI RICERCA ---\n");

    if (mysql_num_rows(result) == 0) {
        printf("Nessun corso trovato.\n");
    }

    while ((row = mysql_fetch_row(result))) {
        printf("\nID: %s\n", row[0]);
        printf("Titolo: %s\n", row[1]);
        printf("Livello: %s\n", row[2]);
    }

    mysql_free_result(result);
}

void cerca_corsi_livello(MYSQL *conn) {
    char livello[50];
    char query[1000];

    printf("\nLivello (base/intermedio/avanzato): ");
    fgets(livello, sizeof(livello), stdin);
    livello[strcspn(livello, "\n")] = '\0';

    snprintf(query, sizeof(query),
        "SELECT id, titolo, categoria "
        "FROM corsi "
        "WHERE livello = '%s'",
        livello
    );

    if (mysql_query(conn, query)) {
        printf("Errore ricerca corsi: %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (result == NULL) {
        printf("Errore recupero risultati.\n");
        return;
    }

    MYSQL_ROW row;

    printf("\n--- RISULTATI RICERCA ---\n");

    if (mysql_num_rows(result) == 0) {
        printf("Nessun corso trovato.\n");
    }

    while ((row = mysql_fetch_row(result))) {
        printf("\nID: %s\n", row[0]);
        printf("Titolo: %s\n", row[1]);
        printf("Categoria: %s\n", row[2]);
    }

    mysql_free_result(result);
}

void visualizza_corsi_docente(MYSQL *conn, int docente_id)
{
    char query[1000];

    snprintf(query, sizeof(query),
        "SELECT id, titolo, categoria, livello "
        "FROM corsi "
        "WHERE docente_id = %d",
        docente_id
    );

    if (mysql_query(conn, query)) {
        printf("Errore visualizzazione corsi docente: %s\n",
               mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (result == NULL) {
        printf("Errore recupero risultati.\n");
        return;
    }

    MYSQL_ROW row;

    printf("\n--- I MIEI CORSI ---\n");

    if (mysql_num_rows(result) == 0) {
        printf("Non hai ancora creato corsi.\n");
        mysql_free_result(result);
        return;
    }

    while ((row = mysql_fetch_row(result))) {
        printf("\nID: %s\n", row[0]);
        printf("Titolo: %s\n", row[1]);
        printf("Categoria: %s\n", row[2]);
        printf("Livello: %s\n", row[3]);
    }

    mysql_free_result(result);
}

void visualizza_iscritti_corso(MYSQL *conn)
{
    int corso_id;
    char query[1000];

    printf("\n--- ISCRITTI AL CORSO ---\n");

    printf("ID corso: ");
    scanf("%d", &corso_id);
    getchar();

    snprintf(query, sizeof(query),
        "SELECT u.id, u.nome, u.email "
        "FROM utenti u "
        "INNER JOIN iscrizioni i "
        "ON u.id = i.studente_id "
        "WHERE i.corso_id = %d",
        corso_id
    );

    if (mysql_query(conn, query)) {
        printf("Errore visualizzazione iscritti: %s\n",
               mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (result == NULL) {
        printf("Errore recupero risultati.\n");
        return;
    }

    MYSQL_ROW row;

    printf("\n--- STUDENTI ISCRITTI ---\n");

    if (mysql_num_rows(result) == 0) {
        printf("Nessuno studente iscritto.\n");
        mysql_free_result(result);
        return;
    }

    while ((row = mysql_fetch_row(result))) {
        printf("\nID: %s\n", row[0]);
        printf("Nome: %s\n", row[1]);
        printf("Email: %s\n", row[2]);
    }

    mysql_free_result(result);
}