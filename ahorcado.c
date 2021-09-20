#include "ahorcado.h"
#include <string.h>


void ahorcadoInicializar(Ahorcado* ahorcado, char* palabra, int numIntentos){
	if(ahorcado == NULL)
		return;
	ahorcado->numIntentos = numIntentos;
	strncpy(ahorcado->palabraAAdivinar, palabra, strlen(palabra)*sizeof(char)+1);
	for(int i = 0; i < strlen(ahorcado->palabraAAdivinar); i++)
		ahorcado->restante[i] = '_';
	ahorcado->restante[strlen(ahorcado->palabraAAdivinar)]='\0';
}


void ahorcadoJugar(Ahorcado* ahorcado){
	printf("Tenemos la palabra '%s' con la cantidad de intentos %i\n", ahorcado->palabraAAdivinar, ahorcado->numIntentos);
	printf("Palabra por adivinar: %s\n", ahorcado->restante);
}