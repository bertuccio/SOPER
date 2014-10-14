// programa16.c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void (*signal (int sig, void (*func)(int))) (int);
void captura (int sennal)
{
	if (sennal == SIGUSR1) {
		printf("Esperando a las señal SIGUSR1\n");
		while(1);
	}
	printf ("He conseguido capturar la señal %d\n",sennal);
	return;
}
int main (int argc, char *argv [], char *env [])
{
	int i;

	printf("PID= %d\n",getpid());

	for(i=1;i<=64;i++) {
			if (signal (i, captura) == SIG_ERR){
				printf("Error en la captura de la señal %d\n",i);
				continue;
			}
	}
	while(1);
	exit(0);
}

