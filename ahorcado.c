#include "ahorcado.h"
#include <string.h>
#include <stdbool.h>

#define SIN_NOVEDADES 0
#define SIN_INTENTOS -1
#define VICTORIA 150
#define ERROR -10

void imprimirPalabraSecretaYNumIntentos(Ahorcado* ahorcado){
	printf("Palabra secreta: %s\n", ahorcado->restante);
	printf("Te quedan %i intentos\n", ahorcado->numIntentos);
	printf("Ingrese letra:\n");
}

void imprimirMensajeDeFinDePartida(Ahorcado* ahorcado, int aux){
	if(aux == VICTORIA)
		printf("Ganaste!!\n");
	else
		printf("Perdiste! La palabra secreta era: %s\n",ahorcado->palabraAAdivinar);
}

int ahorcadoInicializar(Ahorcado* ahorcado, char* palabra, int numIntentos, char** infoRestante){
	if(ahorcado == NULL)
		return ERROR;
	ahorcado->numIntentos = numIntentos;
	strncpy(ahorcado->palabraAAdivinar, palabra, strlen(palabra)*sizeof(char));
	ahorcado->palabraAAdivinar[strlen(palabra)-1]='\0';
	for(int i = 0; i < strlen(ahorcado->palabraAAdivinar); i++)
		ahorcado->restante[i] = '_';
	ahorcado->restante[strlen(ahorcado->palabraAAdivinar)]='\0';
	*infoRestante = ahorcado->restante;
	return strlen(ahorcado->palabraAAdivinar);
}

void validarLetra(Ahorcado* ahorcado, char letra){
	bool letraIncorrecta = true;
	int len = strlen(ahorcado->palabraAAdivinar);
	for(int i = 0; i < len; i++){
		if(ahorcado->palabraAAdivinar[i] == letra){
			ahorcado->restante[i] = letra;
			letraIncorrecta = false;
		}
	}
	if(letraIncorrecta){
		ahorcado->numIntentos--;
	}
}

int letrasRestantes(Ahorcado* ahorcado){
	int len = strlen(ahorcado->palabraAAdivinar);
	int contador = 0;
	for(int i = 0; i < len; i++){
		if(ahorcado->restante[i] == '_')
			contador++;
	}
	return contador;
}

int estadoDePartida(Ahorcado* ahorcado){
	if (ahorcado->numIntentos == 0){
		strcpy(ahorcado->restante, ahorcado->palabraAAdivinar);
		return SIN_INTENTOS;
	}
	else if (letrasRestantes(ahorcado) == 0)
		return VICTORIA;
	else
		return ahorcado->numIntentos;

}

int ahorcadoJugarCaracter(Ahorcado* ahorcado, char letraRecibida){
	if(!ahorcado)
		return ERROR;
	validarLetra(ahorcado, letraRecibida);
	int aux = estadoDePartida(ahorcado);
	if(aux != VICTORIA && aux != SIN_INTENTOS)
		imprimirPalabraSecretaYNumIntentos(ahorcado);
	else
		imprimirMensajeDeFinDePartida(ahorcado, aux);
	return aux;
}
