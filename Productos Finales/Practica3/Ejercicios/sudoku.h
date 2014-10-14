#ifndef _SUDOKU
#define _SUDOKU
/******************************************************************************

Nombre del fichero: sudoku.h

******************************************************************************/
/*=== Cabeceras==============================================================*/
#include<stdio.h>
#include<time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/time.h>
/*=== Definiciones===========================================================*/

#define MAX_LINE 256
#define MAX_JUGADORES 4

#define ORDENADOR 4
#define JUGADA_ORDENADOR 5
#define BLOQUE_COMPLETO 6

#define ESPACIOS 20

#define FIN_TURNO 27

#define SEM_MAX_JUGADORES 0
#define SEM_SUDOKU_MUTEX 1
#define SEM_JUGADOR_MUTEX 2

#define PETICION_JUGADOR 1
#define PETICION_JUGADA 99

#define CLAVE_JUGADORES 1555
#define CLAVE_MENSAJES 1556
#define CLAVE_SUDOKU 1557

#define MAX_TAM 9

#define TURNO_TIEMPO 25
#define TIEMPO_INICIO 5

typedef enum{
	TRUE=1,
	FALSE=0
}boolean;

typedef struct {
    int espacios;                
    char tablero[MAX_TAM][MAX_TAM];  
}Sudoku;


typedef struct{
	int jugadasCorrectas;
	char nombre[MAX_LINE];
	int id;
}Jugador;


typedef struct{
	long tipo;
	Jugador jugador;
}PeticionJugador;

typedef struct{
	int cordx;
	int cordy;
	int valor;
}Jugada;

typedef struct{
	long tipo;
	Jugada jugada;
}PeticionJugada;

typedef struct{
	long tipo;
	boolean turnoAgotado;
}Turno;

typedef struct{
	long tipo;
	int idOrdenador;
}Ordenador;

/*=== Funciones==============================================================*/
Jugador iniJugador();
void iniIdJugadores(int *idJugadores);
void setNombreJugador(Jugador *jugador, char *nombre);
char * getNombreJugador(Jugador *jugador);
void setIdJugador(Jugador *jugador, int id);
int getIdJugador(Jugador jugador);
int getPuntosJugador(Jugador jugador);
Jugador getJugadorPeticionJugador(PeticionJugador peticion);
void setJugadorPeticionJugador(Jugador jugador, PeticionJugador *peticion);

void ImprimeTablero(Sudoku sudoku);
void generaTablero(Sudoku *sudoku, int espacios);
int getNumEspaciosSudoku(Sudoku sudoku);

void setJugada(Jugada *jugada, int cordx, int cordy, int valor);
int getCordxJugada(Jugada jugada);
int getCordyJugada(Jugada jugada);
int getValorJugada(Jugada jugada);

void setJugadaPeticionJugada(PeticionJugada *peticion, Jugada jugada);
Jugada getJugadaPeticionJugada(PeticionJugada *peticion);

boolean turnoAgotado(Turno turno);

void setOrdenador(Ordenador *ordenador, int id);
int getOrdenador(Ordenador ordenador);
void jugadaOrdenador(Sudoku *sudoku);

boolean estaBloqueCompleto(Sudoku sudoku, int inicialx, int inicialy, int finalx, int finaly);
boolean realizaJugada(Sudoku *sudoku, Jugada jugada);

void finDePartida(int queue, Jugador *jugadores, int numJugadores);
/*===========================================================================*/
#endif
