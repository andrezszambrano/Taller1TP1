#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


#define EXITO 0
#define ERROR_SOCKET -1
#define MAX_QUEUE 1
#define ERROR -1

int socket_iniciarCliente(char* host , char* puerto);

int main(int argc, char* argv[]){
	
	if(argc < 3){
		printf("Error, debe enviar primero el host y después el número de puerto a conectarse.\n");
		return 0;
	}

	int fdSocketServidor = socket_iniciarCliente(argv[1], argv[2]);
	if(fdSocketServidor == ERROR)
		return 0;

	char* hola = "Hola puerto";
	int aux = send(fdSocketServidor, hola, strlen(hola), MSG_NOSIGNAL);
	printf("Se envió %i bytes.\n", aux);
	sleep(30);
	shutdown(fdSocketServidor, SHUT_RDWR);
	close(fdSocketServidor);
	return 0;
}



int socket_iniciarCliente(char* host , char* puerto){
	struct addrinfo baseaddr;  
	struct addrinfo* ptraddr;
	memset(&baseaddr, 0, sizeof(struct addrinfo));
	baseaddr.ai_socktype = SOCK_STREAM;
	baseaddr.ai_family = AF_UNSPEC; //Ipv4 o Ipv6 
	baseaddr.ai_flags = 0;
	int aux = getaddrinfo(host, puerto, &baseaddr, &ptraddr);
	if(aux != EXITO){
		printf("Error al intentar obtener las direcciones\n");
		return ERROR;
	}
	int fdCliente = socket(ptraddr->ai_family, ptraddr->ai_socktype, ptraddr->ai_protocol);
	

	if(fdCliente == ERROR_SOCKET){
		printf("Error creando el socket del servidor\n");
		freeaddrinfo(ptraddr);
		return ERROR;
	}

	aux = connect(fdCliente, ptraddr->ai_addr, ptraddr->ai_addrlen);
    if (aux == -1) {
        printf("Error al conectarse al puerto\n");
        close(fdCliente);
        freeaddrinfo(ptraddr);
    	return ERROR;
    }
    freeaddrinfo(ptraddr);
	/*
	int val = 1;
   	aux = setsockopt(fdServidor, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
	if(aux != EXITO){
		printf("Error asignandole un nombre al socket\n");
		freeaddrinfo(ptraddr);
		return ERROR;
	}
	aux = bind(fdServidor, ptraddr->ai_addr, ptraddr->ai_addrlen);
	if(aux != EXITO){
		printf("Error asignandole un nombre al socket\n");
		freeaddrinfo(ptraddr);
		return ERROR;
	}
	freeaddrinfo(ptraddr);
	aux = listen(fdServidor, MAX_QUEUE);
	if(aux != EXITO){
		printf("Error en la función listen\n");
		return ERROR;
	}*/
	return fdCliente;
}