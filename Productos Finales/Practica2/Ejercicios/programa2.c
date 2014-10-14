#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#define CLAVE_MEMORIA 2556
#define SHM_MODE (IPC_CREAT | SHM_R | SHM_W | IPC_EXCL) 

int main() {

	int shmid;
	shmid = shmget(CLAVE_MEMORIA, sizeof(int), SHM_MODE ); /*EEXIST*/
	
	if (shmid == -1) {
		perror("shmget:");
		exit(-1);
	}
	printf("\n\n/*****CREACION DEL SEGMENTO %d*****/\n\n",shmid);
	system("ipcs");	
	if ((shmctl(shmid, IPC_RMID, 0))<0){
	//status = shmctl(shm,IPC_RMID, NULL);
		perror("shmctl:");
		exit(-1);
	}
	printf("\n\n/*****ELIMINACION DEL SEGMENTO %d*****/\n\n",shmid);
	system("ipcs");
	exit(0);
}
