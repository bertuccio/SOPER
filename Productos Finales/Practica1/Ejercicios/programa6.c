// programa6.c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <wait.h>

int main (int argc, char *argv [], char *env []){
	int estadoChild;
	int id;
	
	switch(id =fork()){
		case -1:printf("Fallo al crear el hijo\n");
			exit(-1);
		case 0:	printf("Soy hijo con PID=%d con PPID=%d\n",getpid(),getppid());
			exit(5);
		default:wait(&estadoChild);
			if (WIFEXITED(estadoChild)!=0){ /*Ha terminado con exit()*/
				printf ("Yo soy su padre mi hijo ha devuelto: %d \n", WEXITSTATUS(estadoChild));
			}
			else
				printf("Yo soy su padre y mi hijo a muerto\n");

			break;
	}

	exit(0);
}
