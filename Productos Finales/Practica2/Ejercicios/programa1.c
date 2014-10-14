#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#define CLAVE_MEMORIA 2555

int main() {

      int shm;
      shm = shmget(CLAVE_MEMORIA, sizeof(int), IPC_CREAT | IPC_EXCL); /*EEXIST*/

      if (shm == -1) {
          perror("shmget:");
          exit(-1);
      }

      exit(0);
}
