// ej2
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CLAVE_MENSAJES 1555

typedef struct msgbuf {
	long type;
	char msg [20];
} MSGBUF;

int main() {

int queue;
int result;
struct msgbuf buffer;

queue = msgget(CLAVE_MENSAJES, IPC_CREAT|SHM_R|SHM_W);

if (queue == -1) {
	perror("msgget:");
	exit(-1);
}

buffer.type = 1;
strcpy (buffer.msg, "Hola Mundo!");
result = msgsnd (queue, &buffer, strlen(buffer.msg)+1, IPC_NOWAIT);

if (result == -1) {
	perror("msgget:");
	exit(-1);
}

exit(0);
}
