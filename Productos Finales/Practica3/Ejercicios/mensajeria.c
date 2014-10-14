/******************************************************************************

Nombre del módulo: mensajeria.c

Descripción: Este archivo contiene las funciones de mensajeria.h

******************************************************************************/

/*=== Cabeceras =============================================================*/
#include"mensajeria.h"
#include"sudoku.h"
/*=== Funciones==============================================================*/

/******************************************************************************

Nombre de la función: sendPeticionJugador

Descripción: Envia una peticion de jugador

Devuelve: -1 si ha habido algun error


******************************************************************************/
int sendPeticionJugador(int queue, PeticionJugador *peticion){
	int longitud=sizeof(PeticionJugador) - sizeof(peticion->tipo);

	return msgsnd(queue,peticion,longitud,0);
}
/******************************************************************************

Nombre de la función: recievePeticionJugador

Descripción: Recibe una peticion de jugador

Devuelve: -1 si ha habido algun error


******************************************************************************/
int recievePeticionJugador(int queue, PeticionJugador *peticion){

	int longitud=sizeof(PeticionJugador) - sizeof(peticion->tipo);

	return msgrcv(queue,peticion,longitud,PETICION_JUGADOR,0);
}
/******************************************************************************

Nombre de la función: sendTurnoJugador

Descripción: Envia el turno de un jugador

Devuelve: -1 si ha habido algun error


******************************************************************************/
int sendTurnoJugador(int queue, Jugador jugador){
	long id;
	id=jugador.id;
	return msgsnd(queue,&id,sizeof(long),0);
}
/******************************************************************************

Nombre de la función: recieveTurnoJugador

Descripción: Recibe el turno de un jugador

Devuelve: -1 si ha habido algun error


******************************************************************************/
int recieveTurnoJugador(int queue, Jugador jugador){

	long idRecibida;
	return msgrcv(queue,&idRecibida,sizeof(long),jugador.id,0);
}
/******************************************************************************

Nombre de la función: sendTurnoAgotadoJugador

Descripción: Envia un mensaje señalizando que el jugador a agotado su turno

Devuelve: -1 si ha habido algun error


******************************************************************************/
int sendTurnoAgotadoJugador(int queue, Jugador jugador){
	Turno turno;
	int longitud;
	
	longitud=sizeof(Turno)-sizeof(turno.tipo);
	turno.tipo=jugador.id*5;
	turno.turnoAgotado=TRUE;

	return msgsnd(queue,&turno,longitud,0);
}
/******************************************************************************

Nombre de la función: sendTurnoPerdidoJugador

Descripción: envia un mensaje diciendo que ha acabado el turno

Devuelve: -1 si ha habido algun error


******************************************************************************/
int sendTurnoPerdidoJugador(int queue,Jugador jugador){
	Turno turno;
	int longitud;
	
	longitud=sizeof(Turno)-sizeof(turno.tipo);
	turno.tipo=jugador.id*5;
	turno.turnoAgotado=FALSE;

	return msgsnd(queue,&turno,longitud,0);
}
/******************************************************************************

Nombre de la función: recieveFinTurnoJugador

Descripción: Recibe el mensaje de turno acabado

Devuelve: -1 si ha habido algun error


******************************************************************************/
int recieveFinTurnoJugador(int queue,Turno *turno,Jugador jugador){

	int longitud=sizeof(Turno)-sizeof(turno->tipo);	

	return msgrcv(queue,turno,longitud,jugador.id*5,IPC_NOWAIT);
}

/******************************************************************************

Nombre de la función: sendPeticionJugada

Descripción: envia una peticion de jugada

Devuelve: -1 si ha habido algun error


******************************************************************************/
int sendPeticionJugada(int queue, PeticionJugada *peticion){
	
	int longitud;
	
	longitud=sizeof(PeticionJugada)-sizeof(peticion->tipo);

	return msgsnd(queue,peticion,longitud,0);
}
/******************************************************************************

Nombre de la función: recievePeticionJugada

Descripción: recibe una peticion de jugada de parte de un jugador

Devuelve: -1 si ha habido algun error


******************************************************************************/
int recievePeticionJugada(int queue, PeticionJugada *peticion){
	
	int longitud;
	
	longitud=sizeof(PeticionJugada)-sizeof(peticion->tipo);

	return msgrcv(queue,peticion,longitud,PETICION_JUGADA,IPC_NOWAIT);
}
/******************************************************************************

Nombre de la función: sendJugadaOrdenador

Descripción: envia un mensaje para que el proceso Ordenador realice una jugada

Devuelve: -1 si ha habido algun error


******************************************************************************/
int sendJugadaOrdenador(int queue){
	long tipo=JUGADA_ORDENADOR;
	return msgsnd(queue,&tipo,sizeof(long),0);
}
/******************************************************************************

Nombre de la función: recieveJugadaOrdenador

Descripción: recibe un mensaje para que el proceso Ordenador realice una jugada

Devuelve: -1 si ha habido algun error


******************************************************************************/
int recieveJugadaOrdenador(int queue){

	long computer;

	return msgrcv(queue,&computer,sizeof(long),JUGADA_ORDENADOR,0);
}
/******************************************************************************

Nombre de la función: sendOrdenador

Descripción: envia un mensaje para que comience el proceso Ordenador

Devuelve: -1 si ha habido algun error


******************************************************************************/
int sendOrdenador(int queue, Ordenador *computer){
	int longitud=sizeof(Ordenador)-sizeof(long);

	return msgsnd(queue,computer,longitud,0);
}
/******************************************************************************

Nombre de la función: recieveOrdenador

Descripción: recibe el mensaje para que comience el proceso Ordenador

Devuelve: -1 si ha habido algun error


******************************************************************************/
int recieveOrdenador(int queue, Ordenador *computer){

	int longitud=sizeof(Ordenador)-sizeof(long);

	return msgrcv(queue,computer,longitud,ORDENADOR,0);
}

/******************************************************************************

Nombre de la función: sendBloqueCompleto

Descripción: envia un mensaje señalando que se ha completado un bloque

Devuelve: -1 si ha habido algun error


******************************************************************************/
int sendBloqueCompleto(int queue){

	long tipo=BLOQUE_COMPLETO;

	return msgsnd(queue,&tipo,sizeof(long),0);

}

/******************************************************************************

Nombre de la función: recieveBloqueCompleto

Descripción: recibe un mensaje de bloque completado

Devuelve: -1 si ha habido algun error


******************************************************************************/
int recieveBloqueCompleto(int queue){

	long bloque;

	return msgrcv(queue,&bloque,sizeof(long),BLOQUE_COMPLETO,IPC_NOWAIT);
}
