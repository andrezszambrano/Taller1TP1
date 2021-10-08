#include <stdio.h>
#include <string.h>
#include "common_comunicacion_con_protocolo.h"

#define UN_BYTE 1
#define DOS_BYTES 2
#define ERROR -1
#define EXITO 0
#define MAX_PALABRA 25
#define VICTORIA 1
#define DERROTA 2
#define SIN_NOVEDADES 0
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
									 			char** infoRestante,
									 			int estadoPartida) {
	if (!socket)
		return ERROR;
	cantIntentos =  cantIntentos + 128; 
	return enviarInfoConProtocolo(socket, cantIntentos, largoPalabra, 
								   infoRestante);
}

void vaciarBuffer(){
	char aux = getchar();
	while(aux != '\n')
		aux = getchar();
}

void pedirCaracter(char* caracteres){
	char charAux = getchar();
	while(charAux == '\n')
		charAux = getchar();
	int i = 0;
	while(i < MAX_PALABRA && charAux != '\n'){
		if(97 <= charAux && charAux <= 122){//Caracter es válido
			caracteres[i] = charAux;
			i++;
		}
		charAux = getchar();
	}
	if(i == MAX_PALABRA && charAux != '\n')
		vaciarBuffer();
	caracteres[i] = '\0';	
}

int enviarCaracter(socket_t* socket, char* caracter){
	int aux = socketEnviar(socket, caracter, sizeof(int8_t));
	if(aux != UN_BYTE){
		printf("No se envió el byte correspondiente.\n");
		return ERROR;
	}
	return EXITO;
}

int protocoloClienteEnviarCaracteresYRecibirMensaje(socket_t* socket){
	char caracteres[MAX_PALABRA];
	pedirCaracter(caracteres);
	for(int i = 0; i < strlen(caracteres); i++){
		int aux = enviarCaracter(socket, caracteres + i);
		if(aux == ERROR){
			return ERROR;
		}
		aux = protocoloClienteRecibirMensaje(socket);
		if(aux == ERROR){
			return ERROR;
		}
		if(aux == VICTORIA || aux == DERROTA)
			return aux;
	}
	return EXITO;
}

void imprimirMensajeDeDerrota(char* restante, int lenPalabra){
	restante[lenPalabra] = '\0';
	printf("Perdiste! La palabra secreta era: '%s'\n", restante);
}

void imprimirMensajeDeVictoria(){
	printf("Ganaste!!\n");
}

void imprimirMensajeDelServidor(int numIntentos, char* restante,
								 int lenPalabra){
	printf("Palabra secreta: ");
	for(int i = 0; i < lenPalabra; i++)
		printf("%c", restante[i]);
	if(numIntentos < 128)
		printf("\nTe quedan %i intentos\n", numIntentos);
	
	printf("Ingrese letra:\n");
}

int protocoloClienteRecibirMensaje(socket_t* socket){
	char caracterConSigno;
	int aux = socketRecibir(socket, &caracterConSigno, sizeof(int8_t));
	if(aux != UN_BYTE){
		fprintf(stderr, "No se recibió el byte correspondiente.\n");
		return ERROR;
	}
	unsigned char caracter = (unsigned char) caracterConSigno;
	int numIntentos = caracter;
	char stringNum[2];
	aux = socketRecibir(socket, stringNum, 2);
	uint16_t lenPalabra = stringNum[0] | stringNum[1];
	if(aux != DOS_BYTES){
		fprintf(stderr, "No se recibieron los dos bytes correspondientes.\n");
		return ERROR;
	}
	char palabraRestante[MAX_PALABRA];
	aux = socketRecibir(socket, palabraRestante, lenPalabra);
	if(aux != lenPalabra){
		fprintf(stderr,"No se leyeron los n bytes correspondientes a la "
				"palabra.\n");
		return ERROR;
	}
	if(numIntentos == 128){//La partida acabó por derrota
		imprimirMensajeDeDerrota(palabraRestante, lenPalabra);
		return DERROTA;
	}else if (numIntentos > 128){//La partida terminó por victoria
		imprimirMensajeDeVictoria();
		return VICTORIA;
	}
	imprimirMensajeDelServidor(numIntentos, palabraRestante, lenPalabra);
	return SIN_NOVEDADES;
}
