/* ej1 */
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

int queue;
queue = msgget( IPC_PRIVATE, IPC_CREAT| 0600);

if (queue == -1) {
	perror("msgget:");
	exit(-1);
}

exit(0);
}
