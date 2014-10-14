#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include<stdio.h>

#define CLAVE_SEMAFORO 2000
#define NUM_SEMAF 10

       #if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
       /* la union semun se define al incluir <sys/sem.h> */
       #else
       /* segÃºn X/OPEN tenemos que definirla nosostros mismos */
       union semun {
               int val;                    /* valor para SETVAL */
               struct semid_ds *buf;       /* buffer para IPC_STAT, IPC_SET */
               unsigned short int *array;  /* array para GETALL, SETALL */
               struct seminfo *__buf;      /* buffer para IPC_INFO */
       };
#endif	


int main() {

      int semid;
      union semun arg;
      int i;

      /*Crea los semaforos*/
      semid = semget(CLAVE_SEMAFORO, 10, IPC_CREAT|IPC_EXCL|SHM_R|SHM_W);
     
      if (semid == -1 && errno == EEXIST){
	  printf("\n>Los semaforos ya existen\n");
          semid = semget(CLAVE_SEMAFORO, 10, SHM_R|SHM_W);
      }
      if (semid == -1) {
          perror("semget:");
          exit(errno);
      }

      for(i=1;i<=NUM_SEMAF;i++){
	arg.array[i-1]=i;
	
      }
      /*Inicializa los semaforos*/
      if((semctl(semid,0,SETALL,arg))==-1){
		printf("Error al incializar el semáforo\n");
                /*Elimina los semaforos*/
      		semctl (semid, 0, IPC_RMID);
		exit(-1);
      }
	else
       	printf("Incialización del semáforo terminada\n");

      system("ipcs -s");

      /*Elimina los semaforos*/
      semctl (semid, 0, IPC_RMID);

      system("ipcs -s");

exit(0);
}
