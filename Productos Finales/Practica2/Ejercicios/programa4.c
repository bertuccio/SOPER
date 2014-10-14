#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <wait.h>
#include <unistd.h>

#define CLAVE_MEMORIA 1555

int main() {

	int shm;
	int idHijo;
	int *dir=NULL;

	shm = shmget(CLAVE_MEMORIA, sizeof(int), IPC_CREAT|SHM_R|SHM_W | IPC_EXCL);

	if (shm == -1) {

		printf(">El segmento de memoria compartida ya ha sido creado\n");
		/*Se quita IPC_CREAT si la memoria compartida ya esta creada, devuelve el id de la memoria*/
		shm =shmget(CLAVE_MEMORIA, sizeof(int),SHM_R|SHM_W);

		if(shm==-1){
			printf("Error al obtener el id de la memoria compartida\n");
			exit(-1);
		}
	}
	else
		printf("\n>El segmento de memoria compartida ha sido creado\n");

		printf("************************************************************************\n");
		system("ipcs -m");
		printf("************************************************************************\n");
		        
	/*Vincula la memoria compartida*/
	dir = (int *) shmat (shm, NULL, 0);
	if(dir==(int *)-1){
		printf("Error al obtener la dirección de memoria compartida\n");
		/*elimina la memoria compartida (no se elimina hasta que no haya ningún proceso vinculado)*/
		if(shmctl(shm,IPC_RMID,NULL)==-1){
			printf("Error al eliminar la memoria compartida\n");
			exit(-1);
		}
		exit(-1);
	}
	printf(">Proceso Padre Vinculado a memoria compartida\n");
	

	idHijo=fork();

	if(idHijo<0){
		printf("Error al crear el proceso hijo\n");
		exit(-1);
	}

	if(idHijo==0){
		/*PROCESO HIJO*/

		/*elimina la memoria compartida (no se elimina hasta que no haya ningún proceso vinculado)*/
		if(shmctl(shm,IPC_RMID,NULL)==-1){
			printf("Error al eliminar la memoria compartida\n");
			exit(-1);
		}
		printf(">Memoria compartida marcada como borrada por el proceso Hijo\n");

		printf("************************************************************************\n");
		system("ipcs -m");
		printf("************************************************************************\n");

		exit(0);
	}
	if(idHijo>0){
		/*PADRE*/

		wait(0);
		
		/*Desvincula la memoria compartida antes de eliminarla*/
		if(shmdt(dir)==-1){
			printf("Error al desvincular la memoria compartida\n");
			exit(-1);
		}
		printf(">Proceso Padre desvinculado de la memoria compartida\n");

		printf(">Memoria compartida borrada por el Sistema Operativo \n");

		printf("************************************************************************\n");
		system("ipcs -m");
		printf("************************************************************************\n");


	}
exit(0);

}
