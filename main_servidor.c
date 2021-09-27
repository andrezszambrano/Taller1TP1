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
#define UN_BYTE 1
#define DOS_BYTES 2

char cicloEnviarInfoYRecibirCaracter(socket_t* socket, int cantIntentos, uint16_t largoPalabra, char** infoRestante);

int enviarInfoConProtocolo(socket_t* socket, int cantIntentos, uint16_t largoPalabra, char** infoRestante);

int enviarFlagTerminaPartidasEIntentosRestantes(socket_t* socket, int cantIntentos);

int enviarLargoDePalabra(socket_t* socket, int largoPalabra);

int enviarPalabraRestante(socket_t* socket, int largoPalabra, char** infoRestante);

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
		
		char caracter = cicloEnviarInfoYRecibirCaracter(&socketCliente, cantIntentos, largoPalabra, &restante);
		int intentosRestantes = controlaPartidasJugarCaracter(&controlador, caracter);
		while(intentosRestantes != VICTORIA && intentosRestantes != DERROTA){
			char caracter = cicloEnviarInfoYRecibirCaracter(&socketCliente, intentosRestantes, largoPalabra, &restante);
			intentosRestantes = controlaPartidasJugarCaracter(&controlador, caracter);
		}
		enviarInfoConProtocolo(&socketCliente, intentosRestantes, largoPalabra, &restante);
		socketDestruir(&socketCliente);
		largoPalabra = (int16_t)controlaPartidasEmpezarNuevaPartida(&controlador, &restante);
	}		
	controlaPartidasResumen(&controlador);
	controlaPartidasDestruir(&controlador);
	socketDestruir(&socketServidor);
	return 0;
}

char cicloEnviarInfoYRecibirCaracter(socket_t* socket, int cantIntentos, uint16_t largoPalabra, char** infoRestante){
	int aux = enviarInfoConProtocolo(socket, cantIntentos, largoPalabra, infoRestante);
	if(aux == ERROR)
		return ERROR;
	char caracter;
	aux = socketRecibir(socket, &caracter, sizeof(int8_t));
	if(aux != UN_BYTE){
		printf("Error: no se recibió el byte correspondiente.\n");
		return ERROR;
	}
	return caracter;
}

int enviarInfoConProtocolo(socket_t* socket, int cantIntentos, uint16_t largoPalabra, char** infoRestante){
	int aux = enviarFlagTerminaPartidasEIntentosRestantes(socket, cantIntentos);
	if(aux != UN_BYTE){
		printf("Error: No se envió el primer byte.\n");
		return ERROR;
	}
	aux = enviarLargoDePalabra(socket, largoPalabra);
	if(aux != DOS_BYTES){
		printf("Error: No se enviaron los dos bytes correspondientes.\n");
		return ERROR;
	}
	aux = enviarPalabraRestante(socket, largoPalabra, infoRestante);
	if(aux != largoPalabra){
		printf("Error: No se enviaron los n bytes correspondientes al largo de la palabra.\n");
	}
	return EXITO;
}

int enviarFlagTerminaPartidasEIntentosRestantes(socket_t* socket, int cantIntentos){
	if(cantIntentos == DERROTA)
		cantIntentos = 128;
	unsigned char caracter = cantIntentos;
	return socketEnviar(socket, &caracter, sizeof(int8_t));
}

int enviarLargoDePalabra(socket_t* socket, int largoPalabra){
	return socketEnviarShort(socket, largoPalabra);
}

int enviarPalabraRestante(socket_t* socket, int largoPalabra, char** infoRestante){
	return socketEnviar(socket, *infoRestante, largoPalabra);
}