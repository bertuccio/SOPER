#ifndef _SEMAFOROS
#define _SEMAFOROS
/******************************************************************************

Nombre del fichero: semaforos.h

******************************************************************************/
/*=== Cabeceras==============================================================*/
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
/*=== Definiciones===========================================================*/

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

#define CLAVE_SEMAFORO 2000
#define CLAVE_MEMORIA 1555
#define NUM_SEMAFOROS 1
#define SEM_MUTEX 0
/*=== Funciones==============================================================*/
int creaSemaforos(int key, int numsem);
int borrarSemaforos(int semid);
int up(int id, int num_sem);
int down(int id, int num_sem);
int getValorSemaforo(int semid, int numsem);
int setValorSemaforo(int semid, int numsem, int valor);
int getProcesosEsperando(int semid, int numsem);
#endif
