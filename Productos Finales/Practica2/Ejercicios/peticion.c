/******************************************************************************

Nombre del módulo: peticion.c

Descripción: Este archivo contiene las funciones de peticion.h

******************************************************************************/

/*=== Cabeceras =============================================================*/
#include"peticion.h"

/*=== Funciones==============================================================*/

/******************************************************************************

Nombre de la función: setPeticion

Descripción: Inicializa una estructura Peticion

Devuelve: Void


******************************************************************************/
void setPeticion(int id, char *texto,Peticion *peticion){
	peticion->id=id;
	strcpy(peticion->texto,texto);
}	

/******************************************************************************

Nombre de la función: getIdPeticion

Descripción: Devuelve el id de una peticion

Devuelve: El id de la peticion pasada como parametro


******************************************************************************/
int getIdPeticion(Peticion *peticion){
	return peticion->id;
}
/******************************************************************************

Nombre de la función: getTextoPeticion

Descripción: Devuelve el texto de una peticion

Devuelve: El texto de la peticion pasada como parametro


******************************************************************************/
char *getTextoPeticion(Peticion *peticion){
	return peticion->texto;
}
/******************************************************************************

Nombre de la función: tratamientoTexto

Descripción: Elimina los saltos de carro del texto leido del fichero

Devuelve: Void


******************************************************************************/
void tratamientoTexto(char *texto){

	if(texto[strlen(texto)-1]=='\n')
		texto[strlen(texto)-1]='\0';
		
}

/******************************************************************************

Nombre de la función: iniPeticion

Descripción: Devuelve una peticion inicializada con id=-1

Devuelve: Peticion


******************************************************************************/
Peticion iniPeticion(){
	Peticion peticion;
	
	peticion.id=-1;
	strcpy(peticion.texto,"\0");

	return peticion;

}
