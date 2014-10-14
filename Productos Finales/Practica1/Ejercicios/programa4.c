// programa4 .c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char *argv [], char *env []){
	int i, a=1;

	for(i=0; i<10;i++){
		switch(fork()){
			case 0: a=3;
				printf("a=%d\n",a);
				break;
			case -1: printf("Error al crear el proceso hijo\n");
				 exit(0);
			default:printf("a=%d\n",a);
				break;
			
		}
	 }
exit(0);
}
