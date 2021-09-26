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
#include <arpa/inet.h>
#include <stdint.h>
#define EXITO 0
#define ERROR_SOCKET -1
#define MAX_QUEUE 1
#define ERROR -1
#define SIN_PALABRAS -2
#define VICTORIA 150
#define DERROTA -1
int socket_iniciarServidor(char* puerto);

void enviarMensaje(int fdSocketCliente, int cantIntentos, uint16_t largoPalabra, char** infoRestante);

int main(int argc, char* argv[]){
	
	if(argc < 4){
		printf("Error, debe enviar primero el puerto, la cantidad de intentos por partida y el archivo nombre del archivo para jugar el ahorcado.\n");
		return 0;
	}
	ControlaPartidas controlador;
	int cantIntentos = (int)strtol(argv[2], NULL, 10);
	controlaPartidasInicializar(&controlador, cantIntentos, argv[3]);
	int fdSocketServidor = socket_iniciarServidor(argv[1]);
	if(fdSocketServidor == ERROR)
		return 0;
	char* restante;
	int16_t largoPalabra = (int16_t)controlaPartidasEmpezarNuevaPartida(&controlador, &restante);
	while(largoPalabra != SIN_PALABRAS){
		int fdSocketCliente = accept(fdSocketServidor, NULL, NULL);
		if(fdSocketCliente == ERROR)
			return 0;
		enviarMensaje(fdSocketCliente, cantIntentos, largoPalabra, &restante);
		char caracter;
		recv(fdSocketCliente, &caracter, sizeof(char), 0);
		int intentosRestantes = controlaPartidasJugarCaracter(&controlador, &caracter);
		while(intentosRestantes != VICTORIA && intentosRestantes != DERROTA){
			enviarMensaje(fdSocketCliente, intentosRestantes, largoPalabra, &restante);
			recv(fdSocketCliente, &caracter, sizeof(char), 0);
			intentosRestantes = controlaPartidasJugarCaracter(&controlador, &caracter);
		}
		enviarMensaje(fdSocketCliente, intentosRestantes, largoPalabra, &restante);
		shutdown(fdSocketCliente, SHUT_RDWR);
		close(fdSocketCliente);
		largoPalabra = (int16_t)controlaPartidasEmpezarNuevaPartida(&controlador, &restante);
	}		
	controlaPartidasResumen(&controlador);
	controlaPartidasDestruir(&controlador);
	shutdown(fdSocketServidor, SHUT_RDWR);
	close(fdSocketServidor);

	return 0;
}

void enviarMensaje(int fdSocketCliente, int cantIntentos, uint16_t largoPalabra, char** infoRestante){
	if(cantIntentos == DERROTA)
		cantIntentos = 128;
	unsigned char caracter = cantIntentos;
	int escritos = 0;
	while(escritos < 1){
		int aux = send(fdSocketCliente, &caracter, sizeof(char), MSG_NOSIGNAL);
		escritos = escritos + aux;
	}
	uint16_t largoPalabraBE = htons(largoPalabra); 
	escritos = 0;
	while(escritos < 2){
		int aux = send(fdSocketCliente, &largoPalabraBE, 2*sizeof(char), MSG_NOSIGNAL);
		escritos = escritos + aux;	
	}
	escritos = 0;
	while(escritos < largoPalabra){
		int aux = send(fdSocketCliente, *infoRestante + escritos, largoPalabra*sizeof(char) - escritos, MSG_NOSIGNAL);		
		escritos = escritos + aux;
	}
	
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