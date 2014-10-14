/******************************************************************************

Nombre del módulo: semaforos.c

Descripción: Este archivo contiene las funciones de semaforos.h

******************************************************************************/
/*=== Cabeceras =============================================================*/
#include"semaforos.h"

/*=== Funciones==============================================================*/

/******************************************************************************

Nombre de la función: creaSemaforos

Descripción: Crea (o accede a) un grupo de semaforos mediante una key "key" y
             el número de semáforos que formara el grupo "numsem"

Devuelve: el id del conjunto de semáforos creado (o accedido)
          -1 si se ha producido un error


******************************************************************************/
int creaSemaforos(int key, int numsem){

        int semid;

	semid = semget(key, numsem, IPC_CREAT|IPC_EXCL|SHM_R|SHM_W);
	if (semid == -1 && errno == EEXIST){
		semid = semget(key, numsem, SHM_R|SHM_W);
	}

	return semid;
}
/******************************************************************************

Nombre de la función: down

Descripción: "baja" el semoforo y no permite que otros procesos accedan a memoria
	     compartida.

Devuelve: El numero de caracteres validos


******************************************************************************/
int down(int id, int num_sem){

	int resultado;

	struct sembuf operdown = {num_sem, -1, SEM_UNDO};


	do{
		resultado=semop(id, &operdown, 1);
	}while(errno==EINTR&&resultado==-1);
	
return resultado;	
}
/******************************************************************************

Nombre de la función: up

Descripción: "sube" el semoforo y deja "abierta" la memoria compartida para 
	     el acceso de otros procesos.

Devuelve: -1 si es ha producido algun error.


******************************************************************************/
int up(int id, int num_sem){

	struct sembuf operup = {num_sem, 1, SEM_UNDO};


return semop(id, &operup, 1);

}
/******************************************************************************

Nombre de la función: getValorSemaforo

Descripción: Devuelve el valor de un semáforo en concreto.

Devuelve: El valor del semoforo asociado a la id "semid" y a la posición 
          "numsem" dentro del grupo de semáforos.


******************************************************************************/
int getValorSemaforo(int semid, int numsem){

	return semctl(semid,numsem,GETVAL);
}
/******************************************************************************

Nombre de la función: setValorSemaforo

Descripción: Inicializa el semoforo asociado a la id "semid" y a la posición 
          "numsem" dentro del grupo de semáforos al valor "valor".

Devuelve: -1 si se ha producido algún error.
          0 si todo ha ocurrido correctamente


******************************************************************************/
int setValorSemaforo(int semid, int numsem, int valor){
	
	union semun carg;

	carg.val=valor;

	return semctl(semid,numsem,SETVAL,carg);
}
/******************************************************************************

Nombre de la función: borrarSemaforos

Descripción: Elimina el grupo de semaforos asociados a la id "semid" pasada como
             parámetro.

Devuelve: 0 si todo ha ido correctamente
          -1 si se ha producido un error


******************************************************************************/
int borrarSemaforos(int semid){
	return semctl(semid, 0, IPC_RMID);
}
/******************************************************************************

Nombre de la función: getProcesosEsperando

Descripción: Devuelve el numero de procesos que esperan a que se incremente
             el valor del semaforo

Devuelve: el numero de procesos que esperan a que se incremente el valor 
          de un semaforo
                                               

******************************************************************************/
int getProcesosEsperando(int semid, int numsem){
	return semctl(semid, numsem, GETNCNT);
}

