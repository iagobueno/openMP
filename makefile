FLAGS=-O3 -lm
CC=gcc

nomePrograma=seq


all: $(nomePrograma)

$(nomePrograma): tsp.o
	$(CC) -o $(nomePrograma) tsp.o $(FLAGS)

tsp.o: tsp.c
	$(CC) -c tsp.c $(FLAGS)

clean:
	rm -f *.o *.gch $(nomePrograma)