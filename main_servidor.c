#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include "ahorcado.h"
#include "controla_partidas.h"
#include "socket.h"
#include <stdint.h>
#define EXITO 0
#define ERROR -1
#define SIN_PALABRAS -2
#define VICTORIA 150
#define DERROTA -1

void enviarMensaje(int fdSocketCliente, int cantIntentos, uint16_t largoPalabra, char** infoRestante);

int main(int argc, char* argv[]){
	
	if(argc < 4){
		printf("Error, debe enviar primero el puerto, la cantidad de intentos por partida y el archivo nombre del archivo para jugar el ahorcado.\n");
		return 0;
	}
	ControlaPartidas controlador;
	int cantIntentos = (int)strtol(argv[2], NULL, 10);
	controlaPartidasInicializar(&controlador, cantIntentos, argv[3]);
	socket_t socketServidor;
	int aux = socketInicializarServidorConBindYListen(&socketServidor, NULL, argv[1]);
	if(aux == ERROR)
		return 0;	
	char* restante;
	int16_t largoPalabra = (int16_t)controlaPartidasEmpezarNuevaPartida(&controlador, &restante);
	while(largoPalabra != SIN_PALABRAS){
		socket_t socketCliente;
		aux = socketAceptar(&socketServidor, &socketCliente);
		if(aux == ERROR)
			return 0;
		enviarMensaje(socketCliente.fd, cantIntentos, largoPalabra, &restante);
		char caracter;
		recv(socketCliente.fd, &caracter, sizeof(char), 0);
		int intentosRestantes = controlaPartidasJugarCaracter(&controlador, &caracter);
		while(intentosRestantes != VICTORIA && intentosRestantes != DERROTA){
			enviarMensaje(socketCliente.fd, intentosRestantes, largoPalabra, &restante);
			recv(socketCliente.fd, &caracter, sizeof(char), 0);
			intentosRestantes = controlaPartidasJugarCaracter(&controlador, &caracter);
		}
		enviarMensaje(socketCliente.fd, intentosRestantes, largoPalabra, &restante);
		socketDestruir(&socketCliente);
		largoPalabra = (int16_t)controlaPartidasEmpezarNuevaPartida(&controlador, &restante);
	}		
	controlaPartidasResumen(&controlador);
	controlaPartidasDestruir(&controlador);
	socketDestruir(&socketServidor);
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