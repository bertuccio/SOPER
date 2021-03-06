// ej5
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdio.h>
#define CLAVE_MENSAJES 1555
#define SIZE 1000

typedef struct msgbuf {
	long type;
	char msg [SIZE];
} MSGBUF;


int main() {

int queue;
int result;
struct msgbuf buffer;
int i = 0;

queue = msgget(CLAVE_MENSAJES, IPC_CREAT|SHM_R|SHM_W);

if (queue == -1) {
	perror("msgget:");
	exit(-1);
}

while (1) {
	result = msgrcv (queue, &buffer,SIZE, 0, IPC_NOWAIT);
	if (result == -1) {
		perror("msgrcv:");
		exit(-1);
	}

	printf ("Mensaje %d dice: %s\n", i++, buffer.msg);
}

exit(0);
}
