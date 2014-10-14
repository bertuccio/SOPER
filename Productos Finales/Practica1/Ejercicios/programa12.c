// programa12.c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void (*signal (int sig, void (*func)(int))) (int);
void captura (int sennal)
{
	printf ("Capturada la señal %d\n", sennal);
	fflush (NULL);
	return;
}

int main (int argc, char *argv [], char *env [])
{
	int id;
	switch(id=fork())
	{
		case -1: printf("Error al crear el hijo\n");
			 exit(-1);
		case 0:  if (signal (SIGUSR1, captura) == SIG_ERR)
			 {
				puts ("Error en la captura");
				exit (1);
			 }
			 while(1);
		default: kill(id,SIGUSR1);

	}

	exit (0);
}
