#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "ahorcado.h"
#include "controla_partidas.h"

#define EXITO 0
#define ERROR_SOCKET -1
#define MAX_QUEUE 1
#define ERROR -1

int socket_iniciarServidor(char* puerto);

int main(int argc, char* argv[]){
	
	if(argc < 4){
		printf("Error, debe enviar primero el puerto, la cantidad de intentos por partida y el archivo nombre del archivo para jugar el ahorcado.\n");
		return 0;
	}
	ControlaPartidas controlador;
	controlaPartidasInicializar(&controlador, atoi(argv[2]), argv[3]);
	int fdSocketServidor = socket_iniciarServidor(argv[1]);
	if(fdSocketServidor == ERROR)
		return 0;
	int fdSocketCliente = accept(fdSocketServidor, NULL, NULL);
	if(fdSocketCliente == ERROR)
		return 0;

	char msg[15];

	int aux = recv(fdSocketCliente, msg, 15*sizeof(char), 0);
	printf("Se recibieron %i bytes. El mensaje es: %s\n", aux, msg);
	controlaPartidasResumen(&controlador);
	sleep(30);
	shutdown(fdSocketCliente, SHUT_RDWR);
	shutdown(fdSocketServidor, SHUT_RDWR);
	close(fdSocketServidor);
	close(fdSocketCliente);
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
		return ERROR;
	}
	int fdServidor = socket(ptraddr->ai_family, ptraddr->ai_socktype, ptraddr->ai_protocol);
	if(fdServidor == ERROR_SOCKET){
		printf("Error creando el socket del servidor\n");
		freeaddrinfo(ptraddr);
		return ERROR;
	}
	int val = 1;
   	aux = setsockopt(fdServidor, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
	if(aux != EXITO){
		printf("Error: %s\n", strerror(errno));
		printf("Error asignandole un nombre al socket\n");
		freeaddrinfo(ptraddr);
		return ERROR;
	}
	aux = bind(fdServidor, ptraddr->ai_addr, ptraddr->ai_addrlen);
	if(aux != EXITO){
		printf("Error: %s\n", strerror(errno));
		printf("Error asignandole un nombre al socket\n");
		freeaddrinfo(ptraddr);
		return ERROR;
	}
	freeaddrinfo(ptraddr);
	aux = listen(fdServidor, MAX_QUEUE);
	if(aux != EXITO){
		printf("Error en la función listen\n");
		return ERROR;
	}
	return fdServidor;
}