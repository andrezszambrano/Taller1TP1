#include "controla_partidas.h"
#include <string.h>

#define VICTORIA 2
#define DERROTA 1

void controlaPartidasInicializar(ControlaPartidas* ptrControla, int numIntentos, char* pathAArchivo){
	if(ptrControla == NULL)
		return;
	ptrControla->numIntentos = numIntentos;
	ptrControla->partidasGanadas = 0;
	ptrControla->partidasPerdidas = 0;
	ptrControla->ptrArchivo = fopen(pathAArchivo, "r");
	if (ptrControla->ptrArchivo == NULL)
		printf("Error al abrir el archivo.");
}	

void controlaPartidasEmpezar(ControlaPartidas* controlador){
	char palabra[25];
	char* ptrPalabra = palabra;
	size_t len = 25;
	int aux = getline(&ptrPalabra, &len, controlador->ptrArchivo);
	while (aux != EOF){
		ahorcadoInicializar(&(controlador->partidaActual), palabra, controlador->numIntentos);
		int retJugar = ahorcadoJugar(&controlador->partidaActual);
		if (retJugar == VICTORIA)
			controlador->partidasGanadas++;
		else
			controlador->partidasPerdidas++;
		aux = getline(&ptrPalabra, &len, controlador->ptrArchivo);
	}
}

void controlaPartidasResumen(ControlaPartidas* controlador){
	if(controlador == NULL)
		return;

	printf("Resumen:\n");
	printf("\tVictorias: %i\n", controlador->partidasGanadas);
	printf("\tDerrotas: %i\n", controlador->partidasPerdidas);
}