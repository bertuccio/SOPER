#ifndef _PETICION
#define _PETICION
/******************************************************************************

Nombre del fichero: peticion.h

******************************************************************************/
/*=== Cabeceras==============================================================*/
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

#define MAX_LINE_TEXTO 101 /*1OO char + \0*/
#define MAX_LINE_FILE 2000

#define CLAVE_ZONA1 1555
#define CLAVE_ZONA2 1556

#define SEM_ZONA1_MUTEX 0
#define SEM_ZONA2_MUTEX 1
#define SEM_ZONA1_NPETICIONES 2
#define SEM_ZONA2_NPETICIONES 3
#define SEM_MAX_PETICIONES 4

typedef struct{
	int id;
	char texto[MAX_LINE_TEXTO];
} Peticion;
	

/*=== Funciones==============================================================*/
void setPeticion(int id, char *texto,Peticion *peticion);
void tratamientoTexto(char *texto);
int getIdPeticion(Peticion *peticion);
char *getTextoPeticion(Peticion *peticion);
Peticion iniPeticion();
#endif
