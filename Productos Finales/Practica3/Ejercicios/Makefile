CC=gcc -Wall -g

exe = programa1 programa2 programa3 programa4 programa5 programa8 programa8b jugador ordenador juez
all: $(exe)


.PHONY : clean
clean :
	rm -f *.o
	rm -f *.out

programa1: programa1.o
	$(CC) -o programa1 programa1.o

programa2: programa2.o
	$(CC) -o programa2 programa2.o

programa3: programa3.o
	$(CC) -o programa3 programa3.o

programa4: programa4.o
	$(CC) -o programa4 programa4.o

programa5: programa5.o
	$(CC) -o programa5 programa5.o

programa8: programa8.o 
	$(CC) -o programa8 programa8.o 

programa8b: programa8b.o 
	$(CC) -o programa8b programa8b.o 
juez: juez.o semaforos.o mensajeria.o sudoku.o
	$(CC) -lm -o  juez juez.o semaforos.o mensajeria.o sudoku.o

jugador: jugador.o semaforos.o mensajeria.o sudoku.o
	$(CC) -o  jugador jugador.o semaforos.o mensajeria.o sudoku.o

ordenador: ordenador.o semaforos.o mensajeria.o sudoku.o
	$(CC) -o  ordenador ordenador.o semaforos.o mensajeria.o sudoku.o

programa1.o: programa1.c
	$(CC) -c programa1.c

programa2.o: programa2.c
	$(CC) -c programa2.c

programa3.o: programa3.c
	$(CC) -c programa3.c

programa4.o: programa4.c
	$(CC) -c programa4.c

programa5.o: programa5.c
	$(CC) -c programa5.c

programa8.o: programa8.c
	$(CC) -c programa8.c

programa8b.o: programa8b.c
	$(CC) -c programa8b.c

ordenador.o: ordenador.c 
	$(CC) -c ordenador.c 

jugador.o: jugador.c 
	$(CC) -c jugador.c 

juez.o: juez.c 
	$(CC) -lm -c juez.c 

mensajeria.o: mensajeria.c
	$(CC) -c mensajeria.c

sudoku.o: sudoku.c
	$(CC) -c sudoku.c

semaforos.o: semaforos.c
	$(CC) -c semaforos.c
