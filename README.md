CYBERXARENA LMS

Progetto in linguaggio C con database MySQL/MariaDB per la gestione base di una piattaforma corsi online.

REQUISITI
- XAMPP con MySQL attivo
- MSYS2 UCRT64
- GCC
- mingw32-make
- Librerie MariaDB client

DATABASE
Importare il file:

sql/Schema.sql

Esempio comando:

/c/xampp/mysql/bin/mysql.exe -u root -P 3307 -h 127.0.0.1 < sql/Schema.sql

Se MySQL usa la porta 3306, sostituire 3307 con 3306.

CONFIGURAZIONE DATABASE
La configurazione del database si trova in:

src/database.c

Controllare questi valori:

DB_HOST
DB_USER
DB_PASS
DB_NAME
DB_PORT

Nel progetto la porta è impostata su 3307.

COMPILAZIONE
Aprire MSYS2 UCRT64 nella cartella del progetto ed eseguire:

mingw32-make clean
mingw32-make

ESECUZIONE
Sempre da MSYS2 UCRT64:

./cyberxarena.exe

CREDENZIALI ADMIN DI TEST
Email: admin@example.com
Password: admin123

FUNZIONALITA IMPLEMENTATE
- Connessione al database
- Registrazione studente
- Login utente
- Menu studente
- Menu docente/admin
- Visualizzazione catalogo corsi
- Creazione corsi da docente/admin
- Iscrizione studente a un corso
- Visualizzazione corsi iscritti

SVILUPPI FUTURI
Le funzionalità ancora da completare sono indicate nel file TODO.txt.