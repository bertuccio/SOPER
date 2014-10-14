#ifndef _UTILIDADES
#define _UTILIDADES
/******************************************************************************

Nombre del fichero: utilidades.h

******************************************************************************/
/*=== Cabeceras==============================================================*/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/time.h>
/*=== Definiciones===========================================================*/
#define MAX_LINE 256 
#define NUM_LETRAS 54

#define DIFFTIME(final, comienzo) ((final.tv_sec*1000000+final.tv_usec)-(comienzo.tv_sec*1000000+comienzo.tv_usec))

/*=== Funciones==============================================================*/

int cuentachar(char *cadena);
int cuentapalab(char *cadena);
char *vignere(char *clave,char *texto);
char convierteatablavignere(char caracter);
char convertiraASCII(char carac);
char *noencripchar(char *cadena);
int charvalidos(char *cadena);



#endif


