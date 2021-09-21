#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "ahorcado.h"
#include "controla_partidas.h"

#define EXITO 0
#define ERROR_SOCKET -1
#define MAX_QUEUE 1


int main(int argc, char* argv[]){
	
	if(argc < 4){
		printf("Error, debe enviar primero la cantidad de intentos y una palabra para jugar el ahorcado.\n");
		return 0;
	}
	ControlaPartidas controlador;
	controlaPartidasInicializar(&controlador, atoi(argv[2]), argv[3]);
	int fdSocketServidor = socket_iniciarServidor(argv[1]);
	controlaPartidasEmpezar(&controlador);	
	controlaPartidasResumen(&controlador);
	return 0;
}



int socket_iniciarServidor(char* puerto){
	struct addrinfo baseaddr;  
	struct addrinfo* ptraddr;
	memset(&baseaddr, 0, sizeof(struct addrinfo));
	baseaddr.ai_socktype = SOCK_STREAM;
	baseaddr.ai_family = AF_UNSPEC; //Ipv4 o Ipv6 
	baseaddr.ai_flags = AI_PASSIVE; //Las direcciones dadas podrán usar bind() y accept()
	int aux = getaddrinfo(NULL, puerto, &baseaddr, &ptraddr);
	if(aux != EXITO){
		printf("Error al intentar obtener las direcciones\n");
		return 0;
	}
	int fdServidor = socket(ptraddr->ai_family, ptraddr->ai_socktype, ptraddr->ai_protocol);
	if(fdServidor == ERROR_SOCKET){
		printf("Error creando el socket del servidor\n");
		freeaddrinfo(ptraddr);
		return 0;
	}
	aux = bind(fdServidor, ptraddr->ai_addr, ptraddr->ai_addrlen);
	if(aux != EXITO){
		printf("Error asignandole un nombre al socket\n");
		freeaddrinfo(ptraddr);
		return 0;
	}
	freeaddrinfo(ptraddr);
	aux = listen(fdServidor, MAX_QUEUE);
	if(aux != EXITO){
		printf("Error en la función listen\n");
		return 0;
	}
	return fdServidor;
}