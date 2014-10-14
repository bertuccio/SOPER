/******************************************************************************

Nombre de la función: down

Descripción: "baja" el semoforo y no permite que otros procesos accedan a memoria
	     compartida.

Devuelve: El numero de caracteres validos


******************************************************************************/

int down(int id, int num_sem){

	int resultado;

	struct sembuf operdown = {0, -1, 0};

	operdown.sem_num=num_sem;

	do{
		resultado=semop(id, &operdown, 1);
	}while(errno==EINTR&&resultado==-1);
	
	

return resultado;	

}
