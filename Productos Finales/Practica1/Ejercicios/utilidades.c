/******************************************************************************

Nombre del módulo: utilidades.c

Descripción: Este archivo contiene las funciones de utilidades.h

******************************************************************************/
/*=== Cabeceras =============================================================*/
#include"utilidades.h"
/****************************************************************************/

/******************************************************************************

Nombre de la función: cuentachar

Descripción: Cuenta los caracteres validos

Devuelve: El numero de caracteres validos


******************************************************************************/
int cuentachar(char *cadena){
	int tamano;
	int i;
	char tmp;
	int numCaracteres=0;

	tamano = strlen(cadena);
						
	for(i=0;i<tamano;i++){
		tmp = cadena[i];
		if(((tmp>='A') && (tmp<='Z'))||
		((tmp>='a') && (tmp<='z'))||
		((tmp==-47) || (tmp==-15)))
		{
			numCaracteres++;
		}
	}

return numCaracteres;
}
/******************************************************************************

Nombre de la función: cuentapalab

Descripción: Cuenta el numero de palabras

Devuelve: El numero de palabras


******************************************************************************/
int cuentapalab(char *cadena){

	char *aux=NULL;
	char especial[] = " ";
	int numPalabras=0;

	if((aux = strtok(cadena,especial))!=NULL)
		numPalabras=1;
	while((aux = strtok( NULL,especial))!=NULL)
		numPalabras++;

return numPalabras;
}
/******************************************************************************

Nombre de la función: convierteatablavignere

Descripción: Convierte de ascii a la tabla de vignere de letras

Devuelve: Devuelve el numero correspondiente de un caracter ascii a su 
          equivalente en la tabla vignere


******************************************************************************/
char convierteatablavignere(char caracter){

    if(caracter>='A'&&caracter<='N')
        return caracter-'A';

    else if (caracter>='O'&&caracter<='Z')
        return caracter-'A'+1;/* +1 por la enie*/

    else if (caracter>='a'&&caracter<='n')
        return caracter-'a'+27;

    else if (caracter>='o'&&caracter<='z')
        return caracter-'a'+28;

    else if (caracter==-47)
        return 14;

    else if (caracter==-15)
        return 41;

return -1;
}
/******************************************************************************

Nombre de la función: convertiraASCII

Descripción: Convierte el numero un caracter en la tabla vignere a ASCII

Devuelve: Devuelve el codigo ASCII equivalente a la tabla vignere


******************************************************************************/
char convertiraASCII(char carac){

    if(carac>=0 && carac<=13)/*A a la N*/
        return carac+'A';

    else if (carac>=15 && carac<=26) /*O a la Z*/
        return carac+'A'-1;

    else if (carac>=27 && carac<=40)/*a a la n*/
        return carac+'a'-27;

    else if (carac>=42 && carac<=53)/*o a la z*/
        return carac+'a'-28;

    else if (carac==14)/*Ñ*/
        return -47;

    else if (carac==41)/*ñ*/
        return -15;

return -1;
}
/******************************************************************************

Nombre de la función: noencripchar

Descripción: Genera un string con los caracteres no encriptados por vignere
 
Devuelve: Una cadena de caracteres no encriptados por vignere


******************************************************************************/
char *noencripchar(char *cadena){

	int tamano=0;
	int contador;
	int i;
	char tmp;
	char *noEncriptados;

	noEncriptados=(char *)malloc(sizeof(char)*MAX_LINE);

	tamano = strlen(cadena);
	contador=-1;

	for(i=0;i<tamano;i++){

		tmp = cadena[i];
		if(!(((tmp>='A') && (tmp<='Z'))||
		((tmp>='a') && (tmp<='z'))||
		((tmp==-15) || (tmp==-47))))
		{
			contador++;
			noEncriptados[contador]=tmp;
			noEncriptados[contador+1]='\0';
		}
	}

	return noEncriptados;

}


/******************************************************************************

Nombre de la función: vignere

Descripción: Busca una clave primaria devolviendo el offset correspondiente

Devuelve: El offset del indice encontrado


******************************************************************************/
char *vignere(char *claveCifrado,char *FraseOriginal){

    char *Cifrado=NULL;
    int i,j=0,tamano=strlen(FraseOriginal);
    char charclave,charfrase;
   
    Cifrado=(char *)malloc(sizeof(char)*MAX_LINE);

    for(i=0;i<tamano;i++){

        charclave=convierteatablavignere(claveCifrado[j%strlen(claveCifrado)]);
        charfrase=convierteatablavignere(FraseOriginal[i]);

        if(charfrase!=-1){/*Si es un caracter valido lo vuelve a convertir a ASCII*/
            Cifrado[i]=convertiraASCII((charclave+charfrase)%NUM_LETRAS);/*vignere*/
            j++;
        }
        else /*Si no es un caracter valido:*/
            Cifrado[i]=FraseOriginal[i];
    }
    Cifrado[strlen(FraseOriginal)]='\0';

  return Cifrado;
}
/******************************************************************************

Nombre de la función: charvalidos

Descripción: Recorre una cadena buscando caracteres no validos

Devuelve: 0 si la cadena no contiene ningun caracter no valido
          -1 si ha encontrado algun caracter no valido


******************************************************************************/

int charvalidos(char *cadena){

	int i;
	int tamano=strlen(cadena);

	for(i=0;i<tamano;i++){

		if(!(((cadena[i]>='A') && (cadena[i]<='Z'))||
		((cadena[i]>='a') && (cadena[i]<='z'))||
		((cadena[i]==-15) || (cadena[i]==-47)))){
			printf("\nError, caracter \"%c\" no válido, vuelva a introducir la clave:\n",cadena[i]);
			return -1;
		}
	}


return 0;
}
