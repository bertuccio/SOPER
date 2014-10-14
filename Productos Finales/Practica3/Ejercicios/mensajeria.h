#ifndef _MENSAJERIA
#define _MENSAJERIA
/******************************************************************************

Nombre del fichero: mensajeria.h

******************************************************************************/
/*=== Cabeceras==============================================================*/
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include"sudoku.h"

/*=== Definiciones===========================================================*/

/*=== Funciones==============================================================*/
int sendPeticionJugador(int queue, PeticionJugador *peticion);
int recievePeticionJugador(int queue, PeticionJugador *peticion);

int sendTurnoJugador(int queue, Jugador jugador);
int recieveTurnoJugador(int queue, Jugador jugador);

int sendTurnoAgotadoJugador(int queue, Jugador jugador);
int sendTurnoPerdidoJugador(int queue,Jugador jugador);

int recieveFinTurnoJugador(int queue,Turno *turno, Jugador jugador);

int sendPeticionJugada(int queue, PeticionJugada *peticion);
int recievePeticionJugada(int queue, PeticionJugada *peticion);


int sendOrdenador(int queue, Ordenador *computer);
int recieveOrdenador(int queue, Ordenador *computer);

int sendJugadaOrdenador(int queue);
int recieveJugadaOrdenador(int queue);

int sendBloqueCompleto(int queue);
int recieveBloqueCompleto(int queue);

/*===========================================================================*/
#endif
