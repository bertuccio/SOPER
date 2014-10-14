//programa17.c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int flag=0;

void (*signal (int sig, void (*func)(int))) (int);

void captura(int sennal) {
	flag=1;
	return;
}


int main (int argc, char *argv [], char *env []) {
	int cuenta=0;

	alarm(20);

	if (signal(SIGALRM,captura) == SIG_ERR){
		printf("Error en la captura.\n");
		exit(1);
	}
	while(flag==0){
		printf(" %d,",cuenta++);
	}
	printf("\nÉstos son los números que me ha dado tiempo a contar en 20 segundos.\n");

	exit(0);

}
