#ifndef __AHORCADO_H__
#define __AHORCADO_H__

#include <stdio.h>

typedef struct Ahorcado{
	int numIntentos;
	char palabraAAdivinar[25];
	char restante[25];
}Ahorcado;

void ahorcadoInicializar(Ahorcado* ahorcado, char* palabra, int numIntentos);

void ahorcadoJugar(Ahorcado* ahorcado);

#endif