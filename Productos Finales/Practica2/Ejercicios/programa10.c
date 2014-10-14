/*=== Cabeceras =============================================================*/
#include<stdio.h>
#include<wait.h>
#include"semaforos.h"
/*=== Funciones==============================================================*/
int main (){


	int shm;
	int *dir=NULL;
        int idHijo;
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


	idHijo=fork();

	if(idHijo<0){

		printf("Error al crear el proceso hijo\n");
		/*Desvincula la memoria compartida antes de eliminarla*/
		if(shmdt(dir)==-1){
			printf("Error al desvincular la memoria compartida\n");
			/*Elimina los semaforos*/
			borrarSemaforos(semid);
			exit(-1);
		}
		/*elimina la memoria compartida (no se elimina hasta que no haya ningún proceso vinculado)*/
		if(shmctl(shm,IPC_RMID,NULL)==-1){
			printf("Error al eliminar la memoria compartida\n");
			/*Elimina los semaforos*/
			borrarSemaforos(semid);
			exit(-1);
		}
		exit(0);
	}
	
	if(idHijo>0){

		/*PROCESO PADRE*/

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
		sleep(1);
		printf("Numero de procesos esperando %d\n",getProcesosEsperando(semid,SEM_MUTEX));

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

		wait(0);

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
		
		exit(0);
		
	}


	if(idHijo==0){
		

		/*PROCESO HIJO*/

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

		*dir=5;
		sleep(1);
		printf("Numero de procesos esperando %d\n",getProcesosEsperando(semid,SEM_MUTEX));

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
		exit(0);

	}

exit(0);
}
