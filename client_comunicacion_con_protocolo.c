#include <stdio.h>
#include <string.h>
#include "client_comunicacion_con_protocolo.h"

#define UN_BYTE 1
#define DOS_BYTES 2
#define ERROR -1
#define EXITO 0
#define MAX_PALABRA 25
#define VICTORIA 1
#define DERROTA 2
#define SIN_NOVEDADES 0
#define VALOR_BIT_MAS_SIGNIFICATIVO 128

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
		if(97 <= charAux && charAux <= 122){//Caracter es válido <-- en ese caso podrías poner una función caracterEsValido(char)
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
		fprintf(stderr, "No se envió el byte correspondiente.\n");
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
	for (int i = 0; i < lenPalabra; i++)
		printf("%c", restante[i]);
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

	// Por qué no aplicar la máscara antes y preguntar si numIntentos == 0? Es más legible
	if(numIntentos == VALOR_BIT_MAS_SIGNIFICATIVO){//La partida acabó por 
	//derrota
		imprimirMensajeDeDerrota(palabraRestante, lenPalabra);
		return DERROTA;
	}else if (numIntentos > VALOR_BIT_MAS_SIGNIFICATIVO){//La partida terminó
	//por victoria
		imprimirMensajeDeVictoria();
		return VICTORIA;
	}
	imprimirMensajeDelServidor(numIntentos, palabraRestante, lenPalabra);
	return SIN_NOVEDADES;
}
