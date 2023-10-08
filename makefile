FLAGS=-O3 -lm -fopenmp
CC=gcc

nomePrograma=par


all: $(nomePrograma)

$(nomePrograma): parallel.o
	$(CC) -o $(nomePrograma) parallel.o $(FLAGS)

parallel.o: parallel.c
	$(CC) -c parallel.c $(FLAGS)

clean:
	rm -f *.o *.gch $(nomePrograma)