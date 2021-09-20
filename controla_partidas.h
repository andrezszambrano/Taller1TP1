#ifndef __CONTROLA_PARTIDAS_H__
#define __CONTROLA_PARTIDAS_H__

#include <stdio.h>
#include "ahorcado.h"

typedef struct ControlaPartidas{ //Struct que controla una sesión de ahorcados
	int numIntentos;
	FILE* ptrArchivo;
	Ahorcado partidaActual;
}ControlaPartidas;

void controlaPartidasInicializar(ControlaPartidas* ptrControla, int numIntentos, char* pathAArchivo);


void controlaPartidasEmpezar(ControlaPartidas* controlador);

#endif