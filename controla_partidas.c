#include "controla_partidas.h"
#include <string.h>
#define VICTORIA 2
#define DERROTA 1
#define SIN_NOVEDADES 0 
#define SIN_PALABRAS_RESTANTES -2
#define PARTIDA_EN_JUEGO 1
#define EXITO 0
#define ERROR -1
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

int controlaPartidasEmpezarNuevaPartida(ControlaPartidas* controlador){
	if(!controlador)
		return ERROR;
	if(controlador->partidaEnJuego)
		return PARTIDA_EN_JUEGO;
	char palabra[25];
	char* ptrPalabra = palabra;
	size_t len = 25;
	int aux = getline(&ptrPalabra, &len, controlador->ptrArchivo);
	if (aux != EOF){
		ahorcadoInicializar(&(controlador->partidaActual), palabra, controlador->numIntentos);
		controlador->partidaEnJuego = true; 
		return EXITO;
	}else
		return SIN_PALABRAS_RESTANTES;
}

int controlaPartidasJugarCaracter(ControlaPartidas* controlador, char* caracteres){
	if(!controlador)
		return ERROR;
	if(!controlador->partidaEnJuego)
		return ERROR;
	bool mandarLetra = true;
	int i = 0;
	int len = strlen(caracteres);
	int aux;
	while(mandarLetra && i < len){
		aux = ahorcadoJugarCaracter(&(controlador->partidaActual), caracteres[i]);
		if(aux == VICTORIA || aux == DERROTA)
			mandarLetra = false;
		i++;
	}
	if(aux == SIN_NOVEDADES)
		return aux;
	else if(aux == VICTORIA)
		controlador->partidasGanadas++;
	else
		controlador->partidasPerdidas++;	
	controlador->partidaEnJuego = false;
	return FIN_DE_PARTIDA;
	
}

void controlaPartidasResumen(ControlaPartidas* controlador){
	if(!controlador)
		return;
	printf("Resumen:\n");
	printf("\tVictorias: %i\n", controlador->partidasGanadas);
	printf("\tDerrotas: %i\n", controlador->partidasPerdidas);
}