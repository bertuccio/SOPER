// programa13.c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void (*signal (int sig, void (*func)(int))) (int);
void captura (int sennal)
{
	printf ("He conseguido capturar SIGKILL\n");
	fflush (NULL);
	return;
}
int main (int argc, char *argv [], char *env [])
{
	if (signal (SIGKILL, captura) == SIG_ERR)
	{
		puts ("Error en la captura");
		exit (1);
	}
	while (1);
	exit (0);
}
