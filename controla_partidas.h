#ifndef __CONTROLA_PARTIDAS_H__
#define __CONTROLA_PARTIDAS_H__

#include <stdio.h>
#include <stdbool.h>
#include "ahorcado.h"

typedef struct ControlaPartidas{ //Struct que controla una sesión de ahorcados
	int numIntentos;
	FILE* ptrArchivo;
	Ahorcado partidaActual;
	int partidasGanadas;
	int partidasPerdidas;
	bool partidaEnJuego;
}ControlaPartidas;


//Se crea un controla partidas. Son partidas con numIntentos, y las palabras 
//salen de la lectura del archivo mandado por parámetro
void controlaPartidasInicializar(ControlaPartidas* ptrControla, int numIntentos,
								 char* pathAArchivo);

//Se inicia una nueva partida sí controlador->partidaEnJuego = false y sí hay 
//una palabra que no se ha jugado. En caso exitoso se retorna el largo de la 
//palabra a jugar. En caso de que no haya palabra (se leyó todo el archivo)
// se retorna -2
int controlaPartidasEmpezarNuevaPartida(ControlaPartidas* controlador, 
										char** infoRestante);

//Se juegan los caracteres enviados a la partida de ahorcados actual. Sí 
//controlador->partidaEnJuego = false, devuelve error. Se regresa -1 sí se
// acabaron los intentos después de validar el caracter, 150 sí se ganó la 
//partida, o la cantidad de intentos restantes sí no hay novedades.
int controlaPartidasJugarCaracter(ControlaPartidas* controlador, char caracter);

//Da el resumen de victorias y derrotas del controlador.
void controlaPartidasResumen(ControlaPartidas* controlador);

//Libera todos los recursos usados.
void controlaPartidasDestruir(ControlaPartidas* controlador);
#endif