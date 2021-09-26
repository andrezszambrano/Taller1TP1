#include "controla_partidas.h"
#include <string.h>
#define VICTORIA 150
#define DERROTA -1
#define SIN_NOVEDADES 0 
#define SIN_PALABRAS_RESTANTES -2
#define PARTIDA_EN_JUEGO 1
#define ERROR -10
#define FIN_DE_PARTIDA 1

void controlaPartidasInicializar(ControlaPartidas* ptrControla, int numIntentos, char* pathAArchivo){
	if(!ptrControla)
		return;
	ptrControla->numIntentos = numIntentos;
	ptrControla->partidasGanadas = 0;
	ptrControla->partidasPerdidas = 0;
	ptrControla->partidaEnJuego = false;
	ptrControla->ptrArchivo = fopen(pathAArchivo, "r");
	if (ptrControla->ptrArchivo == NULL)
		printf("Error al abrir el archivo.");
}	

int controlaPartidasEmpezarNuevaPartida(ControlaPartidas* controlador, char** infoRestante){
	if(!controlador)
		return ERROR;
	if(controlador->partidaEnJuego)
		return PARTIDA_EN_JUEGO;
	char palabra[25];
	char* ptrPalabra = palabra;
	size_t len = 25;
	int aux = getline(&ptrPalabra, &len, controlador->ptrArchivo);
	if (aux != EOF){
		int lenPalabra = ahorcadoInicializar(&(controlador->partidaActual), palabra, controlador->numIntentos, infoRestante);
		controlador->partidaEnJuego = true; 
		return lenPalabra;
	}else
		return SIN_PALABRAS_RESTANTES;
}

int controlaPartidasJugarCaracter(ControlaPartidas* controlador, char* caracter){
	if(!controlador)
		return ERROR;
	if(!controlador->partidaEnJuego)
		return ERROR;

	int aux = ahorcadoJugarCaracter(&(controlador->partidaActual), *caracter);

	if(aux != VICTORIA && aux != DERROTA)
		return aux;
	else if(aux == VICTORIA)
		controlador->partidasGanadas++;
	else
		controlador->partidasPerdidas++;	
	controlador->partidaEnJuego = false;
	return aux;
}

void controlaPartidasResumen(ControlaPartidas* controlador){
	if(!controlador)
		return;
	printf("Resumen:\n");
	printf("\tVictorias: %i\n", controlador->partidasGanadas);
	printf("\tDerrotas: %i\n", controlador->partidasPerdidas);
}

void controlaPartidasDestruir(ControlaPartidas* controlador){
	if(!controlador)
		return;
	fclose(controlador->ptrArchivo);
}