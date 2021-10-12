#ifndef __SERVER_H__
#define __SERVER_H__

#include "common_socket.h"

//Struct que controla un cliente de una sesión de ahorcados
typedef struct Cliente{  
	socket_t socketCliente;
}Cliente;

int clienteInicializar(Cliente* cliente, char* argv[], int cantArgumentos);

int clienteJugarPartida(Cliente* cliente);

void clienteDestruir(Cliente* cliente);

#endif
