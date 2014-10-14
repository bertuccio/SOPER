/*=== Cabeceras =============================================================*/
#include<stdio.h>
#include"semaforos.h"
/*=== Funciones==============================================================*/
int main (){


	int shm;
	int *dir=NULL;
	int semid;

	/*Crea los semaoforos*/
	semid =creaSemaforos(CLAVE_SEMAFORO, 1);

	if (semid == -1) {
		perror("semget:");
		exit(errno);
	}
	else
		printf("\n>Semaforo creado\n");


	// inicializar el semáforo mutex a 1
	if(setValorSemaforo(semid,SEM_MUTEX,1)==-1){
		printf("Error al incializar el semáforo\n");

		/*Elimina los semaforos*/
		borrarSemaforos(semid);
		exit(-1);
	}
	else
		printf(">Incialización del semáforo SEM_MUTEX a 1\n");

	system("ipcs -s");
	/*Crea o accede a la memoria compartida*/
	shm = shmget(CLAVE_MEMORIA, sizeof(int), IPC_CREAT|SHM_R|SHM_W | IPC_EXCL);

	if (shm == -1) {

		printf(">El segmento de memoria compartida ya ha sido creado\n");
		/*Se quita IPC_CREAT si la memoria compartida ya esta creada, devuelve el id de la memoria*/
		shm =shmget(CLAVE_MEMORIA, sizeof(int),SHM_R|SHM_W);

		if(shm==-1){
			printf("Error al obtener el id de la memoria compartida\n");
			/*Elimina los semaforos*/
			borrarSemaforos(semid);
			exit(-1);
		}
	}
	else
		printf("\n>El segmento de memoria compartida ha sido creado\n");

	system("ipcs -m");
	/*Vincula la memoria compartida*/
	dir = (int *) shmat (shm, NULL, 0);

	if(dir==(int *)-1){
		printf("Error al obtener la dirección de memoria compartida\n");
		/*elimina la memoria compartida (no se elimina hasta que no haya ningún proceso vinculado)*/
		if(shmctl(shm,IPC_RMID,NULL)==-1){
			printf("Error al eliminar la memoria compartida\n");
			/*Elimina los semaforos*/
			borrarSemaforos(semid);
			exit(-1);
		}
		exit(-1);
	}
	printf(">Proceso vinculado a memoria compartida\n");


	printf("Antes del down: Valor del semaforo SEM_MUTEX %d\n",getValorSemaforo(semid,SEM_MUTEX));
	if((down(semid, SEM_MUTEX))==-1){

		printf("Error al hacer el down en el semaforo SEM_MUTEX\n");

		if(shmdt(dir)==-1){
			printf("Error al desvincular la memoria compartida\n");
			exit(-1);
		}

		if(shmctl(shm,IPC_RMID,NULL)==-1){
			printf("Error al eliminar la memoria compartida\n");
			exit(-1);
		}

		/*Elimina los semaforos*/
		borrarSemaforos(semid);

	}

	*dir=2;
	printf("Entre medias: Valor del semaforo SEM_MUTEX %d\n",getValorSemaforo(semid,SEM_MUTEX));

	if((up(semid, SEM_MUTEX))==-1){

		printf("Error al hacer el up en el semaforo SEM_MUTEX\n");
		/*elimina la memoria compartida (no se elimina hasta que no haya ningún proceso vinculado)*/
		if(shmctl(shm,IPC_RMID,NULL)==-1){
			printf("Error al eliminar la memoria compartida\n");
			exit(-1);
		}
		/*Elimina los semaforos*/
		borrarSemaforos(semid);

	}
	printf("Despues del up: Valor del semaforo SEM_MUTEX %d\n",getValorSemaforo(semid,SEM_MUTEX));

	/*Elimina los semaforos*/
	borrarSemaforos(semid);


	/*Desvincula la memoria*/
	if(shmdt(dir)==-1){
		printf("Error al desvincular la memoria compartida\n");
		exit(-1);
	}
	/*elimina la memoria compartida (no se elimina hasta que no haya ningún proceso vinculado)*/
	if(shmctl(shm,IPC_RMID,NULL)==-1){
		printf("Error al eliminar la memoria compartida\n");
		exit(-1);
	}
	system("ipcs -m");
	system("ipcs -s");
	exit(1);
}
