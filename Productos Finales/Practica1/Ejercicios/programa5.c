// programa5.c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <wait.h>

int main (int argc, char *argv [], char *env []){

	int i;
	int a = 1;
	int aux = 0;
	int estadoChild=0;

	for(i=0; i<10;i++){
		switch(aux=fork())
		{
		case 0:
			a=3;
			printf("A=%d\n",a);
			exit(a);
		
		case -1: 
			printf("Error al crear el proceso hijo\n");
			exit(-1);

		default:
			wait(&estadoChild);
			if(WIFEXITED(estadoChild)==0){ /*Ha terminado anormalmente*/
				printf("El proceso hijo a terminado anormalmente\n");
				exit(-1);
			}
			else{
				a=WEXITSTATUS(estadoChild);
				printf("a=%d\n",a);
			}
		}
		
	 }
	exit(a);
}
