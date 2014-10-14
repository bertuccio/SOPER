// programa11.c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void (*signal (int sig, void (*func)(int))) (int);

int main (int argc, char *argv [], char *env [])
{
	if (signal (SIGINT, SIG_IGN) == SIG_ERR)
	{
		puts ("Error en la captura");
		exit (1);
	}
	while (1);
	exit (0);
}
