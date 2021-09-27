#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "socket.h"
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

void imprimirMensajeDeDerrota(char* restante);

void imprimirMensajeDeVictoria();

int main(int argc, char* argv[]){
	
	if(argc < 3){
		printf("Error, debe enviar primero el host y después el número 
				de puerto a conectarse.\n");
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
		//aux = enviarMensaje(&socketServidor);
		//aux = recibirMensaje(&socketServidor);
	}
	if(aux == VICTORIA)
		printf("Victoria!!!\n");
	else
		printf("Derrota \n");
	socketDestruir(&socketServidor);	
	return 0;
}

void vaciarBuffer(){
	char aux = getchar();
	while(aux != '\n')
		aux = getchar();
}

void pedirCaracter(char* caracteres){
	char charAux = getchar();
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
	int aux;
	for(int i = 0; i < strlen(caracteres); i++){
		aux = enviarCaracter(socket, caracteres + i);
		if(aux == ERROR){

		}
		aux = recibirMensaje(socket);
		if(aux == ERROR){

		}
		if(aux == VICTORIA || aux == DERROTA)
			return aux;
	}
	return EXITO;
}

int enviarCaracter(socket_t* socket, char* caracter){
	int aux = socketEnviar(socket, caracter, sizeof(int8_t));
	if(aux != UN_BYTE){
		printf("No se envió el byte correspondiente.\n");
		return ERROR;
	}
	return EXITO;
}

int recibirMensaje(socket_t* socket){
	unsigned char caracter;
	int aux = socketRecibir(socket, &caracter, sizeof(int8_t));
	if(aux == ERROR){
		printf("No se recibió el byte correspondiente.\n");
		return ERROR;
	}
	int numIntentos = caracter;
	int lenPalabra;
	aux = socketRecibirShort(socket, &lenPalabra);
	if(aux != DOS_BYTES){
		printf("No se recibieron los dos bytes correspondientes.\n");
		return ERROR;
	}
	char palabraRestante[lenPalabra];
	aux = socketRecibir(socket, palabraRestante, lenPalabra);
	if(aux != lenPalabra){
		printf("No se leyeron los n bytes correspondientes a la palabra.\n");
		return ERROR;
	}
	if(numIntentos == 128){//La partida acabó por derrota
		imprimirMensajeDeDerrota(palabraRestante);
		return DERROTA;
	}else if(numIntentos > 128){//La partida terminó por victoria
		imprimirMensajeDeVictoria();
		return VICTORIA;
	}
	imprimirMensajeDelServidor(numIntentos, palabraRestante, lenPalabra);
	return SIN_NOVEDADES;
}

void imprimirMensajeDeDerrota(char* restante){
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
