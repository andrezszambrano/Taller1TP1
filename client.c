

#include "client.h"
#include "common_comunicacion_con_protocolo.h"

#define ARGUMENTOS_REQUERIDOS 2
#define EXITO 0
#define ERROR -1
#define VICTORIA 1
#define DERROTA 2



int clienteInicializar(Cliente* cliente, char* argv[], int cantArgumentos) {
	if (!cliente || cantArgumentos != ARGUMENTOS_REQUERIDOS)
		return ERROR;
	socket_t* socketCliente = &(cliente->socketCliente);
	int aux = socketInicializarYConectarCliente(socketCliente, argv[0], 
												 argv[1]);
	if (aux == ERROR)
		return ERROR;
	return EXITO;
}

int clienteJugarPartida(Cliente* cliente) {
	if (!cliente)
		return ERROR;
	socket_t* socketCliente = &(cliente->socketCliente);
	int aux = protocoloClienteRecibirMensaje(socketCliente);
	if(aux == ERROR)
		return ERROR;
	while(aux != VICTORIA && aux != DERROTA){
		aux = protocoloClienteEnviarCaracteresYRecibirMensaje(socketCliente);
		if(aux == ERROR)	
			return ERROR;
	}
	return EXITO;
}

void clienteDestruir(Cliente* cliente){
	if (!cliente)
		return;
	socketDestruir(&(cliente->socketCliente));
}
