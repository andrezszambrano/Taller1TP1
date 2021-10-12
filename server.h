#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdio.h>
#include "server_ahorcado.h"
#include "common_socket.h"
#include "server_controla_partidas.h"

typedef struct Servidor{ //Struct que controla un servidor de ahorcados
	socket_t socketServidor;
	socket_t socketCliente;
	ControlaPartidas controlaPartidas;
}Servidor;

//Inicializa un servidor de ahorcados. Recibe un puntero a un servidor, un 
//vector de punteros a caracteres, el primero apuntando al servidor a 
//conectarse, el segundo a la cantidad de intentos que tendrá cada partida de 
//ahorcados, y el tercero al nombre del archivo de texto en donde están las
//palabras. En caso de que no se reciban los punteros requeridos, o no se pueda
//crear el socket, se retorna -1. Se retorna 0 sí se crea el servidor con 
//exito. Cualquier error relacionado con el socket se imprime en stderr.
int servidorInicializar(Servidor* servidor, char* argv[], int cantArgumentos);
	
//Procesa una línea del archivo de texto, se conecta con un cliente y se juega
//la partida. Sí no hay más palabras por procesar se regresa 1. Sí el puntero a
//servidor no existe, hay un error al crear el socket cliente, o si hay un
//error en la comunicación con el cliente, se retorna -1. En caso de éxito 
//se retorna 0. Cualquier error relacionado con el socket se imprime en stderr.
int servidorProcesarLinea(Servidor* servidor);

//Se imprime por stdout los resultados de victorias/derrotas del servidor.
void servidorMostrarResultados(Servidor* servidor);

//Se destruyen todos los recursos usados por el servidor.
void servidorDestruir(Servidor* servidor);

#endif
