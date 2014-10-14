/******************************************************************************

Nombre del módulo: sudoku.c

Descripción: Este archivo contiene las funciones de sudoku.h

******************************************************************************/

/*=== Cabeceras =============================================================*/

#include"sudoku.h"

/*=== Funciones==============================================================*/

/******************************************************************************

Nombre de la función: setNombreJugador

Descripción: Introduce el nombre a una estructura Jugador

Devuelve: Void


******************************************************************************/
void setNombreJugador(Jugador *jugador,char *nombre){
	strcpy(jugador->nombre,nombre);
}	

/******************************************************************************

Nombre de la función: setPidJugador

Descripción: Introduce el pid de Jugador

Devuelve: Void


******************************************************************************/
void setIdJugador(Jugador *jugador, int id){
	jugador->id=id;
}
/******************************************************************************

Nombre de la función: getNombreJugador

Descripción: Devuelve el nombre de un jugador

Devuelve: El nombre del jugador


******************************************************************************/
char * getNombreJugador(Jugador *jugador){
	return jugador->nombre;
}
/******************************************************************************

Nombre de la función: getPidJugador

Descripción: Devuelve el pid de un jugador

Devuelve: el pid de un jugador


******************************************************************************/
int getIdJugador(Jugador jugador){
	return jugador.id;	
}
/******************************************************************************

Nombre de la función: getPuntosJugador

Descripción: Devuelve el numero de jugadas correctas de un jugador

Devuelve: numero de jugadas correctas


******************************************************************************/
int getPuntosJugador(Jugador jugador){
	return jugador.jugadasCorrectas;
}
/******************************************************************************

Nombre de la función: iniJugador

Descripción: Devuelve un jugador inicializado con pid=-1

Devuelve: Jugador inicializado a -1


******************************************************************************/
Jugador iniJugador(){
	Jugador jugador;
	
	jugador.id=-1;
	strcpy(jugador.nombre,"\0");
	jugador.jugadasCorrectas=0;

	return jugador;

}
/******************************************************************************

Nombre de la función: getJugadorPeticionJugador

Descripción: Devuelve el jugador de una peticion

Devuelve: Jugador


******************************************************************************/
Jugador getJugadorPeticionJugador(PeticionJugador peticion){
	return peticion.jugador;
}

/******************************************************************************

Nombre de la función: setJugadorPeticionJugador

Descripción: Introduce el jugador en la peticion de jugador

Devuelve: vacio


******************************************************************************/
void setJugadorPeticionJugador(Jugador jugador, PeticionJugador *peticion){
	peticion->jugador=jugador;
	peticion->tipo=PETICION_JUGADOR;
}

/******************************************************************************

Nombre de la función: iniPidJugadores

Descripción: Inicializa el array de pids a -1

Devuelve: vacio


******************************************************************************/
void iniIdJugadores(int idJugadores[MAX_JUGADORES]){
	int i;

	for(i=0;i<MAX_JUGADORES;i++)
		idJugadores[i]=-1;
}
/******************************************************************************

Nombre de la función: ImprimeTablero

Descripción: Imprime por pantalla el tablero del sudoku

Devuelve: vacio


******************************************************************************/
void ImprimeTablero(Sudoku sudoku){

	int i, j;

	/*Dibuja la referencia numerica de las columnas*/
	printf("   ");
	for(i=0;i<MAX_TAM;i++)
		printf("%d  ",i);
	printf("\n");
	/*Imprime el tablero junto con la referencia de las filas*/
	for(i=0;i<MAX_TAM;i++){
		printf("%d ",i);
		for(j=0;j<MAX_TAM;j++){
			if(sudoku.tablero[i][j]==0)
				printf("[ ]");
			else
				printf("[%d]",sudoku.tablero[i][j]);
			if(j==MAX_TAM-1)
				printf("\n");
		}

	}
}
/******************************************************************************

Nombre de la función: setJugada

Descripción: Introduce los atributos de una jugada

Devuelve: vacio


******************************************************************************/
void setJugada(Jugada *jugada, int cordx, int cordy, int valor){
	jugada->cordx=cordx;
	jugada->cordy=cordy;
	jugada->valor=valor;
}
/******************************************************************************

Nombre de la función: getCordxJugada

Descripción: Obtiene la cordx de una Jugada

Devuelve: vacio


******************************************************************************/
int getCordxJugada(Jugada jugada){
	return jugada.cordx;
}
/******************************************************************************

Nombre de la función: getCordyJugada

Descripción:   Obtiene la cordy de una Jugada

Devuelve: vacio


******************************************************************************/
int getCordyJugada(Jugada jugada){
	return jugada.cordy;
}
/******************************************************************************

Nombre de la función: getValorJugada

Descripción: Obtiene el valor de una Jugada

Devuelve: vacio


******************************************************************************/
int getValorJugada(Jugada jugada){
	return jugada.valor;
}

