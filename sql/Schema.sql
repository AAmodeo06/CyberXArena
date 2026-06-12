CREATE DATABASE IF NOT EXISTS cyberxarena;
USE cyberxarena;

CREATE TABLE IF NOT EXISTS utenti (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nome VARCHAR(100) NOT NULL,
    email VARCHAR(255) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL,
    ruolo ENUM('studente', 'docente', 'admin') NOT NULL DEFAULT 'studente'
);

CREATE TABLE IF NOT EXISTS corsi (
    id INT AUTO_INCREMENT PRIMARY KEY,
    titolo VARCHAR(255) NOT NULL,
    descrizione TEXT,
    categoria VARCHAR(100),
    livello ENUM('base', 'intermedio', 'avanzato') NOT NULL DEFAULT 'base',
    durata INT DEFAULT 0,
    num_lezioni INT DEFAULT 0,
    docente_id INT,
    FOREIGN KEY (docente_id) REFERENCES utenti(id)
);

CREATE TABLE IF NOT EXISTS lezioni (
    id INT AUTO_INCREMENT PRIMARY KEY,
    corso_id INT NOT NULL,
    titolo VARCHAR(100) NOT NULL,
    contenuto TEXT,
    ordine_lezione INT,
    FOREIGN KEY (corso_id) REFERENCES corsi(id)
);

CREATE TABLE IF NOT EXISTS iscrizioni (
    id INT AUTO_INCREMENT PRIMARY KEY,
    studente_id INT NOT NULL,
    corso_id INT NOT NULL,
    data_iscrizione DATETIME DEFAULT CURRENT_TIMESTAMP,
    avanzamento INT DEFAULT 0,
    completato BOOLEAN DEFAULT FALSE,
    FOREIGN KEY (studente_id) REFERENCES utenti(id),
    FOREIGN KEY (corso_id) REFERENCES corsi(id),
    UNIQUE(studente_id, corso_id)
);

CREATE TABLE IF NOT EXISTS feedback (
    id INT AUTO_INCREMENT PRIMARY KEY,
    corso_id INT NOT NULL,
    studente_id INT NOT NULL,
    valutazione INT CHECK (valutazione >= 1 AND valutazione <= 5),
    commento TEXT,
    data_feedback DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (corso_id) REFERENCES corsi(id),
    FOREIGN KEY (studente_id) REFERENCES utenti(id),
    UNIQUE(corso_id, studente_id)
);

INSERT IGNORE INTO utenti (nome, email, password, ruolo)
('Admin', 'admin@example.com', 'admin123', 'admin');

INSERT IGNORE INTO corsi(titolo, descrizione, categoria, livello, durata, num_lezioni, docente_id)
('Programmazione', 'Introduzione alla programmazione in C', 'Programmazione', 'base', 20, 10, 1),
('Basi di Dati', 'Fondamenti di database relazionali', 'Database', 'intermedio', 15, 8, 1);