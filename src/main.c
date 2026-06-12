#include <stdio.h>
#include <string.h>
#include <mariadb/mysql.h>
#include "database.h"
#include "auth.h"
#include "corsi.h"
#include "lezioni.h"
#include "feedback.h"
#include "statistiche.h"
#include "avanzamento.h"

void menu_studente(MYSQL *conn, UtenteLoggato utente) {
    int scelta;

    do {
        printf("\n--- MENU STUDENTE ---\n");
        printf("Benvenuto %s\n", utente.nome);
        printf("1. Visualizza catalogo corsi\n");
        printf("2. Cerca corsi per categoria\n");
        printf("3. Cerca corsi per livello\n");
        printf("4. Iscriviti a un corso\n");
        printf("5. I miei corsi\n");
        printf("6. Visualizza lezioni di un corso\n");
        printf("7. Lascia feedback\n");
        printf("8. Visualizza avanzamento\n");
        printf("9.Aggiorna avanzamento\n");
        printf("0. Logout\n");
        printf("Scelta: ");

        scanf("%d", &scelta);
        getchar();

        switch (scelta) {

            case 1:
                visualizza_corsi(conn);
                break;

            case 2:
                cerca_corsi_categoria(conn);
                break;

            case 3:
                cerca_corsi_livello(conn);
                break;

            case 4:
                iscrivi_corso(conn, utente.id);
                break;

            case 5:
                visualizza_corsi_iscritti(conn, utente.id);
                break;
            case 6: {
                int corso_id;

                printf("ID corso: ");
                scanf("%d", &corso_id);
                getchar();

                visualizza_lezioni(conn, corso_id);
                break;
            }

            case 7:
                lascia_feedback(conn, utente.id);
                break;

            case 8:
                visualizza_avanzamento(conn, utente.id);
                break;

            case 9:
                aggiorna_avanzamento(conn, utente.id);
                break;

            case 0:
                printf("Logout effettuato.\n");
                break;

            default:
                printf("Scelta non valida.\n");
        }

    } while (scelta != 0);
}

void menu_docente(MYSQL *conn, UtenteLoggato utente) {
    int scelta;

    do {
        printf("\n--- MENU DOCENTE ---\n");
        printf("Benvenuto %s\n", utente.nome);
        printf("1. Visualizza catalogo corsi\n");
        printf("2. Crea nuovo corso\n");
        printf("3. Aggiungi lezione\n");
        printf("4. Modifica lezione\n");
        printf("5. Visualizza feedback corso\n");
        printf("6. I miei corsi\n");
        printf("7. Visualizza iscritti corso\n");
        printf("0. Logout\n");
        printf("Scelta: ");

        scanf("%d", &scelta);
        getchar();

        switch (scelta) {

            case 1:
                visualizza_corsi(conn);
                break;

            case 2:
                crea_corso(conn, utente.id);
                break;

            case 3:
                aggiungi_lezione(conn);
                break;

            case 4:
                modifica_lezione(conn);
                break;

            case 5:
                visualizza_feedback_corso(conn);
                break;

            case 6:
                visualizza_corsi_docente(conn, utente.id);
                break;
            
            case 7:
                visualizza_iscritti_corso(conn);
                break;

            case 0:
                printf("Logout effettuato.\n");
                break;

            default:
                printf("Scelta non valida.\n");
        }

    } while (scelta != 0);
}

void menu_admin(MYSQL *conn, UtenteLoggato utente) {
    int scelta;

    do {
        printf("\n--- MENU ADMIN ---\n");
        printf("Benvenuto %s\n", utente.nome);
        printf("1. Visualizza catalogo corsi\n");
        printf("2. Crea nuovo corso\n");
        printf("3. Aggiungi lezione\n");
        printf("4. Modifica lezione\n");
        printf("5. Visualizza feedback corso\n");
        printf("6. Mostra statistiche\n");
        printf("7. Visualizza iscritti corso\n");
        printf("0. Logout\n");
        printf("Scelta: ");

        scanf("%d", &scelta);
        getchar();

        switch (scelta) {

            case 1:
                visualizza_corsi(conn);
                break;

            case 2:
                crea_corso(conn, utente.id);
                break;

            case 3:
                aggiungi_lezione(conn);
                break;

            case 4:
                modifica_lezione(conn);
                break;

            case 5:
                visualizza_feedback_corso(conn);
                break;

            case 6:
                mostra_statistiche(conn);
                break;

            case 7:
                visualizza_iscritti_corso(conn);
                break;

            case 0:
                printf("Logout effettuato.\n");
                break;

            default:
                printf("Scelta non valida.\n");
        }

    } while (scelta != 0);
}

int main(void) {

    printf("Avvio CyberXArena...\n");
    fflush(stdout);

    MYSQL *conn = db_connect();

    if (conn == NULL) {
        printf("Impossibile avviare CyberXArena.\n");
        return 1;
    }

    int scelta;

    do {

        printf("\n=====================================\n");
        printf("          CYBERXARENA LMS\n");
        printf("=====================================\n");

        printf("1. Registrazione studente\n");
        printf("2. Login utente\n");
        printf("3. Esci\n");

        printf("Scegli un'opzione: ");

        scanf("%d", &scelta);
        getchar();

        switch (scelta) {

            case 1:
                registra_studente(conn);
                break;

            case 2: {

                UtenteLoggato utente;

                if (login_utente(conn, &utente)) {

                    if (strcmp(utente.ruolo, "studente") == 0) {
                        menu_studente(conn, utente);
                    }
                    else if (strcmp(utente.ruolo, "docente") == 0) {
                        menu_docente(conn, utente);
                    }
                    else if (strcmp(utente.ruolo, "admin") == 0) {
                        menu_admin(conn, utente);
                    }
                }

                break;
            }

            case 3:
                printf("Chiusura programma...\n");
                break;

            default:
                printf("Scelta non valida.\n");
        }

    } while (scelta != 3);

    db_close(conn);

    return 0;
}