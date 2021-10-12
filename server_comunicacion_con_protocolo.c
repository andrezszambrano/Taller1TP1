#include <stdio.h>
#include <string.h>
#include "server_comunicacion_con_protocolo.h"

#define UN_BYTE 1
#define DOS_BYTES 2
#define ERROR -1
#define EXITO 0
#define MAX_PALABRA 25
#define VALOR_BIT_MAS_SIGNIFICATIVO 128

int enviarFlagTerminaPartidasEIntentosRestantes(socket_t* socket, 
												int cantIntentos) {
	char caracter = cantIntentos; 
	return socketEnviar(socket, &caracter, sizeof(int8_t));
}

int enviarLargoDePalabra(socket_t* socket, uint16_t largoPalabra) {
	char stringNum[2];
	stringNum[1] = largoPalabra & 0xFF; //Byte más significativo primero
	stringNum[0] = largoPalabra >> 8;   //Byte menos significativo segundo
	return socketEnviar(socket, stringNum, 2);
}

int enviarPalabraRestante(socket_t* socket, int largoPalabra, 
							char** infoRestante) {
	return socketEnviar(socket, *infoRestante, largoPalabra);
}

int enviarInfoConProtocolo(socket_t* socket, int cantIntentos, 
							uint16_t largoPalabra, char** infoRestante) {
	int aux = enviarFlagTerminaPartidasEIntentosRestantes(socket, cantIntentos);
	if (aux != UN_BYTE){
		fprintf(stderr, "Error: No se envió el primer byte.\n");
		return ERROR;
	}
	aux = enviarLargoDePalabra(socket, largoPalabra);
	if (aux != DOS_BYTES){
		fprintf(stderr, "Error: No se enviaron los dos bytes "
			"correspondientes.\n");
		return ERROR;
	}
	aux = enviarPalabraRestante(socket, largoPalabra, infoRestante);
	if (aux != largoPalabra){
		fprintf(stderr, "Error: No se enviaron los n bytes correspondientes al" 
				"largo de la palabra.\n");
		return ERROR;
	}
	return EXITO;
}

char protocoloServidorEnviarInfoYRecibirCaracter(socket_t* socket, 
												 int cantIntentos, 
									        	 uint16_t largoPalabra, 
									      		 char** infoRestante) {
	if (!socket)
		return ERROR;
	int aux = enviarInfoConProtocolo(socket, cantIntentos, largoPalabra,
									 infoRestante);
	if (aux == ERROR)
		return ERROR;
	char caracter;
	aux = socketRecibir(socket, &caracter, sizeof(int8_t));
	if (aux != UN_BYTE){
		fprintf(stderr, "Error: no se recibió el byte correspondiente.\n");
		return ERROR;
	}
	return caracter;
}

int protocoloServidorEnviarMensajeFinDePartida(socket_t* socket, 
												int cantIntentos, 
									 			uint16_t largoPalabra,
									 			char** infoRestante) {
	if (!socket)
		return ERROR;
	cantIntentos =  cantIntentos + VALOR_BIT_MAS_SIGNIFICATIVO; 
	return enviarInfoConProtocolo(socket, cantIntentos, largoPalabra, 
								   infoRestante);
}
