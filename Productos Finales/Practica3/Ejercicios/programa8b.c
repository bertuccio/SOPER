// ej8b

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CLAVE_MENSAJES 1555
#define SIZE 15

typedef struct msgbuf {
	long type;
	char msg [SIZE];
} MSGBUF;

int main(int argc, char** argv) {

int queue;
int result;
struct msgbuf buffer;
buffer.msg[SIZE]=0;/*Aseguramos un /0 final de cadena en todos los casos*/
int i = 0;
int type;

if (argc <= 1)
	type = 1;

else
	type = atoi (argv [1]);

queue = msgget(CLAVE_MENSAJES, IPC_CREAT|SHM_R|SHM_W);

if (queue == -1) {
	perror("msgget:");
	exit(-1);
}
while (1) {
	result = msgrcv (queue, &buffer, SIZE, type, MSG_NOERROR);
	if (result == -1) {
		perror("msgrcv:");
		exit(-1);
	}
	printf ("Mensaje %d dice: %s\n", i++, buffer.msg);
}

exit(0);
}
