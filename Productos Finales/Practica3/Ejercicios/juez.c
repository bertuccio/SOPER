/*=== Cabeceras =============================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "semaforos.h"
#include "sudoku.h"
#include "mensajeria.h"
/*===========================================================================*/

void (*signal (int sig, void (*func)(int))) (int);


int jugadores;
int tablero;
int ordenador;
int semid;
int queue;
int *idJugadores=NULL;
Jugador *dirJugadores=NULL;
Sudoku *dirSudoku=NULL;
int idPadre;
int idHijos[MAX_TAM];
boolean comienzoJuego;

/******************************************************************************

Nombre de la función: comienzaJuego

Descripción: Captura la señal SIGALRM y comienza el juego

Devuelve: Void


******************************************************************************/
void comienzaJuego(int sennal){
	comienzoJuego=TRUE;
}


/******************************************************************************

Nombre de la función: terminaJuez

Descripción: Captura la señal SIGINT, libera los recursos y termina

Devuelve: Void


******************************************************************************/
void terminaJuez(int sennal) {
	
	int i;
	/*Si es el padre*/
	if(getpid()==idPadre){
		system("clear");
		printf("\n\n>Terminando\n\n");
	
		i=0;
		/*Envia las señales a los procesos jugador para que terminen*/
		while(idJugadores[i]!=-1&&i<MAX_JUGADORES){
			kill(idJugadores[i],SIGUSR1);
			i++;
		}
		printf("\n>Señal SIGUSR1 enviada a los jugadores\n");

		free(idJugadores);

		i=0;
		/*Finaliza los procesos hijo*/
		while(idHijos[i]!=-1&&i<MAX_TAM){
			kill(idHijos[i],SIGBUS);
			i++;
		}

		/*Envia la señal al proceso ordenador para que termine*/
		kill(ordenador,SIGUSR1);
		printf(">Señal SIGUSR1 enviada al proceso Ordenador\n");

		/*Libera los recursos*/
		if(shmdt(dirJugadores)!=-1)
			printf("\n>Desvinculandose de la Zona Jugadores\n");
	
		if(shmdt(dirSudoku)!=-1)
			printf(">Desvinculandose de la Zona Sudoku\n");
	
		if(shmctl(jugadores, IPC_RMID, 0)!=-1)
			printf("\n>Zona Jugadores shmid: %d eliminada\n",jugadores);
	
		if(shmctl(tablero, IPC_RMID, 0)!=-1)
			printf(">Zona Sudoku shmid: %d eliminada\n",tablero);

		if(borrarSemaforos(semid)!=-1)
			printf("\n>Semaforos borrados\n");

		if(msgctl(queue,IPC_RMID,NULL)!=-1)
			printf("\n>Cola de mensajes borrada\n\n");
		exit(0);
	}
	
}
/******************************************************************************

Nombre de la función: terminaJuezHijos

Descripción: Captura la señal SIGBUS y libera los recursos

Devuelve: Void


******************************************************************************/
void terminaJuezHijos(int sennal) {

	shmdt(dirJugadores);
	
	shmdt(dirSudoku);

	exit(0);
}

