// ej8
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
#define LOOP 5

typedef struct msgbuf {
	long type;
	char msg [SIZE];
} MSGBUF;

int main(int argc, char** argv) {

int queue;
int result;
struct msgbuf buffer;
buffer.msg[SIZE]=0;/*Aseguramos un /0 final de cadena en todos los casos*/
int i;
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

buffer.type = type;

for (i= 0; i<LOOP; i++) {

	snprintf (buffer.msg, SIZE, "Hola Mundo! Soy emisor %d mensaje %d", type, i);
	result = msgsnd (queue, &buffer, strlen(buffer.msg)+1, 0);

	if (result == -1) {
		perror("msgsnd:");
		exit(-1);
	}

}

exit(0);
}
