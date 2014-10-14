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
