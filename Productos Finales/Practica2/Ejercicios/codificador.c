/*=== Cabeceras =============================================================*/
#include <stdio.h>
#include "peticion.h"
#include "semaforos.h"
#include "utilidades.h"
/*===========================================================================*/

#define NUM_ARG 3

int *arrayIdHijo=NULL;
int numProcesos;

void (*signal (int sig, void (*func)(int))) (int);



void termina(int sennal) {

	int i;
	printf("\n>Codificacion Abortada\n");
	for(i=0;i<numProcesos;i++){
		kill(arrayIdHijo[i],SIGUSR1);
	}
	free(arrayIdHijo);
}

void terminaHijo(int sennal) {
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
	int j;
	int z;
	
        int semid;
	int zona1;
	int zona2;
	
	Peticion *dir1=NULL;
	Peticion *dir2=NULL;
	Peticion peticionaux;/*auxiliar de la zona 1*/
	Peticion peticionaux2;/*auxiliar de la zona 2*/
	int idPeticion;

	Peticion respuesta;

	char clave[MAX_LINE];

	sigset_t mascara;

	/*Control Errores parametros entrada*/
	if(argc!=NUM_ARG){
		printf(">Error el numero de parametros debe ser 2\n");
		exit(-1);
	}
	if((numProcesos=atoi(argv[1]))<=0){
		printf(">Error el numero de proceso debe ser mayor que 0\n");
		exit(-1);
	}
	if(charvalidos(argv[2])==-1){
		exit(-1);
	}
	strcpy(clave,argv[2]);
	/*Accede a la memoria compartida Zona 1*/
	zona1 =shmget(CLAVE_ZONA1,sizeof(Peticion),SHM_R|SHM_W);

	if(zona1==-1){
		printf(">Error al obtener el id de la Zona 1, es posible que todavia no haya sido creada\n");
		printf(">Esperando a la creacion de la Zona 1...\n");
		do{
			zona1 =shmget(CLAVE_ZONA1,sizeof(Peticion),SHM_R|SHM_W);
		}while(zona1==-1);
		printf("\n>Zona 1 shmid: %d acccedida\n",zona1);
	}
	else
		printf("\n>Zona 1 shmid: %d acccedida\n",zona1);

	/*Accede a la memoria compartida Zona 2*/
	zona2 =shmget(CLAVE_ZONA2,sizeof(Peticion),SHM_R|SHM_W);

	if(zona2==-1){
		printf(">Error al obtener el id de la Zona 2, es posible que todavia no haya sido creada\n");
		printf(">Esperando a la creacion de la Zona 2...\n");
		do{
			zona2 =shmget(CLAVE_ZONA2,sizeof(Peticion),SHM_R|SHM_W);
		}while(zona2==-1);
		printf("\n>Zona 2 shmid: %d acccedida\n",zona2);
	}
	else
		printf("\n>Zona 2 shmid: %d acccedida\n",zona2);
	
	/*Vincula la memoria compartida Zona 1*/
	dir1 = (Peticion *) shmat (zona1, NULL, 0);

	if(dir1==(Peticion *)-1){
		printf(">Error al vincularse a la Zona 1 %d \n",zona1);
		
		exit(-1);
	}

	/*Vincula la memoria compartida Zona 2*/
	dir2 = (Peticion *) shmat (zona2, NULL, 0);

	if(dir2==(Peticion *)-1){
		printf(">Error al vincularse a la Zona 2 %d \n",zona2);

		exit(-1);
	}

	/*Accediendo a los semaforos*/
	semid=creaSemaforos(CLAVE_SEMAFORO,10);
	if (semid == -1) {
		perror("semget:");
		exit(errno);
	}


	if((arrayIdHijo= (int *)malloc(sizeof(int)*numProcesos))==NULL){
		printf(">Error al reservar memoria\n");
		exit(-1);
	}

	for(i=0;i<numProcesos;i++){

		arrayIdHijo[i]=fork();
		
		if(arrayIdHijo[i]<0){
			printf(">Error al crear proceso hijo\n");
			exit(-1);
		}
		/*Si soy hijo guardo el id de la peticion correspondiente y salgo del bucle*/
		if(arrayIdHijo[i]==0){
			break;
		}
		
	}
	signal(SIGINT,termina);
	/*Busca una peticion en la zona 1 de memoria compartida y sale del bucle*/
	if(arrayIdHijo[i]==0){
		signal(SIGUSR1,terminaHijo);
		j=0;
		down(semid,SEM_ZONA1_MUTEX);
			
		do{
			if(j==getValorSemaforo(semid,SEM_MAX_PETICIONES)){
				up(semid,SEM_ZONA1_MUTEX);
				sleep(0.5);
				printf(">Esperando... \n");
				j=0;
				down(semid,SEM_ZONA1_MUTEX);	
				continue;
			}
			peticionaux=(Peticion)*(dir1+j);
			if(getIdPeticion(&peticionaux)==-1){
				up(semid,SEM_ZONA1_MUTEX);
				j++;
				down(semid,SEM_ZONA1_MUTEX);
			}
			else{
				idPeticion=j;
				break;
			}
		}while(1);
	
		/*mediante el indice libera esa posicion de la zona de memoria*/
		*(dir1+idPeticion)=iniPeticion();
		up(semid,SEM_ZONA1_MUTEX);
		up(semid,SEM_ZONA1_NPETICIONES);

		/* A partir de este momento se ignoraran las señales para evitar la interrupcion en el 
		procesamiento de la peticion*/
		sigfillset(&mascara);
		
		/*procesa el texto mediante vigenere y crea la respuesta con el id de la peticion*/
		setPeticion(getIdPeticion(&peticionaux),vignere(clave,getTextoPeticion(&peticionaux)),&respuesta);

		down(semid,SEM_ZONA2_NPETICIONES);
		down(semid,SEM_ZONA2_MUTEX);
		/*Busca una posicion libre en la zona 2 de memoria compartida para introducir la respuesta*/
		z=0;                                                                
		do{
			/*Si ha llegado al final de la memoria compartida
			vuelve al inicio de la misma*/
			if(z==getValorSemaforo(semid,SEM_MAX_PETICIONES)){
				up(semid,SEM_ZONA2_MUTEX);
				z=0;
				down(semid,SEM_ZONA2_MUTEX);
				continue;
			}
			peticionaux2=*(dir2+z);
			/*si encuentra una posicion libre introduce la respuesta*/
			if(getIdPeticion(&peticionaux2)==-1){
				*(dir2+z)=respuesta;
				break;
			}
			/*incrementa el iterador*/
			else{
				up(semid,SEM_ZONA2_MUTEX);
				z++;
				down(semid,SEM_ZONA2_MUTEX);
			}
		}while(1);
		up(semid,SEM_ZONA2_MUTEX);
		sigemptyset(&mascara);
		exit(0);

	}
	if(arrayIdHijo[i]>0){
		printf("\n>Codificacion Finalizada\n");
		/*Espera a los hijos*/
		for(i=0;i<numProcesos;i++)
			wait(0);
		exit(0);
	}

	exit(0);
}
