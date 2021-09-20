#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ahorcado.h"
#include "controla_partidas.h"

int main(int argc, char* argv[]){
	
	if(argc < 3){
		printf("Error, debe enviar primero la cantidad de intentos y una palabra para jugar el ahorcado.\n");
		return 0;
	}

	ControlaPartidas controlador;
	controlaPartidasInicializar(&controlador, atoi(argv[1]), argv[2]);
	controlaPartidasEmpezar(&controlador);	
	return 0;
}