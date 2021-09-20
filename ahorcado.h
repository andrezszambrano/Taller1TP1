#ifndef __AHORCADO_H__
#define __AHORCADO_H__

#include <stdio.h>

typedef struct Ahorcado{
	int numIntentos;
	char palabraAAdivinar[25];
	char restante[25];
}Ahorcado;

//Inicializa una partida de ahorcado para una palabra y con número de intentos
void ahorcadoInicializar(Ahorcado* ahorcado, char* palabra, int numIntentos);

//Se juega la partida
int ahorcadoJugar(Ahorcado* ahorcado);

#endif