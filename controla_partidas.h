#ifndef __CONTROLA_PARTIDAS_H__
#define __CONTROLA_PARTIDAS_H__

#include <stdio.h>
#include "ahorcado.h"

typedef struct ControlaPartidas{ //Struct que controla una sesión de ahorcados
	int numIntentos;
	FILE* ptrArchivo;
	Ahorcado partidaActual;
	int partidasGanadas;
	int partidasPerdidas;
}ControlaPartidas;


//Se crea un controla partidas. Son partidas con numIntentos, y las palabras salen de la lectura
//del archivo mandado por parámetro
void controlaPartidasInicializar(ControlaPartidas* ptrControla, int numIntentos, char* pathAArchivo);


//Comienza el ciclo de juego. Se lee una palabra del archivo, se juega al ahorcado, se termina
//la partida, y se inicia otra partida con la palabra siguiente, así hasta que se lean todas 
//las palabras del archivo
void controlaPartidasEmpezar(ControlaPartidas* controlador);

#endif