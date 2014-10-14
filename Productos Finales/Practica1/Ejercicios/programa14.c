// programa14.c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void (*signal (int sig, void (*func)(int))) (int);
void captura (int sennal)
{
	printf ("He conseguido capturar ALARMA\n");
	fflush (NULL);
	return;
}
int main (int argc, char *argv [], char *env [])
{
	if (signal (SIGALRM, captura) == SIG_ERR)
	{
		puts ("Error en la captura");
		exit (1);
	}

	alarm(3);
	printf("\nComienza la alarma.\n");
	pause();

	return 0;
}
