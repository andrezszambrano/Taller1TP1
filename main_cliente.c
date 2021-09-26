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

int socket_iniciarCliente(char* host , char* puerto);

void imprimirMensajeDelServidor(int numIntentos, char* restante, int lenPalabra);

int recibirMensaje(int fdSocketServidor);

void enviarMensaje(int fdSocketServidor);

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

	aux = recibirMensaje(fdSocketServidor);
	while(aux != VICTORIA && aux != DERROTA){	
		enviarMensaje(fdSocketServidor);
		aux = recibirMensaje(fdSocketServidor);
	}
	if(aux == VICTORIA)
		printf("Victoria!!!\n");
	else
		printf("Derrota \n");
	shutdown(fdSocketServidor, SHUT_RDWR);
	close(fdSocketServidor);
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

void enviarMensaje(int fdSocketServidor){
	char caracter;
	pedirCaracter(&caracter);
	int escritos = 0;
	while(escritos < 1){
		int aux = send(fdSocketServidor, &caracter, sizeof(char), MSG_NOSIGNAL);
		escritos = escritos + aux;
	}
}

int recibirMensaje(int fdSocketServidor){
	unsigned char caracter;
	int leidos = 0;
	while(leidos < 1){ 
		int aux = recv(fdSocketServidor, &caracter, sizeof(char) - leidos, 0);
		leidos = leidos + aux;
	}
	int numIntentos = caracter;
	uint16_t lenPalabraBE;
	leidos = 0;
	while(leidos < 2){ 
		int aux = recv(fdSocketServidor, &lenPalabraBE, 2*sizeof(char) - leidos, 0);
		leidos = leidos + aux;
	}
	int lenPalabra = ntohs(lenPalabraBE);	
	char palabraRestante[lenPalabra];
	leidos = 0;
	while(leidos < lenPalabra){
		int aux = recv(fdSocketServidor, palabraRestante + leidos, lenPalabra*sizeof(char) - leidos, 0);
		leidos = leidos + aux;
	}
	if(numIntentos == 128){//La partida acabó por derrota
		imprimirMensajeDeDerrota(palabraRestante);
		return DERROTA;
	}else if(numIntentos > 128){//La partida terminó por victoria
		imprimirMensajeDeVictoria();
		return VICTORIA;
	}
	imprimirMensajeDelServidor(numIntentos, palabraRestante, leidos);
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

int socket_iniciarCliente(char* host , char* puerto){
	struct addrinfo baseaddr;  
	struct addrinfo* ptraddr;
	memset(&baseaddr, 0, sizeof(struct addrinfo));
	baseaddr.ai_socktype = SOCK_STREAM;
	baseaddr.ai_family = AF_UNSPEC; //Ipv4 o Ipv6 
	baseaddr.ai_flags = 0;
	int aux = getaddrinfo(host, puerto, &baseaddr, &ptraddr);
	if(aux != EXITO){
		printf("Error al intentar obtener las direcciones\n");
		return ERROR;
	}
	int fdCliente = socket(ptraddr->ai_family, ptraddr->ai_socktype, ptraddr->ai_protocol);

	if(fdCliente == ERROR_SOCKET){
		printf("Error creando el socket del servidor\n");
		freeaddrinfo(ptraddr);
		return ERROR;
	}
	aux = connect(fdCliente, ptraddr->ai_addr, ptraddr->ai_addrlen);
    if (aux == -1) {
        printf("Error al conectarse al puerto\n");
        close(fdCliente);
        freeaddrinfo(ptraddr);
    	return ERROR;
    }
    freeaddrinfo(ptraddr);
	return fdCliente;
}