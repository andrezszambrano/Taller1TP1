#define _POSIX_C_SOURCE 200112L
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

void imprimirMensajeDelServidor(int numIntentos, char* restante, int lenPalabra);

int recibirMensaje(socket_t* socket);

int enviarMensaje(socket_t* socket);

void pedirCaracter(char* caracter);

void imprimirMensajeDeDerrota(char* restante);

void imprimirMensajeDeVictoria();

int main(int argc, char* argv[]){
	
	if(argc < 3){
		printf("Error, debe enviar primero el host y después el número de puerto a conectarse.\n");
		return 0;
	}
	socket_t socketServidor;
	int aux = socketInicializarYConectarCliente(&socketServidor, argv[1], argv[2]);
	if(aux == ERROR)
		return 0;

	aux = recibirMensaje(&socketServidor);
	while(aux != VICTORIA && aux != DERROTA){	
		aux = enviarMensaje(&socketServidor);
		aux = recibirMensaje(&socketServidor);
	}
	if(aux == VICTORIA)
		printf("Victoria!!!\n");
	else
		printf("Derrota \n");
	socketDestruir(&socketServidor);	
	return 0;
}

void pedirCaracter(char* caracter){
	*caracter = getchar();
	getchar();
	//int i = 0;
	//char aux = getchar();
	//while(i < MAX_CARACTERES && aux != '\n'){
	//	caracter[i] = aux;
	//	aux = getchar();
	//	i++;
	//}
	//if(i == MAX_CARACTERES && aux != '\n')
	//	vaciarBuffer();
	//caracter[i] = '\0';	

}

int enviarMensaje(socket_t* socket){
	char caracter;
	pedirCaracter(&caracter);
	int aux = socketEnviar(socket, &caracter, sizeof(int8_t));
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

void imprimirMensajeDelServidor(int numIntentos, char* restante, int lenPalabra){
	printf("Palabra secreta: ");
	for(int i = 0; i < lenPalabra; i++)
		printf("%c", restante[i]);
	if(numIntentos < 128)
		printf("\nTe quedan %i intentos\n", numIntentos);
	
	printf("Ingrese letra:\n");

}
