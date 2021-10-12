#include "server_ahorcado.h"
#include <string.h>
#include <stdbool.h>

#define SIN_NOVEDADES 0
#define DERROTA 2
#define VICTORIA 1
#define ERROR -10

int ahorcadoInicializar(Ahorcado* ahorcado, char* palabra, int numIntentos) {
	if (!ahorcado)
		return ERROR;
	ahorcado->numIntentos = numIntentos;
	snprintf(ahorcado->palabraAAdivinar, MAX_PALABRA, 
			"%s",palabra);
	ahorcado->palabraAAdivinar[strlen(palabra)-1] = '\0';
	for (int i = 0; i < strlen(ahorcado->palabraAAdivinar); i++)
		ahorcado->restante[i] = '_';
	ahorcado->restante[strlen(ahorcado->palabraAAdivinar)]='\0';
	return strlen(ahorcado->palabraAAdivinar);
}

void jugarYValidarLetra(Ahorcado* ahorcado, char letra) {
	bool letraIncorrecta = true;
	int len = strlen(ahorcado->palabraAAdivinar);
	for (int i = 0; i < len; i++){
		if (ahorcado->palabraAAdivinar[i] == letra){
			ahorcado->restante[i] = letra;
			letraIncorrecta = false;
		}
	}
	if (letraIncorrecta){
		ahorcado->numIntentos--;
	}
}

int letrasRestantes(Ahorcado* ahorcado) {
	int len = strlen(ahorcado->palabraAAdivinar);
	int contador = 0;
	for (int i = 0; i < len; i++){
		if(ahorcado->restante[i] == '_')
			contador++;
	}
	return contador;
}

int estadoDePartida(Ahorcado* ahorcado) {
	if (ahorcado->numIntentos == 0){
		snprintf(ahorcado->restante, strlen(ahorcado->restante)+1, 
				"%s",ahorcado->palabraAAdivinar);
		return DERROTA;
	} else if (letrasRestantes(ahorcado) == 0){
		return VICTORIA;
	} else {
		return SIN_NOVEDADES;
	}
}

int ahorcadoJugarCaracter(Ahorcado* ahorcado, char letraRecibida) {
	if (!ahorcado)
		return ERROR;
	jugarYValidarLetra(ahorcado, letraRecibida);
	return ahorcado->numIntentos;
}

char* ahorcadoPalabraRestante(Ahorcado* ahorcado) { 
	if (!ahorcado)
		return NULL;
	return ahorcado->restante;
}

int ahorcadoEstadoDePartida(Ahorcado* ahorcado) {
	if (!ahorcado)
		return ERROR;
	return estadoDePartida(ahorcado);
}

void ahorcadoDestruir(Ahorcado* ahorcado) {
	return;
}
