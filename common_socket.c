#include "common_socket.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#define EXITO 0
#define ERROR -1
#define MAX_QUEUE 8
#define SOCKET_NO_DISPONIBLE 0

int socketInicializarYConectarCliente(socket_t* socketCliente, const char* host,
										 const char* servicio) {
	struct addrinfo baseaddr;  
	struct addrinfo* ptraddr;
	struct addrinfo* ptrAux;
	memset(&baseaddr, 0, sizeof(struct addrinfo));
	baseaddr.ai_socktype = SOCK_STREAM;
	baseaddr.ai_family = AF_UNSPEC; //Ipv4 o Ipv6 
	baseaddr.ai_flags = 0; //Ningún flag ya que es un cliente
	int aux = getaddrinfo(NULL, servicio, &baseaddr, &ptraddr);
	if (aux != EXITO){
		fprintf(stderr, "Error: %s\n", gai_strerror(aux));
		return ERROR;
	}
	int fdDelServidor = 0;
	bool conectados = false;
	ptrAux = ptraddr;
	while (ptrAux != NULL && conectados == false){
        fdDelServidor = socket(ptrAux->ai_family, ptrAux->ai_socktype, 
      						 ptrAux->ai_protocol);
        if (fdDelServidor == ERROR) {
            fprintf(stderr, "Error: %s\n", strerror(errno));
	    } else {
    	    aux = connect(fdDelServidor, ptrAux->ai_addr, ptrAux->ai_addrlen);
     	    if (aux == ERROR) {
         	    fprintf(stderr, "Error: %s\n", strerror(errno));
            	aux = close(fdDelServidor);
            	if (aux != EXITO)
					fprintf(stderr, "Error: %s\n", strerror(errno));
	        } else {
    	     	conectados = true;
    	    }
    	} 
        ptrAux = ptraddr->ai_next;		
	}
    freeaddrinfo(ptraddr);
    if (!conectados)
    	return ERROR;
    socketCliente->fd = fdDelServidor;
	return EXITO;
}


int socketInicializarServidorConBindYListen(socket_t* socketServidor, 
											const char* host, 
											const char* servicio) {
	struct addrinfo baseaddr;  
	struct addrinfo* ptraddr;
	struct addrinfo* ptrAux;
	memset(&baseaddr, 0, sizeof(struct addrinfo));
	baseaddr.ai_socktype = SOCK_STREAM;
	baseaddr.ai_family = AF_UNSPEC; //Ipv4 o Ipv6 
	baseaddr.ai_flags = AI_PASSIVE; //Las direcciones dadas podrán usar bind() 
	//y accept()
	int aux = getaddrinfo(NULL, servicio, &baseaddr, &ptraddr);
	if (aux != EXITO){
		fprintf(stderr, "Error: %s\n", gai_strerror(aux));
		return ERROR;
	}
	int fdServidor = 0;
	bool socketActivo = false;
	ptrAux = ptraddr;
	while (ptrAux != NULL && socketActivo == false){
        fdServidor = socket(ptrAux->ai_family, ptrAux->ai_socktype, 
      						 ptrAux->ai_protocol);
        if (fdServidor == ERROR) {
            printf("Error: %s\n", strerror(errno));
	    } else {
	    	int val = 1;
   			aux = setsockopt(fdServidor, SOL_SOCKET, SO_REUSEADDR, 
   							 &val, sizeof(val));
			if (aux != EXITO){
				fprintf(stderr, "Error: %s\n", strerror(errno));
				aux = close(fdServidor);
				if (aux != EXITO)
					fprintf(stderr, "Error: %s\n", strerror(errno));
			} else {
    		    aux = bind(fdServidor, ptraddr->ai_addr, ptraddr->ai_addrlen);
				if (aux != EXITO){
					fprintf(stderr, "Error: %s\n", strerror(errno));
					aux = close(fdServidor);
					if (aux != EXITO)
						fprintf(stderr, "Error: %s\n", strerror(errno));
				} else {
					socketActivo = true;
				}
			}
        }  
        ptrAux = ptraddr->ai_next;		
	}
	freeaddrinfo(ptraddr);
	aux = listen(fdServidor, MAX_QUEUE);
	if (aux != EXITO){
		fprintf(stderr, "Error: %s\n", strerror(errno));
		fprintf(stderr, "Error en la función listen\n");
		aux = close(fdServidor);
		if (aux != EXITO)
			fprintf(stderr, "Error: %s\n", strerror(errno));
		return ERROR;
	}
	socketServidor->fd = fdServidor;
	return EXITO;
}

int socketAceptar(socket_t* socketServidor, socket_t* socketCliente) {
	int fd = accept(socketServidor->fd, NULL, NULL); 
	if (fd == ERROR)
		return ERROR;
	socketCliente->fd = fd;
	return EXITO; 
}

ssize_t socketEnviar(socket_t* socket, const char* buffer, size_t length) {
	if (!socket)
		return ERROR;
	int escritos = 0;
	while (escritos < length){
		int aux = send(socket->fd, buffer + escritos,
						 length - escritos, MSG_NOSIGNAL);
		if (aux == ERROR)
			return ERROR;
		else if (aux == SOCKET_NO_DISPONIBLE)
			return escritos;
		escritos = escritos + aux;
	}
	return escritos;
}

ssize_t socketRecibir(socket_t* socket, char* buffer, size_t length) {
	if (!socket)
		return ERROR;
	int leidos = 0;
	while(leidos < length){
		int aux = recv(socket->fd, buffer + leidos,
					 length - leidos, 0);
		if(aux == ERROR)
			return ERROR;
		else if (aux == SOCKET_NO_DISPONIBLE)
			return leidos;
		leidos = leidos + aux;
	}
	return leidos;
}

void socketDestruir(socket_t* socket) {
	shutdown(socket->fd, SHUT_RDWR);
	int aux = close(socket->fd);
	if (aux != EXITO)
		fprintf(stderr, "Error: %s\n", strerror(errno));
}
