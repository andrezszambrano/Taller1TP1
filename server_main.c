#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include "server_ahorcado.h"
#include "server_controla_partidas.h"
#include "common_socket.h"
#include <inttypes.h>
#define EXITO 0
#define ERROR -1
#define SIN_PALABRAS -2
#define VICTORIA 150
#define DERROTA -1
#define UN_BYTE 1
#define DOS_BYTES 2

char cicloEnviarInfoYRecibirCaracter(socket_t* socket, int cantIntentos, 
									uint16_t largoPalabra, char** infoRestante,
									int cantIntentosAnterior);

int enviarInfoConProtocolo(socket_t* socket, int cantIntentos, 
							uint16_t largoPalabra, char** infoRestante,
							int cantIntentosAnterior);

int enviarFlagTerminaPartidasEIntentosRestantes(socket_t* socket, 
												int cantIntentos, 
												int cantIntentosAnterior);

int enviarLargoDePalabra(socket_t* socket, uint16_t largoPalabra);

int enviarPalabraRestante(socket_t* socket, int largoPalabra,
						 char** infoRestante);

int main(int argc, char* argv[]){
	if(argc < 4){
		printf("Error, debe enviar primero el puerto, la cantidad de intentos"
		"por partida y el archivo nombre del archivo para jugar el"
		" ahorcado.\n");
		return 0;
	}
	ControlaPartidas controlador;
	int cantIntentos = (int)strtol(argv[2], NULL, 10);
	controlaPartidasInicializar(&controlador, cantIntentos, argv[3]);
	socket_t socketServidor;
	int aux = socketInicializarServidorConBindYListen(&socketServidor, NULL, 
														argv[1]);
	if(aux == ERROR)
		return 0;	
	char* restante;
	int16_t largoPalabra = (int16_t)controlaPartidasEmpezarNuevaPartida(
							&controlador, &restante);
	while(largoPalabra != SIN_PALABRAS){
		socket_t socketCliente;
		aux = socketAceptar(&socketServidor, &socketCliente);
		if(aux == ERROR)
			return 0;	
		int cantIntentosAnterior = cantIntentos;	
		char caracter = cicloEnviarInfoYRecibirCaracter(&socketCliente, 
														 cantIntentos, 
														 (uint16_t)largoPalabra, 
														 &restante, 
														 cantIntentosAnterior);
		if(caracter == ERROR){
			controlaPartidasDestruir(&controlador);
			socketDestruir(&socketCliente);
			socketDestruir(&socketServidor);
			return 0;
		}
		int intentosRestantes = controlaPartidasJugarCaracter(&controlador, 
																caracter);
		while(intentosRestantes != VICTORIA && intentosRestantes != DERROTA){
			caracter = cicloEnviarInfoYRecibirCaracter(&socketCliente, 
														intentosRestantes,
														(uint16_t)largoPalabra,
														&restante,
														cantIntentosAnterior);
			if(caracter == ERROR){
				controlaPartidasDestruir(&controlador);
				socketDestruir(&socketCliente);
				socketDestruir(&socketServidor);
				return 0;
			}
			cantIntentosAnterior = intentosRestantes;
			intentosRestantes = controlaPartidasJugarCaracter(&controlador, 
																caracter);
		}
		aux = enviarInfoConProtocolo(&socketCliente, intentosRestantes, 
									(uint16_t)largoPalabra, &restante,
									cantIntentosAnterior);
		if(aux == ERROR){
			controlaPartidasDestruir(&controlador);
			socketDestruir(&socketCliente);
			socketDestruir(&socketServidor);
			return 0;
		}
		socketDestruir(&socketCliente);
		largoPalabra = (int16_t)controlaPartidasEmpezarNuevaPartida(
																&controlador, 
																&restante);
	}		
	controlaPartidasResumen(&controlador);
	controlaPartidasDestruir(&controlador);
	socketDestruir(&socketServidor);
	return 0;
}

char cicloEnviarInfoYRecibirCaracter(socket_t* socket, int cantIntentos, 
									uint16_t largoPalabra, char** infoRestante,
									int cantIntentosAnterior){
	int aux = enviarInfoConProtocolo(socket, cantIntentos, largoPalabra,
									 infoRestante, cantIntentosAnterior);
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

int enviarInfoConProtocolo(socket_t* socket, int cantIntentos, 
							uint16_t largoPalabra, char** infoRestante,
							int cantIntentosAnterior){
	int aux = enviarFlagTerminaPartidasEIntentosRestantes(socket, cantIntentos,
														cantIntentosAnterior);
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
		printf("Error: No se enviaron los n bytes correspondientes al" 
				"largo de la palabra.\n");
		return ERROR;
	}
	return EXITO;
}

int enviarFlagTerminaPartidasEIntentosRestantes(socket_t* socket, 
												int cantIntentos,
												int cantIntentosAnterior){
	if(cantIntentos == VICTORIA)
		cantIntentos = cantIntentosAnterior + 128;
	else if (cantIntentos == DERROTA)
		cantIntentos = cantIntentos + 128;
	char caracter = cantIntentos; 
	return socketEnviar(socket, &caracter, sizeof(int8_t));
}

int enviarLargoDePalabra(socket_t* socket, uint16_t largoPalabra){
	char stringNum[2];
	stringNum[1] = largoPalabra & 0xFF; //Byte más significativo primero
	stringNum[0] = largoPalabra >> 8;   //Byte menos significativo segundo
	//snprintf(stringNum, sizeof(uint16_t),"%"PRIu16, largoPalabra);
	return socketEnviar(socket, stringNum, 2);
}

int enviarPalabraRestante(socket_t* socket, int largoPalabra, 
							char** infoRestante){
	return socketEnviar(socket, *infoRestante, largoPalabra);
}
