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

int servidorInicializar(Servidor* servidor, char* argv[], int cantArgumentos);
	
int servidorProcesarLinea(Servidor* servidor);

void servidorMostrarResultados(Servidor* servidor);

void servidorDestruir(Servidor* servidor);

#endif
