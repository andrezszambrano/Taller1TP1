#define _POSIX_C_SOURCE 200112L
#include "socket.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#define EXITO 0
#define ERROR -1
#define MAX_QUEUE 8

int socketInicializarYConectarCliente(socket_t* socketCliente, const char* host,
										 const char* servicio){
	struct addrinfo baseaddr;  
	struct addrinfo* ptraddr;
	memset(&baseaddr, 0, sizeof(struct addrinfo));
	baseaddr.ai_socktype = SOCK_STREAM;
	baseaddr.ai_family = AF_UNSPEC; //Ipv4 o Ipv6 
	baseaddr.ai_flags = 0; //Ningún flag ya que es un cliente
	int aux = getaddrinfo(NULL, servicio, &baseaddr, &ptraddr);
	if(aux != EXITO){
		printf("Error: %s\n", strerror(errno));
		printf("Error al intentar obtener las direcciones\n");
		return ERROR;
	}
	int fdDelServidor = socket(ptraddr->ai_family, ptraddr->ai_socktype,
								 ptraddr->ai_protocol);
	if(fdDelServidor == ERROR){
		printf("Error: %s\n", strerror(errno));
		printf("Error creando el socket del servidor\n");
		freeaddrinfo(ptraddr);
		return ERROR;
	}
	aux = connect(fdDelServidor, ptraddr->ai_addr, ptraddr->ai_addrlen);
    if (aux == -1) {
        printf("Error al conectarse al puerto\n");
        close(fdDelServidor);
        freeaddrinfo(ptraddr);
    	return ERROR;
    }
    freeaddrinfo(ptraddr);
    socketCliente->fd = fdDelServidor;
	return EXITO;
}

int socketInicializarServidorConBindYListen(socket_t* socketServidor, 
											const char* host, 
											const char* servicio){
	struct addrinfo baseaddr;  
	struct addrinfo* ptraddr;
	memset(&baseaddr, 0, sizeof(struct addrinfo));
	baseaddr.ai_socktype = SOCK_STREAM;
	baseaddr.ai_family = AF_UNSPEC; //Ipv4 o Ipv6 
	baseaddr.ai_flags = AI_PASSIVE; //Las direcciones dadas podrán usar bind() 
	//y accept()
	int aux = getaddrinfo(NULL, servicio, &baseaddr, &ptraddr);
	if(aux != EXITO){
		printf("Error: %s\n", strerror(errno));
		printf("Error al intentar obtener las direcciones\n");
		return ERROR;
	}
	int fdServidor = socket(ptraddr->ai_family, ptraddr->ai_socktype, 
							ptraddr->ai_protocol);
	if(fdServidor == ERROR){
		printf("Error: %s\n", strerror(errno));
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
		printf("Error: %s\n", strerror(errno));
		printf("Error en la función listen\n");
		return ERROR;
	}
	socketServidor->fd = fdServidor;
	return EXITO;
}

int socketAceptar(socket_t* socketServidor, socket_t* socketCliente){
	int fd = accept(socketServidor->fd, NULL, NULL); 
	if(fd == ERROR)
		return ERROR;
	socketCliente->fd = fd;
	return EXITO; 
}

ssize_t socketEnviar(socket_t* socket, void* buffer, size_t length){
	if(!socket)
		return ERROR;
	int escritos = 0;
	while(escritos < length){
		int aux = send(socket->fd, buffer + escritos,
						 length*sizeof(int8_t) - escritos, MSG_NOSIGNAL);
		escritos = escritos + aux;
	}
	return escritos;
}

ssize_t socketEnviarShort(socket_t* socket, int numAEnviar){
	if(!socket)
		return ERROR;
	uint16_t numBE = htons(numAEnviar); //Número estará en big endian 
	int escritos = 0;
	while(escritos < 2){
		int aux = send(socket->fd, &numBE, 2*sizeof(int8_t), MSG_NOSIGNAL);
		escritos = escritos + aux;	
	}
	return escritos;
}


ssize_t socketRecibir(socket_t* socket, void* buffer, size_t length){
	if(!socket)
		return ERROR;
	int leidos = 0;
	while(leidos < length){
		int aux = recv(socket->fd, buffer + leidos,
					 length*sizeof(int8_t) - leidos, 0);
		leidos = leidos + aux;
	}
	return leidos;
}

ssize_t socketRecibirShort(socket_t* socket, int* numARecibir){
	if(!socket)
		return ERROR;
	int leidos = 0;
	uint16_t numBE; 		
	while(leidos < 2){
		int aux = recv(socket->fd, &numBE, 2*sizeof(int8_t), 0);
		leidos = leidos + aux;	
	}
	*numARecibir = ntohs(numBE); //Número pasará de BE a lo que maneje la pc
	//(ya sea LE o BE) 
	return leidos;
}

void socketDestruir(socket_t* socket){
	shutdown(socket->fd, SHUT_RDWR);
	close(socket->fd);

}