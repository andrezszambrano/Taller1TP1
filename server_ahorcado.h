#ifndef __AHORCADO_H__
#define __AHORCADO_H__

#include <stdio.h>

#define MAX_PALABRA 25

typedef struct Ahorcado{
	int numIntentos;
	char palabraAAdivinar[MAX_PALABRA];
	char restante[MAX_PALABRA];
}Ahorcado;

//Inicializa una partida de ahorcado para una palabra y con número de intentos
//Regresa el largo de la palabra
int ahorcadoInicializar(Ahorcado* ahorcado, char* palabra, int numIntentos, 
						char** infoRestante);

//Se juega el caracter enviado por parámetro. Se regresa -1 sí se acabaron los 
//intentos al verificar el caracter, 150 sí se ganó la partida, o la cantidad 
//de intentos restantes sí no hay novedades.
int ahorcadoJugarCaracter(Ahorcado* ahorcado, char letra);

#endif
