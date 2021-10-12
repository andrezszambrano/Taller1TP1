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
int ahorcadoInicializar(Ahorcado* ahorcado, char* palabra, int numIntentos);

//Se juega el caracter enviado por parámetro. Se regresan la cantidad de 
//intentos restantes después de jugar el carácter. Sí no existe el puntero al
//ahorcado, se devuelve -1.
int ahorcadoJugarCaracter(Ahorcado* ahorcado, char letra);

//Se retorna el puntero a la palabra parcialmente adivinada. Sí la partida
//terminó (ya sea victoria o derrota), se regresa la palabra que se tenía que
//adividar. En caso de que no exista el puntero al ahorcado se retorna NULL.
char* ahorcadoPalabraRestante(Ahorcado* ahorcado);

//Se regresa el estado de la partida: se regresa 2 en caso de que se haya 
//perdido la partida, 1 sí se ganó la partida y 0 sí la partida todavía no ha
//acabado. Sí el puntero al ahorcado no existe, se retorna -1.
int ahorcadoEstadoDePartida(Ahorcado* ahorcado);

//Destruye los recursos usados por el ahorcado.
void ahorcadoDestruir(Ahorcado* ahorcado);
#endif