/******************************************************************************

Nombre de la función: main

Descripción: 

Devuelve: 0 si todo ha funcionado correctamente
	  -1 si no


******************************************************************************/
int main(){
	
	int i;
	Jugador jugador;
	Jugador tablaJugadores[MAX_JUGADORES];
	Turno turno;
	Ordenador computer;

	
	double maxTamano=MAX_TAM;
	int numBloquesCompletos;
	int inicialx,inicialy,finalx,finaly;
	int turnoJugador;
	int idHijo;

	boolean mensajeJugada=FALSE;
	boolean	mensajeFinTurno=FALSE;
	

	int jugadasNulas;

	PeticionJugador peticionjugador;
	PeticionJugada peticionjugada;

	Sudoku sudoku;
	


	int numJugadores=0;
	
	

	signal(SIGINT,terminaJuez);
	idPadre=getpid();

	if((idJugadores=(int *)malloc(sizeof(int)*MAX_JUGADORES))==NULL)
		return -1;

	signal(SIGALRM,comienzaJuego);
	

	/*****************************************************************************
	***********************CREACION MEMORIA COMPARTIDA****************************
	*****************************************************************************/
	jugadores = shmget(CLAVE_JUGADORES, MAX_JUGADORES*sizeof(Jugador), IPC_CREAT|SHM_R|SHM_W | IPC_EXCL);

	if (jugadores == -1) {

		printf(">La Zona Jugadores ya ha sido creada\n");
		/*Se quita IPC_CREAT si la memoria compartida ya esta creada, devuelve el id de la memoria*/
		jugadores =shmget(CLAVE_JUGADORES,MAX_JUGADORES*sizeof(Jugador),SHM_R|SHM_W);

		if(jugadores==-1){
			printf(">Error al obtener el id de la Zona Jugadores\n");
			exit(-1);
		}
	}
	else
		printf(">La zona Jugadores shmid: %d ha sido creada\n",jugadores);


	tablero = shmget(CLAVE_SUDOKU, sizeof(Sudoku), IPC_CREAT|SHM_R|SHM_W | IPC_EXCL);

	if (tablero == -1) {

		printf(">La Zona Sudoku ya ha sido creada\n");
		/*Se quita IPC_CREAT si la memoria compartida ya esta creada, devuelve el id de la memoria*/
		tablero =shmget(CLAVE_SUDOKU,sizeof(Sudoku),SHM_R|SHM_W);

		if(tablero==-1){
			printf(">Error al obtener el id de la Zona Sudoku\n");
			exit(-1);
		}
	}
	else
		printf(">La zona Sudoku shmid: %d ha sido creada\n",tablero);

	/*****************************************************************************
	********************VINCULACION DE LA MEMORIA COMPARTIDA *********************
	*****************************************************************************/

	/*Vincula la memoria compartida Zona Jugadores*/
	dirJugadores = (Jugador *) shmat (jugadores, NULL, 0);

	if(dirJugadores==(Jugador *)-1){
		printf(">Error al vincularse a la Zona Jugadores %d \n",jugadores);
		exit(-1);
	}
	printf(">Vinculado a la Zona Jugadores\n");

	/*Vincula la memoria compartida Zona Sudoku*/
	dirSudoku = (Sudoku *) shmat (tablero, NULL, 0);

	if(dirSudoku==(Sudoku *)-1){
		printf(">Error al vincularse a la Zona Sudoku %d \n",tablero);
		exit(-1);
	}
	printf(">Vinculado a la Zona Sudoku\n");

	/*****************************************************************************
	*****************INICIALIZACION DE LA MEMORIA COMPARTIDA *********************
	*****************************************************************************/
	for(i=0;i<MAX_JUGADORES;i++){

		*(dirJugadores+i)=iniJugador();
	}

	/*****************************************************************************
	************************CREACION DE LOS SEMAFOROS*****************************
	*****************************************************************************/
	semid=creaSemaforos(CLAVE_SEMAFORO,10);
	if (semid == -1) {
		perror(">semget:");
		exit(errno);
	}
	else
		printf("\n>Semaforos creados\n");



	/*Inicializa el semaforo SEM_MAX_JUGADORES a NUM_JUGADORES*/
	setValorSemaforo(semid,SEM_MAX_JUGADORES,MAX_JUGADORES);
	/*Inicializa el semaforo SEM_Sudoku_MUTEX a 1 para la zona de almacenamiento del juego*/
	setValorSemaforo(semid,SEM_SUDOKU_MUTEX,1);
	/*Inicializa el semaforo SEM_Sudoku_MUTEX a 1 para la zona de almacenamiento de jugadores*/
	setValorSemaforo(semid,SEM_JUGADOR_MUTEX,1);

	/*****************************************************************************
	***********************CREACION DE LA COLA DE MENSAJES************************
	*****************************************************************************/
	queue = msgget(CLAVE_MENSAJES, IPC_CREAT|SHM_R|SHM_W);
	if (queue == -1) {
		printf(">La cola de mensajes ya ha sido creada\n");
		queue = msgget(CLAVE_MENSAJES,SHM_R|SHM_W);
		if(queue==-1){
			perror("msgget:");
			raise(SIGINT);
		}
	}
	printf(">La cola de mensajes ha sido creada\n");

	/*Recibe el pid del proceso Ordenador*/
	printf("\n>Esperando al proceso Ordenador:\n");
	recieveOrdenador(queue,&computer);
	ordenador=getOrdenador(computer);
	

	/****************************************************************************/
	do{
		for(i=0;i<MAX_JUGADORES;i++)
			tablaJugadores[i]=iniJugador();
		for(i=0;i<MAX_TAM;i++)
			idHijos[i]=-1;
		/*Inicializa el Sudoku*/
		numBloquesCompletos=0;
		generaTablero(&sudoku,ESPACIOS);

		/*Introduce el Sudoku en memoria*/
		down(semid,SEM_SUDOKU_MUTEX);
		*dirSudoku=sudoku;
		up(semid,SEM_SUDOKU_MUTEX);

		numBloquesCompletos=0;
		iniIdJugadores(idJugadores);
		numJugadores=0;
		comienzoJuego=FALSE;
		/*Recibe peticiones de jugadores*/
		printf("\n>Esperando a los jugadores\n");
		do{
			
			recievePeticionJugador(queue,&peticionjugador);
			if(comienzoJuego==FALSE){
		
				jugador=getJugadorPeticionJugador(peticionjugador);
				alarm(0);
				/*Introduce el pid del jugador en el array de pids*/
				idJugadores[numJugadores]=getIdJugador(getJugadorPeticionJugador(peticionjugador));

				down(semid,SEM_JUGADOR_MUTEX);
				*(dirJugadores+numJugadores)=getJugadorPeticionJugador(peticionjugador);
				up(semid,SEM_JUGADOR_MUTEX);

				alarm(TIEMPO_INICIO);

				numJugadores++;	
				/*si el numero de jugadores es igual al maximo comienza inmediatamente*/		
				if(numJugadores==MAX_JUGADORES)
					comienzoJuego=TRUE;

			}
		}while(comienzoJuego==FALSE);
	
		/*Crea los hijos y cada uno obtiene las limitaciones de su bloque*/
		inicialx=0;
		inicialy=0;
		finalx=0;
		finaly=0;
		for(i=0;i<MAX_TAM;i++){
			finalx=inicialx+sqrt(maxTamano)-1;
			finaly=inicialy+sqrt(maxTamano)-1;
			idHijo=fork();
			if(idHijo==-1){
				printf("Error al crear el proceso hijo\n");
				raise(SIGINT);
			}
			if(idHijo==0)
				break;
			if(idHijo>0){
				idHijos[i]=idHijo;
				inicialx=finalx+1;
				if(inicialx==MAX_TAM){
					inicialx=0;
					inicialy=finaly+1;
					if(inicialy==MAX_TAM)
						inicialy=0;
				}
				continue;
			}
		
		}

		/*Sondeo de los procesos hijos*/
		if(idHijo==0){
			signal(SIGBUS,terminaJuezHijos);
			do{
			
				down(semid,SEM_SUDOKU_MUTEX);
				sudoku=*dirSudoku;
				up(semid,SEM_SUDOKU_MUTEX);
				if(estaBloqueCompleto(sudoku,inicialx,inicialy,finalx,finaly)==TRUE){
					sendBloqueCompleto(queue);
					exit(0);
				}
				sleep(0.5);
			}while(1);
			exit(0);
		}





		/*Proceso Padre*/
		/*Empieza la partida*/
		if(idHijo>0){
			jugadasNulas=0;
			turnoJugador=0;
			do{
			
				/*Saca al jugador de memoria*/
				down(semid,SEM_JUGADOR_MUTEX);
				jugador=*(dirJugadores+turnoJugador);
				up(semid,SEM_JUGADOR_MUTEX);
				/*envia el turno al jugador*/
				sendTurnoJugador(queue,jugador);
				system("clear");
				printf("Turno del Jugador %s\n",getNombreJugador(&jugador));
				/*Turnos*/
				mensajeJugada=FALSE;
				mensajeFinTurno=FALSE;
				do{
		
					if(numBloquesCompletos==MAX_TAM){
						up(semid,SEM_SUDOKU_MUTEX);
						break;
					}
					if(recieveBloqueCompleto(queue)!=-1){
						wait(0);
						numBloquesCompletos++;
					}
					/*Recibe peticion de jugada por parte del jugador*/
					if(recievePeticionJugada(queue,&peticionjugada)!=-1){
						down(semid,SEM_SUDOKU_MUTEX);
						/*Si es correcta*/
						if(realizaJugada(dirSudoku,
						getJugadaPeticionJugada(&peticionjugada))==TRUE){
							up(semid,SEM_SUDOKU_MUTEX);
							jugador.jugadasCorrectas++;
							down(semid,SEM_JUGADOR_MUTEX);
							*(dirJugadores+turnoJugador)=jugador;
							up(semid,SEM_JUGADOR_MUTEX);
							sleep(0.5);
							if(recieveBloqueCompleto(queue)!=-1){
								wait(0);
								numBloquesCompletos++;
							}
							if(numBloquesCompletos==MAX_TAM){
								up(semid,SEM_SUDOKU_MUTEX);
								break;
							}
							jugadasNulas=0;
							/*sigue en su turno*/
							sendTurnoJugador(queue,jugador);
					
						}
						/*si no es correcta sale del bucle*/
						else{
							mensajeFinTurno=TRUE;
						}	
						up(semid,SEM_SUDOKU_MUTEX);
					}	
					/*si recibe un mensaje de finalizacion de turno*/	
					else if(recieveFinTurnoJugador(queue,&turno,jugador)!=-1){
						mensajeFinTurno=TRUE;
						//printf("queteapsa\n");
					}
				}while(mensajeFinTurno==FALSE);
		
				/*si el turno ha sido perdido*/
				if(turnoAgotado(turno)){
					jugadasNulas++;
				}
				/*Si hay tantas jugadas nulas como jugadores recurrimos a las pistas*/
				if(jugadasNulas==numJugadores){
					sendJugadaOrdenador(queue);
					if(recieveBloqueCompleto(queue)!=-1){
						wait(0);
						numBloquesCompletos++;
					}
					jugadasNulas=0;
				}
				/*Si se han completado todos los bloques finaliza la partida*/
				if(numBloquesCompletos==MAX_TAM){	
					for(i=0;i<numJugadores;i++){
						down(semid,SEM_JUGADOR_MUTEX);
						tablaJugadores[i]=*(dirJugadores+i);
						*(dirJugadores+i)=iniJugador();
						up(semid,SEM_JUGADOR_MUTEX);
					}
					finDePartida(queue,tablaJugadores,numJugadores);
				}
			
				/*Pasa al siguiente jugador en la memoria compartida*/
				turnoJugador++;
				
				if(turnoJugador==numJugadores)
					turnoJugador=0;

		
			/*Mientras no esten todos los bloques completos*/
			}while(numBloquesCompletos<MAX_TAM);
		}
		
	}while(1);
return 1;
}
