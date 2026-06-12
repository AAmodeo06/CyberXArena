CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -I/ucrt64/include/mariadb -I/ucrt64/include/mariadb/mysql
LIBS = -L/ucrt64/lib -lmariadb -lssl -lcrypto -lz -lws2_32

SRC = src/main.c src/database.c src/auth.c src/corsi.c src/lezioni.c src/feedback.c src/statistiche.c src/avanzamento.c
OUT = cyberxarena.exe

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)

clean:
	rm -f $(OUT)