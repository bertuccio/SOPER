/*=== Cabeceras =============================================================*/
#include "semaforos.h"
#include "sudoku.h"
#include"mensajeria.h"
/*===========================================================================*/

void (*signal (int sig, void (*func)(int))) (int);

Sudoku *dirSudoku=NULL;

/******************************************************************************

Nombre de la función: terminaOrdenador

Descripción: Captura la señal SIGUSR1 o SIGINT y libera los recursos

Devuelve: Void


******************************************************************************/
void terminaOrdenador(int sennal) {

	system("clear");
	printf("\n>Terminando\n");

	if(shmdt(dirSudoku)!=-1)
		printf(">Desvinculado de la Zona Sudoku\n\n");

	exit(0);
}

/******************************************************************************

Nombre de la función: main

Descripción: 

Devuelve: 0 si todo ha funcionado correctamente
	  -1 si no


******************************************************************************/
int main(int argc, char *argv[]){

	Sudoku sudoku;
	Ordenador ordenador;
	int semid;
	int queue;
	int tablero;
	
	signal(SIGUSR1,terminaOrdenador);
	signal(SIGINT,terminaOrdenador);

	/*Se engancha a memoria compartida*/
	tablero =shmget(CLAVE_SUDOKU,sizeof(Sudoku),SHM_R|SHM_W);
	if(tablero==-1){
		printf(">Error al obtener el id de la Zona Sudoku\n");
		exit(-1);
	}
	/*Vincula la memoria compartida Zona Tablero*/
	dirSudoku = (Sudoku *) shmat (tablero, NULL, 0);

	if(dirSudoku==(Sudoku *)-1){
		printf(">Error al vincularse a la Zona Sudoku %d \n",tablero);
		exit(-1);
	}
	printf(">Vinculado a la Zona Sudoku\n");

	/*Se engancha a los semaforos*/
	semid=creaSemaforos(CLAVE_SEMAFORO,10);
	if (semid == -1) {
		perror(">semget:");
		exit(errno);
	}


	/*Se engancha a la cola de mensajes*/
	queue = msgget(CLAVE_MENSAJES,SHM_R|SHM_W);
	if(queue==-1){
		perror("msgget:");
		exit(-1);
	}

	/*Introduce el pid del ordenador en una estructura de envio*/
	setOrdenador(&ordenador,getpid());
	/*Envia la informacion al juez*/
	sendOrdenador(queue,&ordenador);

	do{
		
		/*Recibe el turno de jugada*/
		printf("\n>Esperando turno (Ordenador)\n");
		recieveJugadaOrdenador(queue);
		system("clear");

		printf("\n>Turno obtenido\n");
		/*Accede a memoria para obtener la tabla*/
		down(semid,SEM_SUDOKU_MUTEX);
		sudoku=*dirSudoku;
		
		jugadaOrdenador(&sudoku);
		//jugadaCorrecta(&sudoku);

		/*Escribe la tabla en memoria*/
		*dirSudoku=sudoku;
		up(semid,SEM_SUDOKU_MUTEX);
		printf("\n>Jugada realizada\n");
	}while(1);


return 1;
}
