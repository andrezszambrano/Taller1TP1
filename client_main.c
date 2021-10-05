#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "common_socket.h"
#define EXITO 0
#define ERROR_SOCKET -1
#define MAX_QUEUE 1
#define ERROR -1
#define VICTORIA 2
#define DERROTA 1
#define SIN_NOVEDADES 0
#define UN_BYTE 1
#define DOS_BYTES 2
#define MAX_PALABRA 25

void vaciarBuffer();

void imprimirMensajeDelServidor(int numIntentos, char* restante, 
								int lenPalabra);

int enviarCaracteresYRecibirMensaje(socket_t* socket);

int recibirMensaje(socket_t* socket);

int enviarCaracter(socket_t* socket, char* caracter);

void pedirCaracter(char* caracter);

void imprimirMensajeDeDerrota(char* restante, int lenPalabra);

void imprimirMensajeDeVictoria();

// Toda la lógica de este archivo fuente es global. Hay que asignar responsabilidades
// a los TDAs correspondientes y modularizar.

int main(int argc, char* argv[]) {
	if (argc < 3) {
		// estaría mejor imprimir esto en stderr y retornar algún código de error.
		printf("Error, debe enviar primero el host y después el número" 
				"de puerto a conectarse.\n");
		return 0;
	}
	socket_t socketServidor;
	int aux = socketInicializarYConectarCliente(&socketServidor, argv[1], 
												argv[2]);
	if(aux == ERROR)
		return 0;

	aux = recibirMensaje(&socketServidor);
	if(aux == ERROR){
		socketDestruir(&socketServidor);
		return 0;
	}
	while(aux != VICTORIA && aux != DERROTA){
		aux = enviarCaracteresYRecibirMensaje(&socketServidor);
		if(aux == ERROR){
			socketDestruir(&socketServidor);
			return 0;
		}
	}
	socketDestruir(&socketServidor);	
	return 0;
}

void vaciarBuffer(){
	char aux = getchar();
	while (aux != '\n')
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

int enviarCaracteresYRecibirMensaje(socket_t* socket){
	char caracteres[MAX_PALABRA];
	pedirCaracter(caracteres);
	for (int i = 0; i < strlen(caracteres); i++) {
		int aux = enviarCaracter(socket, caracteres + i);
		if (aux == ERROR) {
			return ERROR;
		}
		aux = recibirMensaje(socket);
		if (aux == ERROR) {
			return ERROR;
		}
		if (aux == VICTORIA || aux == DERROTA)
			return aux;
	}
	return EXITO;
}

int enviarCaracter(socket_t* socket, char* caracter){
	int aux = socketEnviar(socket, caracter, sizeof(int8_t));
	if (aux != UN_BYTE) {
		printf("No se envió el byte correspondiente.\n");
		return ERROR;
	}
	return EXITO;
}

int recibirMensaje(socket_t* socket){
	char caracterConSigno;
	int aux = socketRecibir(socket, &caracterConSigno, sizeof(int8_t));
	if(aux != UN_BYTE){
		printf("No se recibió el byte correspondiente.\n");
		return ERROR;
	}

	// esto es un bug, lo que dice el protocolo es que se precede el caracter con
	// un bit en 1, no que se le cambia el signo. (Pista: Hay que usar un operador & )
	unsigned char caracter = (unsigned char) caracterConSigno;
	int numIntentos = caracter;
	char stringNum[2];
	aux = socketRecibir(socket, stringNum, 2);
	uint16_t lenPalabra = stringNum[0] | stringNum[1];
	if (aux != DOS_BYTES) {
		printf("No se recibieron los dos bytes correspondientes.\n");
		return ERROR;
	}
	char palabraRestante[MAX_PALABRA];
	aux = socketRecibir(socket, palabraRestante, lenPalabra);
	if (aux != lenPalabra) {
		printf("No se leyeron los n bytes correspondientes a la palabra.\n");
		return ERROR;
	}

	// número mágico!
	if (numIntentos == 128) {//La partida acabó por derrota
		imprimirMensajeDeDerrota(palabraRestante, lenPalabra);
		return DERROTA;
	}else if (numIntentos > 128){//La partida terminó por victoria
		imprimirMensajeDeVictoria();
		return VICTORIA;
	}
	imprimirMensajeDelServidor(numIntentos, palabraRestante, lenPalabra);
	return SIN_NOVEDADES;
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
