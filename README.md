# CyberXArena

CyberXArena è una piattaforma didattica sviluppata in linguaggio C che permette la gestione di corsi, lezioni, feedback, statistiche e avanzamento degli utenti tramite un database MariaDB.

## Funzionalità

- Gestione utenti e autenticazione
- Gestione corsi
- Gestione lezioni
- Gestione feedback
- Monitoraggio avanzamento
- Statistiche e report
- Integrazione con database MariaDB

## Struttura del progetto

```
CyberXArena
│
├── Include/          # Header files
├── src/              # Codice sorgente
├── sql/              # Schema database
├── Makefile          # Compilazione
├── README.md
└── .gitignore
```

## Requisiti

- GCC (MSYS2 UCRT64)
- MariaDB Client Library
- XAMPP / MariaDB

## Configurazione database

1. Avviare MariaDB tramite XAMPP.
2. Creare un database chiamato:

```sql
cyberxarena
```

3. Importare il file:

```text
sql/Schema.sql
```

## Parametri di connessione

Nel file `src/database.c`:

```c
#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS ""
#define DB_NAME "cyberxarena"
#define DB_PORT 3307
```

## Compilazione

Aprire il terminale MSYS2 UCRT64 nella cartella del progetto ed eseguire:

```bash
mingw32-make
```

## Avvio

```bash
./cyberxarena.exe
```