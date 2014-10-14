//programa18.c

#include"utilidades.h"

/* Declaracion de variables globales */

int flagTiempo;
struct timeval comienzo;
struct timeval TiempoCodificador;
struct timeval TiempoCoutPalb;
struct timeval TiempoCountChar;
struct timeval  TiempoCoutCharnoCod;

int miPipe[2];

/* Bloque de manejadores de señal */
void (*signal (int sig, void (*func)(int))) (int);

void expira(int sennal) {
	if (flagTiempo==0){
		printf("\n\nTiempo agotado para introducir los datos (60s)\n\n");
		fflush (NULL);
		exit(0);
	}
}

void termina(int sennal){

	close(miPipe[0]);
	close(miPipe[1]); 
	printf("\n\nTerminado\n\n");
	exit(0);	
}

void finalTiempoCodificador(int sennal) {

	gettimeofday(&TiempoCodificador,NULL);
	fflush (NULL);
	return;
}
void finalTiempoCoutPalb(int sennal) {
	
	gettimeofday(&TiempoCoutPalb,NULL);
	fflush (NULL);
	return;
}
void finalTiempoCountChar(int sennal) {

	gettimeofday(&TiempoCountChar,NULL);
	fflush (NULL);
	return;
}
void finalTiempoCoutCharnoCod(int sennal) {

	gettimeofday(&TiempoCoutCharnoCod,NULL);
	fflush (NULL);
	return;
}

void imprimeTiempos(int sennal) {
	
	fflush (NULL);
	return;
}

