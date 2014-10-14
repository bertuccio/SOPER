// programa8.c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <wait.h>

#define MAX_LINE 20

int main (int argc, char *argv [], char *env []){
	char * cadena;	
	int estadoChild;
	int id;

	cadena = malloc(MAX_LINE*sizeof(int));
	switch(id =fork()){
		case -1:printf("Fallo al crear el hijo\n");
			exit(-1);
		case 0:	printf("Introduzca texto: \n");
			fgets(cadena,MAX_LINE,stdin);
			exit(0);
		default:wait(&estadoChild);
			if(WIFEXITED(estadoChild)==0){ /*Ha terminado anormalmente*/
				printf("El proceso hijo a terminado anormalmente\n");
				exit(-1);
			}
			else
				printf("El contenido de la cadena es %s \n",cadena);
	}
	free(cadena);
	exit(0);
}
