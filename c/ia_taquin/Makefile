CC=gcc

CFLAGS=-g -std=c99 -Wextra -Wall

main: main.o state.o utils.o
	$(CC) $(CFLAGS) -o main main.o state.o utils.o

state.o: state.c
	$(CC) $(CFLAGS) -c state.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

utils.o: utils.c
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm main.o state.o