int main (int argc, char *argv [], char *env []) {

	char fraseOriginal[MAX_LINE];
	char claveCifrado[MAX_LINE];
	char readbuffer[MAX_LINE];
	char *noEncriptados=NULL;
	char *Cifrado=NULL;

	pid_t PIDcronometro;
	pid_t PIDcifrador;
	pid_t PIDcontPalabras;
	pid_t PIDcontCaracteres;
	pid_t PIDnoCifrados;

	sigset_t maskImprime;

	int estadoaux;
	int numPalabras, numCaracteres;;
	int estadoPIDcontPalabras;
	int estadoPIDcontCaracteres;	

	signal(SIGALRM, expira);
	signal(SIGINT, termina);

	/* 
	 * Captura de la clave de cifrado.
	 * Se repetira hasta que se introduzca una clave correcta
	 */
	do{
		alarm(60);
		flagTiempo=0;	

		printf("Clave de cifrado: ");
		fgets(claveCifrado, MAX_LINE,stdin);

		alarm(0);
		flagTiempo=1;

		claveCifrado[strlen(claveCifrado)-1]='\0';

	}while(charvalidos(claveCifrado)==-1);
		
	/*
	 * Inicialización de la mascara del sigsuspend() del cronometro
	 * y del pipe que utilizaran los procesos que lo necesiten.
	 */
	sigfillset(&maskImprime);
	sigdelset(&maskImprime,SIGBUS);

	pipe(miPipe);

	do{
		/*
		 * Captura de la frase a codificar.
		 * Se añade '\0' al final para su manipulación posterior.
		 */
		alarm(60);
		flagTiempo=0;

		printf("Frase a codificar: ");
		fgets(fraseOriginal, MAX_LINE,stdin);
		printf("\n\n");

		flagTiempo=1;
	
		fraseOriginal[strlen(fraseOriginal)-1]='\0';
		alarm(0);

		
		PIDcronometro = fork();

		if(PIDcronometro==-1){
			printf("Error al crear el proceso Cronometro\n");
			exit(-1);
		}

		if(PIDcronometro==0){
		/**********************************************************************
		**************************PROCESO CRONOMETRO***************************
		**********************************************************************/
			
			gettimeofday(&comienzo,NULL); 
			/* El inicio de todos los procesos es practicamente
			* en el mismo instante de tiempo */

			signal(SIGUSR1,finalTiempoCodificador);
			signal(SIGUSR2,finalTiempoCoutPalb);
			signal(SIGFPE,finalTiempoCountChar);
			signal(SIGXFSZ,finalTiempoCoutCharnoCod);
			signal(SIGBUS, imprimeTiempos);

			/* Espera a que el proceso padre le de la señal de imprimir (SIGBUS) */
			sigsuspend(&maskImprime);

			printf("-Y por último los tiempos de cada proceso en el mismo\n");
			printf("orden que las impresiones anteriores: %lf, %lf, %lf, %lf \n\n\n",
			(double)DIFFTIME(TiempoCodificador,comienzo)*0.000001,
			(double)DIFFTIME(TiempoCoutPalb,comienzo)*0.000001,
			(double)DIFFTIME(TiempoCountChar,comienzo)*0.000001,
			(double)DIFFTIME(TiempoCoutCharnoCod,comienzo)*0.000001);

			exit(0);
			
		}

		if(PIDcronometro>0){
			
			PIDcifrador = fork();

			if(PIDcifrador==-1){
				printf("Error al crear el proceso Codificador\n");
				exit(-1);
			}

			if(PIDcifrador==0){
			
			/**********************************************************************
			**************************PROCESO CODIFICADOR**************************
			**********************************************************************/
						
				Cifrado=vignere(claveCifrado,fraseOriginal);
				printf("Codificado: %s\n",Cifrado);

				free(Cifrado);			
				kill(PIDcronometro, SIGUSR1);
				exit(0);
			
			}
			if(PIDcifrador>0){

				PIDcontPalabras = fork();

				if(PIDcontPalabras==-1){
					printf("Error al crear el proceso Contador de Palabras\n");
					exit(-1);
				}

				if(PIDcontPalabras==0){
					/**********************************************************************
					**************************PROCESO CONT PALABRAS************************
					**********************************************************************/

					 /* Comienza el proceso */
					
					numPalabras=cuentapalab(fraseOriginal);
					
					kill(PIDcronometro, SIGUSR2); /* Proceso finalizado */
					
					exit(numPalabras);
				}
				if(PIDcontPalabras>0){

					PIDcontCaracteres= fork();

					if(PIDcontCaracteres==-1){
						printf("Error al crear el proceso Contador de Caracteres\n");
						exit(-1);
					}

					if(PIDcontCaracteres==0){
						/**********************************************************************
						**************************PROCESO CONT CARACTER************************
						**********************************************************************/
						/* Comienza el proceso */
						
						numCaracteres=cuentachar(fraseOriginal);
						
						kill(PIDcronometro,SIGFPE);
			
						exit(numCaracteres);
					}

					if(PIDcontCaracteres>0){

						PIDnoCifrados= fork();

						if(PIDnoCifrados==-1){
							printf("Error al crear el proceso Caracteres no Cifrados\n");
							exit(-1);
						}

						if(PIDnoCifrados==0){
						/**********************************************************************
						**********************PROCESO CARACTER NOCIFRADOS**********************
						**********************************************************************/
						
						noEncriptados=noencripchar(fraseOriginal);
						
						/* Escribe en el pipe el resultado de la operacion */
						write(miPipe[1],noEncriptados,strlen(noEncriptados)+1);					
						free(noEncriptados);
						kill(PIDcronometro, SIGXFSZ);
						exit(0);
						}
					}
				}
			}
			
			/* 
			 * El padre espera a la finalización de todos sus hijos
			 * de forma ordenada.
			 */
			waitpid(PIDcifrador,&estadoaux,0);
			waitpid(PIDcontPalabras,&estadoPIDcontPalabras,0);

			if (WIFEXITED(estadoPIDcontPalabras)!=0)/*Ha terminado con exit()*/
				numPalabras=WEXITSTATUS(estadoPIDcontPalabras);
			
			waitpid(PIDcontCaracteres,&estadoPIDcontCaracteres,0);

			if (WIFEXITED(estadoPIDcontCaracteres)!=0)/*Ha terminado con exit()*/
				numCaracteres=WEXITSTATUS(estadoPIDcontCaracteres);
			
			waitpid(PIDnoCifrados,&estadoaux,0);

			/* Lee el resultado del proceso NoCifrados del pipe */ 			
			read(miPipe[0], readbuffer, sizeof(readbuffer));
			
			/* Impresión por pantalla de los resultados */
			printf("-Después el conteo de palabras: El numero de palabras es %d\n", numPalabras);
			printf("-Luego el número de letras: El numero de caracteres es %d\n", numCaracteres);
			printf("-A continuación cuantos caracteres no se han encriptado: %s \n",readbuffer);
			
			/* Por ultimo, envia la señal de impresion al cronometro */
			kill(PIDcronometro,SIGBUS);
			waitpid(PIDcronometro,&estadoaux,0);
		}
	}while(1);

exit(0);					
}
