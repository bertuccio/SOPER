/*=== Cabeceras =============================================================*/
#include <stdio.h>
#include "peticion.h"
#include "semaforos.h"

/*===========================================================================*/

#define NUM_ARG 2

int zona1;
int zona2;
int semid;
Peticion *dir1=NULL;
Peticion *dir2=NULL;

void (*signal (int sig, void (*func)(int))) (int);

/******************************************************************************

Nombre de la función: termina

Descripción: Captura la señal SIGINT, libera los recursos y termina

Devuelve: Void


******************************************************************************/
void termina(int sennal) {

	printf("\n>Terminando\n");

	if(shmdt(dir1)!=-1){
		printf(">Desvinculandose de la Zona 1\n");
	}
	if(shmdt(dir2)!=-1){
		printf(">Desvinculandose de la Zona 2\n");
	}

	if(shmctl(zona1, IPC_RMID, 0)!=-1)
		printf("\n>Zona 1 shmid: %d eliminada\n",zona1);	
	if(shmctl(zona2, IPC_RMID, 0)!=-1)
		printf("\n>Zona 2 shmid: %d eliminada\n",zona2);
	if(borrarSemaforos(semid)!=-1)
		printf("\n>Semaforos borrados\n");
	
	exit(0);
}


/******************************************************************************

Nombre de la función: main

Descripción: monitoriza la memoria compartida y los procesos que entran a ella

Devuelve: 0 si todo ha funcionado correctamente
	  -1 si no


******************************************************************************/
int main(int argc, char *argv[]){

	int i;
	int numPeticionesZona2;
	int numPeticionesZona1;

	int numeroMensajes;

	Peticion peticion1;
	Peticion peticion2;


	signal(SIGINT,termina);
	
	/*Control Errores parametros entrada*/
	if(argc!=NUM_ARG){
		printf(">Error el numero de parametros debe ser 1\n");
		exit(-1);
	}
	if((numeroMensajes=atoi(argv[1]))<=0){
		printf(">Error el numero de mensajes debe ser mayor que 0\n");
		exit(-1);
	}

	/*****************************************************************************
	***********************CREACION MEMORIA COMPARTIDA****************************
	*****************************************************************************/
	zona1 = shmget(CLAVE_ZONA1, numeroMensajes*sizeof(Peticion), IPC_CREAT|SHM_R|SHM_W | IPC_EXCL);

	if (zona1 == -1) {

		printf(">La Zona 1 ya ha sido creada\n");
		/*Se quita IPC_CREAT si la memoria compartida ya esta creada, devuelve el id de la memoria*/
		zona1 =shmget(CLAVE_ZONA1,numeroMensajes*sizeof(Peticion),SHM_R|SHM_W);

		if(zona1==-1){
			printf(">Error al obtener el id de la Zona 1\n");
			exit(-1);
		}
	}
	else
		printf(">La zona 1 shmid: %d ha sido creada\n",zona1);


	zona2 = shmget(CLAVE_ZONA2, numeroMensajes*sizeof(Peticion), IPC_CREAT|SHM_R|SHM_W | IPC_EXCL);

	if (zona2 == -1) {

		printf(">La Zona 2 ya ha sido creada\n");
		/*Se quita IPC_CREAT si la memoria compartida ya esta creada, devuelve el id de la memoria*/
		zona2 =shmget(CLAVE_ZONA2,numeroMensajes*sizeof(Peticion),SHM_R|SHM_W);

		if(zona2==-1){
			printf(">Error al obtener el id de la memoria compartida de la Zona 2\n");
			exit(-1);
		}
	}
	else
		printf(">La zona 2 shmid: %d ha sido creada\n",zona2);

	
	/*Vincula la memoria compartida Zona 1*/
	dir1 = (Peticion *) shmat (zona1, NULL, 0);

	if(dir1==(Peticion *)-1){
		printf("Error al vincularse a la Zona 1 %d \n",zona1);
		exit(-1);
	}
	printf(">Vinculado a la Zona 1\n");
	/*Vincula la memoria compartida Zona 2*/
	dir2 = (Peticion *) shmat (zona2, NULL, 0);

	if(dir2==(Peticion *)-1){
		printf("Error al vincularse a la Zona 2 %d \n",zona2);
		exit(-1);
	}
	printf(">Vinculado a la Zona 2\n");

	/*****************************************************************************
	*****************INICIALIZACION DE LA MEMORIA COMPARTIDA *********************
	*****************************************************************************/
	for(i=0;i<numeroMensajes;i++){

		*(dir1+i)=iniPeticion();
		*(dir2+i)=iniPeticion();
	}

	/*****************************************************************************
	***************************CREACION SEMAFOROS*********************************
	*****************************************************************************/
	semid=creaSemaforos(CLAVE_SEMAFORO,10);
	if (semid == -1) {
		perror("semget:");
		exit(errno);
	}
	else
		printf("\n>Semaforos creados\n");

	/*Inicializa el semaforo mutex para los procesos escritores-lector de las dos zonas*/
	setValorSemaforo(semid,SEM_ZONA1_MUTEX,1);
	setValorSemaforo(semid,SEM_ZONA2_MUTEX,1);

	/*Inicializa los semaforos de n peticiones para evitar el acceso a la memoria, si esta llena*/
	setValorSemaforo(semid,SEM_ZONA1_NPETICIONES,numeroMensajes);
	setValorSemaforo(semid,SEM_ZONA2_NPETICIONES,numeroMensajes);
	/*Semaforo que servira de constante para obtener el numero maximo de peticiones*/
	setValorSemaforo(semid,SEM_MAX_PETICIONES,numeroMensajes);


	do{
		sleep(0.5);
		numPeticionesZona1=0;
		numPeticionesZona2=0;
		down(semid,SEM_ZONA1_MUTEX);
		down(semid,SEM_ZONA2_MUTEX);
		for(i=0;i<numeroMensajes;i++){
			
			peticion1=(Peticion)*(dir1+i);
			peticion2=(Peticion)*(dir2+i);
			if(getIdPeticion(&peticion1)!=-1)
				numPeticionesZona1++;
			if(getIdPeticion(&peticion2)!=-1)
				numPeticionesZona2++;
		}
		printf("Peticiones en la Zona 1: %d\n",numPeticionesZona1);
		printf("Peticiones en la Zona 2: %d\n",numPeticionesZona2);
		up(semid,SEM_ZONA1_MUTEX);
		up(semid,SEM_ZONA2_MUTEX);
		
	}while(1);
		
		
	

exit(0);
}
