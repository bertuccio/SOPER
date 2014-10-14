/*=== Cabeceras =============================================================*/
#include <stdio.h>
#include "peticion.h"
#include "semaforos.h"
/*===========================================================================*/

#define NUM_ARG 3

/******************************************************************************

Nombre de la función: main

Descripción: introduce las peticiones en la memoria compartida y lee los 
	     resultados

Devuelve: 0 si todo ha funcionado correctamente
	  -1 si no


******************************************************************************/
int main(int argc, char *argv[]){

	
	FILE *filein=NULL;
	FILE *fileout=NULL;

	char buffer[MAX_LINE_TEXTO];

	int linea;/*Contendra el numero delineas leidas del fichero( numero de peticiones)*/

	int i;
	int j;
	int z;

	int numProcesados;

	Peticion *dir1=NULL;
	Peticion *dir2=NULL;
	Peticion peticion[MAX_LINE_FILE];
	Peticion peticionaux;/*sirve para evaluar si hay algun espacio libre en memoria (id=-1)*/
	Peticion peticionaux2;/*sirve para evaluar si hay algun espacio libre en memoria (id=-1)*/

	int idHijo;
	int idPeticionHijo;/*representa la peticion que cada proceso hijo va a tener a su cargo*/
	int idPeticion;

	int semid;
	int zona1;
	int zona2;

	/*Control Errores parametros entrada*/
	if(argc!=NUM_ARG){
		printf(">Error el numero de parametros debe ser 2\n");
		exit(-1);
	}
	if((filein=fopen(argv[1],"r"))==NULL){
		printf(">Error el fichero de lectura con el nombre %s no existe\n",argv[1]);
		exit(-1);
	}
	if((fileout=fopen(argv[2],"w"))==NULL){
		printf(">Error al crear el fichero %s\n",argv[2]);
		fclose(filein);
		exit(-1);
	}
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
		printf("Error al vincularse a la Zona 1 %d \n",zona1);
		fclose(filein);
		fclose(fileout);
		exit(-1);
	}

	/*Vincula la memoria compartida Zona 2*/
	dir2 = (Peticion *) shmat (zona2, NULL, 0);

	if(dir2==(Peticion *)-1){
		printf("Error al vincularse a la Zona 2 %d \n",zona2);
		fclose(filein);
		fclose(fileout);
		exit(-1);
	}
	/*Accediendo a los semaforos*/
	semid=creaSemaforos(CLAVE_SEMAFORO,10);
	if (semid == -1) {
		perror("semget:");
		exit(errno);
	}
	/*Mientras no llegue al maximo de lineas de fichero y el fichero no acabe inicializa las peticiones*/
	for(linea=0; linea<MAX_LINE_FILE && (fgets(buffer,MAX_LINE_TEXTO,filein)!=NULL) ;linea++){

		/*Elimina caracteres molestos e inicializa las peticiones*/
		tratamientoTexto(buffer);
		setPeticion(linea,buffer,&peticion[linea]);
	}
	/*Si soy el padre sigo en el bucle y creo tantos hijos como peticiones realizadas*/
	for(i=0;i<linea;i++){

		idHijo=fork();
		
		if(idHijo<0){
			printf("Error al crear proceso hijo\n");
			exit(-1);
		}
		/*Si soy hijo guardo el id de la peticion correspondiente y salgo del bucle*/
		if(idHijo==0){
			idPeticionHijo=i;
			break;
		}
		
	}
	
	if(idHijo==0){
		/*Introducira la peticion en la primera posicion de memoria compartida disponible*/
		j=0;
		down(semid,SEM_ZONA1_NPETICIONES);
		down(semid,SEM_ZONA1_MUTEX);

		do{
			/*si llega al final de la memoria compartida reinicia el iterador y continua*/
			if(j==getValorSemaforo(semid,SEM_MAX_PETICIONES)){
				up(semid,SEM_ZONA1_MUTEX);
				j=0;
				down(semid,SEM_ZONA1_MUTEX);
				continue;
			}
			peticionaux=*(dir1+j);
			/*si encuentra una posicion libre introduce la peticion*/
			if(getIdPeticion(&peticionaux)==-1){
				*(dir1+j)=peticion[idPeticionHijo];
				break;
			}
			/*si no continua iterando*/
			else{
				up(semid,SEM_ZONA1_MUTEX);
				j++;
				down(semid,SEM_ZONA1_MUTEX);
			}	
		}while(1);

		up(semid,SEM_ZONA1_MUTEX);


		exit(0);
	}

	if(idHijo>0){

		numProcesados=0;
			z=0;
			down(semid,SEM_ZONA2_MUTEX);
			/*Busca una respuesta en la zona 2*/
			do{
				/*Si llega al final de la memoria reinica el iterador y continua*/
				if(z==getValorSemaforo(semid,SEM_MAX_PETICIONES)){
					up(semid,SEM_ZONA2_MUTEX);
					sleep(0.5);
					printf("Esperando... \n");
					z=0;
					down(semid,SEM_ZONA2_MUTEX);
					continue;
				}
				peticionaux2=(Peticion)*(dir2+z);
				/*Si la encuentra la libera de memoria principal y la imprime en el fichero de salida*/
				if(getIdPeticion(&peticionaux2)!=-1){
					idPeticion=z;
					*(dir2+idPeticion)=iniPeticion();
		
					fprintf(fileout,"%d %s\n",getIdPeticion(&peticionaux2),getTextoPeticion(&peticionaux2));
			
					up(semid,SEM_ZONA2_MUTEX);
					up(semid,SEM_ZONA2_NPETICIONES);
					numProcesados++;
					sleep(0.5);
					z=0;
					down(semid,SEM_ZONA2_MUTEX);
					continue;
				
				}
				else{
					/*Si no itera entre las posiciones de memoria*/
					up(semid,SEM_ZONA2_MUTEX);
					z++;
					down(semid,SEM_ZONA2_MUTEX);
				}
			}while(numProcesados<linea);
			
		up(semid,SEM_ZONA2_MUTEX);

		/*Espera a los hijos*/
		for(i=0;i<linea;i++)
			wait(0);
		printf("\n>Fichero cifrado mediante vignère\n");
		exit(0);
	}	
		

fclose(filein);
fclose(fileout);
exit(0);/*Se desvincula automáticamente de la memoria compartida*/

}
