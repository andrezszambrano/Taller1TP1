#include "controla_partidas.h"
#include <string.h>


void controlaPartidasInicializar(ControlaPartidas* ptrControla, int numIntentos, char* pathAArchivo){
	if(ptrControla == NULL)
		return;
	ptrControla->numIntentos = numIntentos;
	ptrControla->ptrArchivo = fopen(pathAArchivo, "r");
	if (ptrControla->ptrArchivo == NULL){
		printf("Error al abrir el archivo.");
		return;
	}
}	

void controlaPartidasEmpezar(ControlaPartidas* controlador){
	char palabra[25];
	char* ptrPalabra = palabra;
	size_t len = 25;
	int aux = getline(&ptrPalabra, &len, controlador->ptrArchivo);
	while (aux != EOF){
		ahorcadoInicializar(&(controlador->partidaActual), palabra,controlador->numIntentos);
		ahorcadoJugar(&controlador->partidaActual);
		aux = getline(&ptrPalabra, &len, controlador->ptrArchivo);
	}

}