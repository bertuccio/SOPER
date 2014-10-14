// ej4
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define CLAVE_MENSAJES 1555
#define SIZE 1000
#define LOOP 9990000

typedef struct msgbuf {
	long type;
	char msg [SIZE];
} MSGBUF;

int main() {

int queue;
int result;
struct msgbuf buffer;
int i;

queue = msgget(CLAVE_MENSAJES, IPC_CREAT|SHM_R|SHM_W);

if (queue == -1) {
	perror("msgget:");
	exit(-1);
}

buffer.type = 1;
strncpy (buffer.msg, "Hola Mundo!", SIZE);

for (i= 0; i < LOOP; i++) {
	result = msgsnd (queue, &buffer, strlen(buffer.msg)+1, 0);
	if (result == -1) {
		perror("msgget:");
		exit(-1);
	}
}

exit(0);
}
