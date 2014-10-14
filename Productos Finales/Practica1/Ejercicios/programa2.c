//programa2

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	int i;

	printf("Raiz: PID=%d PPID=%d\n", getpid(),getppid());

	for(i=0;i<5;i++){
		switch(fork())
		{
			case 0:			
				printf("Proceso hijo(GEN %d): PID=%d PPID=%d\n",i+1, getpid(),getppid());
				break;
			case -1:
				printf("Error en la funcion fork()\n");
				break;
			default:
				printf("Proceso padre(GEN %d): PID=%d\n",i+1, getpid());
				break;
		}
	}

	exit(0);
}
