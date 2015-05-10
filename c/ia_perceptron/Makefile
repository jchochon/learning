CC=gcc

CFLAGS=-g -std=c99 -Wextra -Wall

main: main.o neuron.o utils.o
	$(CC) $(CFLAGS) -o main main.o neuron.o utils.o

neuron.o: neuron.c
	$(CC) $(CFLAGS) -c neuron.c

utils.o: utils.c
	$(CC) $(CFLAGS) -c utils.c

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm main.o neuron.o utils.o

