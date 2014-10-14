// programa15.c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void (*signal (int sig, void (*func)(int))) (int);
void captura (int sennal)
{
	printf ("He conseguido capturar SIGALRM\n");
	fflush (NULL);
	return;
}
int main (int argc, char *argv [], char *env [])
{
	sigset_t mascara;

	if (signal (SIGALRM, captura) == SIG_ERR)
	{
		puts ("Error en la captura");
		exit (1);
	}

	alarm(3);	
	printf("\nComienza la alarma.\n");
	
	/* Rellenamos la mascara de señales para que espere únicamente a SIGALRM */
	sigfillset(&mascara);
	sigdelset(&mascara,SIGALRM);

	sigsuspend(&mascara);

	exit (0);
}
