#include "server_ahorcado.h"
#include <string.h>
#include <stdbool.h>

#define SIN_NOVEDADES 0
#define SIN_INTENTOS -1
#define VICTORIA 150
#define ERROR -10

int ahorcadoInicializar(Ahorcado* ahorcado, char* palabra, 
						int numIntentos, char** infoRestante){
	if (ahorcado == NULL)
		return ERROR;
	ahorcado->numIntentos = numIntentos;
	snprintf(ahorcado->palabraAAdivinar, MAX_PALABRA, 
			"%s",palabra);
	ahorcado->palabraAAdivinar[strlen(palabra)-1] = '\0';
	for (int i = 0; i < strlen(ahorcado->palabraAAdivinar); i++)
		ahorcado->restante[i] = '_';
	ahorcado->restante[strlen(ahorcado->palabraAAdivinar)]='\0';

	// Esto rompe el encapsulamiento de la palabra, no?
	*infoRestante = ahorcado->restante;
	return strlen(ahorcado->palabraAAdivinar);
}

// No valida una letra, sino que intenta jugarla.
void validarLetra(Ahorcado* ahorcado, char letra) {
	bool letraIncorrecta = true;
	int len = strlen(ahorcado->palabraAAdivinar);
	for (int i = 0; i < len; i++) {
		if (ahorcado->palabraAAdivinar[i] == letra) {
			ahorcado->restante[i] = letra;
			letraIncorrecta = false;
		}
	}
	if (letraIncorrecta) {
		ahorcado->numIntentos--;
	}
}

int letrasRestantes(Ahorcado* ahorcado) {
	// por performance, esto lo podrías ir calculando a medida que se adivinan letras.
	int len = strlen(ahorcado->palabraAAdivinar);
	int contador = 0;
	for (int i = 0; i < len; i++) {
		if (ahorcado->restante[i] == '_')
			contador++;
	}
	return contador;
}

// Esta función y las macros con códigos de retorno podría ser parte del contrato, y 
// de esa manera no tendrías que retornar códigos raros 
int estadoDePartida(Ahorcado* ahorcado) {
	if (ahorcado->numIntentos == 0) {
		snprintf(ahorcado->restante, strlen(ahorcado->restante)+1, 
				 "%s", ahorcado->palabraAAdivinar);
		return SIN_INTENTOS;
	} else if (letrasRestantes(ahorcado) == 0) {
		return VICTORIA;
	} else {
		return ahorcado->numIntentos;
	}
}

int ahorcadoJugarCaracter(Ahorcado* ahorcado, char letraRecibida){
	if (!ahorcado)
		return ERROR;
	validarLetra(ahorcado, letraRecibida);
	int aux = estadoDePartida(ahorcado);
	return aux;
}
