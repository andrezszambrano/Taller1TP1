#include <stdio.h>
#include "server.h"


#define ERROR -1
#define EXITO 0
#define SIN_PALABRAS 1

int terminarProcesoYLibrerarRecursosConMensaje(Servidor* servidor, int num);

int main(int argc, char* argv[]){
	Servidor servidor;
	int aux = servidorInicializar(&servidor, argv + 1, argc - 1);
	if (aux == ERROR)
		return ERROR;	
	aux = servidorProcesarLinea(&servidor);
	if (aux == ERROR)
		return terminarProcesoYLibrerarRecursosConMensaje(&servidor, ERROR);
	while (aux != SIN_PALABRAS){
		aux = servidorProcesarLinea(&servidor);
		if (aux == ERROR)
			return terminarProcesoYLibrerarRecursosConMensaje(&servidor, ERROR);
	}
	servidorMostrarResultados(&servidor);
	return terminarProcesoYLibrerarRecursosConMensaje(&servidor, EXITO);
}

int terminarProcesoYLibrerarRecursosConMensaje(Servidor* servidor, int num) {
	servidorDestruir(servidor);
	if (num == ERROR)
		return ERROR;
	else 
		return EXITO;
}