/******************************************************************************

Nombre de la función: setJugadaPeticionJugada

Descripción: Introduce una Jugada en una Peticion

Devuelve: vacio


******************************************************************************/
void setJugadaPeticionJugada(PeticionJugada *peticion, Jugada jugada){
	peticion->tipo=PETICION_JUGADA;
	peticion->jugada=jugada;
	
}
/******************************************************************************

Nombre de la función: getJugadaPeticionJugada

Descripción: Obtiene una jugada de una Peticion

Devuelve: vacio


******************************************************************************/
Jugada getJugadaPeticionJugada(PeticionJugada *peticion){
	return peticion->jugada;
}
/******************************************************************************

Nombre de la función: turnoAgotado

Descripción: comprueba si el turno ha terminado o se ha agotado

Devuelve: vacio


******************************************************************************/
boolean turnoAgotado(Turno turno){
	if(turno.turnoAgotado==TRUE)
		return TRUE;
	return FALSE;
}
/******************************************************************************

Nombre de la función: generaTablero

Descripción: genera el tablero

Devuelve: vacio


******************************************************************************/
void generaTablero(Sudoku *sudoku, int espacios){

	int i,j;	
	int cordx,cordy;

	sudoku->espacios=espacios;

	srand(time(NULL));

	for(i=0;i<MAX_TAM;i++){
		for(j=0;j<MAX_TAM;j++)
			sudoku->tablero[i][j]=rand() % MAX_TAM+1;
	}
	for(i=0;i<espacios;i++){
		/*Si ya habia un 0 en esa posicion lo embucla*/
		do{
			cordx=rand() % MAX_TAM;
			cordy=rand() % MAX_TAM;
		}while(sudoku->tablero[cordx][cordy]==0);
		sudoku->tablero[cordx][cordy]=0;
	}
		
	
}
/******************************************************************************

Nombre de la función: getNumEspaciosSudoku

Descripción: obtiene el numero de espacios vacios del sudoku

Devuelve: numero de espacios


******************************************************************************/
int getNumEspaciosSudoku(Sudoku sudoku){
	return sudoku.espacios;
}

/******************************************************************************

Nombre de la función: jugadaOrdeandor

Descripción: obtiene el numero de espacios vacios del sudoku

Devuelve: numero de espacios


******************************************************************************/
void jugadaOrdenador(Sudoku *sudoku){
	int i,j;

	for(i=0;i<MAX_TAM;i++){
		for(j=0;j<MAX_TAM;j++){
			if(sudoku->tablero[i][j]==0){
				sudoku->tablero[i][j]=rand() % MAX_TAM+1;
				return;
			}
		}
	}
}	
/******************************************************************************

Nombre de la función: setOrdenador

Descripción: obtiene el numero de espacios vacios del sudoku

Devuelve: numero de espacios


******************************************************************************/
void setOrdenador(Ordenador *ordenador,int id){
	ordenador->tipo=ORDENADOR;
	ordenador->idOrdenador=id;
}
/******************************************************************************

Nombre de la función: getOrdenador

Descripción: obtiene el numero de espacios vacios del sudoku

Devuelve: numero de espacios


******************************************************************************/
int getOrdenador(Ordenador ordenador){
	return ordenador.idOrdenador;
}
/******************************************************************************

Nombre de la función: estaBloqueCompleto

Descripción: comprueba si un determinado bloque está completo o no

Devuelve: TRUE o FALSE


******************************************************************************/
boolean estaBloqueCompleto(Sudoku sudoku, int inicialx, int inicialy, int finalx, int finaly){
	int contador=0;
	int i,j;
	for(i=inicialx;i<=finalx;i++){
		for(j=inicialy;j<=finaly;j++){
			
			if(sudoku.tablero[i][j]==0){	
				contador++;
			}
		}
	}
	if(contador==0)
		return TRUE;

	return FALSE;
}
/******************************************************************************

Nombre de la función: realizaJugada

Descripción: comprueba si la jugada es correcta y en caso de serlo la realiza

Devuelve: TRUE o FALSE


******************************************************************************/
boolean realizaJugada(Sudoku *sudoku, Jugada jugada){

	if(sudoku->tablero[jugada.cordx][jugada.cordy]==0){
		sudoku->tablero[jugada.cordx][jugada.cordy]=jugada.valor;
		return TRUE;
	
	}
	
	return FALSE;
}

/******************************************************************************

Nombre de la función: finDePartida

Descripción: manda la señal correspondiente de final de partida a los jugadores

Devuelve: void


******************************************************************************/
void finDePartida(int queue, Jugador *jugadores, int numJugadores){
	int i;
	int puntuacion=-1;
	int idGanador;

	/*Obtiene el id del jugador con máxima puntuacion*/
	for(i=0;i<numJugadores;i++){
		if(jugadores[i].jugadasCorrectas>puntuacion){
			puntuacion=jugadores[i].jugadasCorrectas;
			idGanador=jugadores[i].id;
		}
	}
	/*Envia las señales correspodientes a ganador/perdedores*/
	for(i=0;i<numJugadores;i++){
		if(idGanador==jugadores[i].id)
			kill(idGanador,SIGUSR2);
	
		else
			kill(jugadores[i].id,SIGBUS);
	}
}
