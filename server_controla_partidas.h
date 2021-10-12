#ifndef __CONTROLA_PARTIDAS_H__
#define __CONTROLA_PARTIDAS_H__

#include <stdio.h>
#include <stdbool.h>
#include "server_ahorcado.h"

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
// se retorna 1.
int controlaPartidasEmpezarNuevaPartida(ControlaPartidas* controlador);

//Se regresan las cantidad de intentos totales con las cuales se comienzan las 
//partidas de ahorcado. Sí no existe el controlador, se devuelve -1.  
int controlaPartidasIntentosPorPartida(ControlaPartidas* controlador);

//Se juegan los caracteres enviados a la partida de ahorcados actual. Sí 
//controlador->partidaEnJuego = false, devuelve error. Se regresa la cantidad 
//de intentos restantes después de jugar el carácter. Sí el puntero a 
//controlador no existe se retorna -1.
int controlaPartidasJugarCaracter(ControlaPartidas* controlador, char caracter);

//Se actualiza la partida en juego sí hubo victoria o derrota, modificando
//los atributos del controlador acorde.
//Se regresa el estado de la partida en juego, sí la partida acabó y es victoria
//se regresa 1, sí acabó y es derrota se regresa 2, y sí no hay novedades se
//regresa 0. Sí el controlador no existe se regresa -1.
int controlaPartidasActualizarYDarEstadoActualDePartida(ControlaPartidas* 
	                                                     conrolador);

//Se retorna el puntero a la palabra parcialmente adivinada. En caso de que el
//controlador no exista se retorna NULL.
char* controlaPartidasPalabraRestante(ControlaPartidas* controlador);

//Da el resumen de victorias y derrotas del controlador, imprimiendolos por el
//stdout.
void controlaPartidasResumen(ControlaPartidas* controlador);

//Libera todos los recursos usados.
void controlaPartidasDestruir(ControlaPartidas* controlador);
#endif
