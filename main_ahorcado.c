#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ahorcado.h"
#include "controla_partidas.h"

#define FIN_DE_PARTIDA 1
#define SIN_PARTIDAS -2
#define MAX_CARACTERES 5

void vaciarBuffer();

void vaciarBuffer(){
	char aux = getchar();
	while(aux != '\n')
		aux = getchar();
}

void pedirCaracter(char* caracter);

void pedirCaracter(char* caracter){
	int i = 0;
	char aux = getchar();
	while(i < MAX_CARACTERES && aux != '\n'){
		caracter[i] = aux;
		aux = getchar();
		i++;
	}
	if(i == MAX_CARACTERES && aux != '\n')
		vaciarBuffer();
	caracter[i] = '\0';	
}

int main(int argc, char* argv[]){
	
	if(argc < 3){
		printf("Error, debe enviar primero la cantidad de intentos y una palabra para jugar el ahorcado.\n");
		return 0;
	}

	ControlaPartidas controlador;
	controlaPartidasInicializar(&controlador, atoi(argv[1]), argv[2]);
	int aux = controlaPartidasEmpezarNuevaPartida(&controlador);
	while(aux != SIN_PARTIDAS){
		char caracter[MAX_CARACTERES]; 
		pedirCaracter(caracter);
		int aux2 = controlaPartidasJugarCaracter(&controlador, caracter);
		while(aux2 != FIN_DE_PARTIDA){
			pedirCaracter(caracter);
			aux2 = controlaPartidasJugarCaracter(&controlador, caracter);
		}
		aux = controlaPartidasEmpezarNuevaPartida(&controlador);
	}

	controlaPartidasResumen(&controlador);
	return 0;
}

