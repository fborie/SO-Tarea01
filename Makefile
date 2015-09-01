CC = gcc
CFLAGS = -Wall -Werror -std=c11 -g
EXE = shellcita

all: shellcita.c
	$(CC) $(CFLAGS) shellcita.c -o $(EXE)

.PHONY: clean

clean: 
	rm -f $(EXE)
