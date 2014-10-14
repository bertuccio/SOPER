/*=== Cabeceras =============================================================*/
#include "semaforos.h"
#include "sudoku.h"
#include"mensajeria.h"
/*===========================================================================*/

#define NUM_ARG 2

void (*signal (int sig, void (*func)(int))) (int);

int semid;
int queue;
Sudoku *dirSudoku=NULL;
Jugador jugador;
boolean turnoFinalizado;

/******************************************************************************

Nombre de la función: ganador

Descripción: Captura la señal SIGUSR2 y finaliza la partida con un mensaje

Devuelve: Void


******************************************************************************/
void ganador(int sennal) {

	/*mensaje de fin de partida*/
	system("clear");
	printf(">Felicidades, has ganado la partida\n");
	printf("\n\n>Juego terminado\n");

	/*libera recursos*/
	if(shmdt(dirSudoku)!=-1)
		printf(">Desvinculado de la Zona Sudoku\n\n");

	up(semid,SEM_MAX_JUGADORES);
	exit(0);
}
/******************************************************************************

Nombre de la función: ganador

Descripción: Captura la señal SIGBUS y finaliza la partida con un mensaje

Devuelve: Void


******************************************************************************/
void perdedor(int sennal) {

	/*mensaje de fin de partida*/
	system("clear");
	printf(">Lo siento, has perdido la partida\n");
	printf("\n\n>Juego terminado\n");

	/*libera recursos*/
	if(shmdt(dirSudoku)!=-1)
		printf(">Desvinculado de la Zona Sudoku\n\n");

	up(semid,SEM_MAX_JUGADORES);
	exit(0);
}


/******************************************************************************

Nombre de la función: terminaJugador

Descripción: Captura la señal SIGUSR1 o SIGINT y libera los recursos

Devuelve: Void


******************************************************************************/
void terminaJugador(int sennal) {

	system("clear");
	printf("\n>Terminando\n");

	if(shmdt(dirSudoku)!=-1)
		printf(">Desvinculado de la Zona Sudoku\n\n");

	up(semid,SEM_MAX_JUGADORES);
	exit(0);
}

/******************************************************************************

Nombre de la función: terminaTurno

Descripción: Captura la señal SIGALARM y envia una notificacion de fin de turno

Devuelve: Void


******************************************************************************/
void terminaTurno(int sennal) {

	/*envia un mensaje a juez notificando la perdida del turno*/
	sendTurnoAgotadoJugador(queue,jugador);
	turnoFinalizado=TRUE;
	/*(presiona enter ya que se el programa se queda bloqueado dentro del fgets)*/
	printf("\n>Turno agotado, presiona Enter\n");
	
}

/******************************************************************************

Nombre de la función: main

Descripción: 

Devuelve: 0 si todo ha funcionado correctamente
	  -1 si no


******************************************************************************/
int main(int argc, char *argv[]){


	PeticionJugador peticionJugador;
	PeticionJugada peticionjugada;
	int error;	
	Sudoku sudoku;
	Jugada jugada;

	int tablero;

	char buffer[MAX_LINE];
	int cordx,cordy,valor;

	signal(SIGINT,terminaJugador);
	signal(SIGUSR1,terminaJugador);
	signal(SIGUSR2,ganador);
	signal(SIGBUS,perdedor);

	/*Control errores parametros entrada*/
	if(argc!=NUM_ARG){
		printf(">Error el numero de parametros debe ser 1\n");
		exit(-1);
	}
	

	/*Introduce los valores de la estructura Jugador*/
	setNombreJugador(&jugador,argv[1]);
	setIdJugador(&jugador,getpid());

	/*Intoduce los valores de la peticion de jugador*/
	setJugadorPeticionJugador(jugador,&peticionJugador);

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

	/*Nuevo jugador añadido al juego, hace down del semaforo*/
	down(semid,SEM_MAX_JUGADORES);
	
	/*Manda la peticion a juez, lo guardara en memoria compartida*/
	if(sendPeticionJugador(queue,&peticionJugador)==-1){
		perror("sendPeticionJugador: ");

	}
	do{
	
		turnoFinalizado=FALSE;
		system("clear");
		printf("\n>Jugador: %s\n",getNombreJugador(&jugador));
		printf("\n>Esperando turno...\n");
		/*Recibe el turno de jugada*/
		recieveTurnoJugador(queue,jugador);
		system("clear");

		/*Accede a memoria para obtener la tabla*/
		down(semid,SEM_SUDOKU_MUTEX);
		sudoku=*dirSudoku;
		up(semid,SEM_SUDOKU_MUTEX);

		/*Imprime la tabla*/
		ImprimeTablero(sudoku);

		signal(SIGALRM,terminaTurno);
		alarm(TURNO_TIEMPO);

		/*Obtiene las coordenadas*/
		do{
			error=0;
			printf(">Introduzca las coordendas separadas por comas donde desea introducir el valor\n");
			fgets(buffer,MAX_LINE,stdin);
			if(turnoFinalizado)
				break;
			if(sscanf(buffer,"%d,%d",&cordx,&cordy)!=2){
				printf(">Error al obtener las coordenadas, Ej: 1,1\n");
				error=1;
			}
			if(cordx<0||cordy<0)
				error=1;
		}while(error==1);

		/*Si el turno ha finalizando en este momento hace que espera al comienzo su nuevo turno*/
		if(turnoFinalizado)
			continue;

		/*Obtiene el valor*/
		do{
			error=0;
			printf(">Introduzca el valor de esa posicion\n");
			fgets(buffer,MAX_LINE,stdin);
			if(turnoFinalizado)
					break;
			if(sscanf(buffer,"%d",&valor)!=1){
				printf(">Error al obtener el valor, Ej: 1\n");
				error=1;
			}
			if(valor<=0)
				error=1;
		}while(error==1);

		/*para la cuenta atras*/
		alarm(0);

		/*Si el turno ha finalizando en este momento hace que espera al comienzo su nuevo turno*/
		if(turnoFinalizado)
			continue;

		/*Introduce los valores a la jugada*/
		setJugada(&jugada,cordx,cordy,valor);
	
		/*Envia la jugada al proceso juez*/
		setJugadaPeticionJugada(&peticionjugada,jugada);
		sendPeticionJugada(queue,&peticionjugada);
		printf(">\nJugada enviada\n");

		/*Segun responda juez el jugador seguira en su turno o no*/

	}while(1);
return 0;
}
