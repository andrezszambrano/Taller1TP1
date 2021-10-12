#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "client.h"
#define EXITO 0
#define ERROR -1

int main(int argc, char* argv[]) {
	Cliente cliente;
	int aux = clienteInicializar(&cliente, argv + 1, argc - 1);
	if (aux == ERROR)
		return ERROR;
	aux = clienteJugarPartida(&cliente);
	if (aux == ERROR){
		clienteDestruir(&cliente);
		return ERROR;
	}
	clienteDestruir(&cliente);
	return EXITO;
}
