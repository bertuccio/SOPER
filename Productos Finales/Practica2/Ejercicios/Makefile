CC=gcc -Wall -g

exe = programa1 programa2 programa3 programa4 programa5 programa6 programa7 programa10 programa11 cliente monitor codificador
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

programa6: programa6.o
	$(CC) -o programa6 programa6.o

programa7: programa7.o semaforos.o
	$(CC) -o programa7 programa7.o semaforos.o

programa10: programa10.o semaforos.o
	$(CC) -o programa10 programa10.o semaforos.o

programa11: programa11.o semaforos.o
	$(CC) -o programa11 programa11.o semaforos.o

cliente: cliente.o semaforos.o peticion.o
	$(CC) -o cliente cliente.o semaforos.o peticion.o

monitor: monitor.o semaforos.o peticion.o
	$(CC) -o monitor monitor.o semaforos.o peticion.o

codificador: codificador.o semaforos.o peticion.o utilidades.o
	$(CC) -o codificador codificador.o semaforos.o peticion.o utilidades.o

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

programa6.o: programa6.c
	$(CC) -c programa6.c

programa7.o: programa7.c
	$(CC) -c programa7.c

programa10.o: programa10.c
	$(CC) -c programa10.c

programa11.o: programa11.c
	$(CC) -c programa11.c

cliente.o: cliente.c
	$(CC) -c cliente.c

codificador.o: codificador.c
	$(CC) -c codificador.c

monitor.o: monitor.c
	$(CC) -c monitor.c

utilidades.o: utilidades.c
	$(CC) -c utilidades.c

peticion.o: peticion.c
	$(CC) -c peticion.c

semaforos.o: semaforos.c
	$(CC) -c semaforos.c
