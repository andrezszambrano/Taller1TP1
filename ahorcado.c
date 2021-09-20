#include "ahorcado.h"
#include <string.h>
#include <stdbool.h>

#define SIN_NOVEDADES 0
#define SIN_INTENTOS 1
#define VICTORIA 2
void ahorcadoInicializar(Ahorcado* ahorcado, char* palabra, int numIntentos){
	if(ahorcado == NULL)
		return;
	ahorcado->numIntentos = numIntentos;
	strncpy(ahorcado->palabraAAdivinar, palabra, strlen(palabra)*sizeof(char));
	ahorcado->palabraAAdivinar[strlen(palabra)-1]='\0';
	for(int i = 0; i < strlen(ahorcado->palabraAAdivinar); i++)
		ahorcado->restante[i] = '_';
	ahorcado->restante[strlen(ahorcado->palabraAAdivinar)]='\0';
}

void validarLetra(Ahorcado* ahorcado,char letra){
	bool letraIncorrecta = true;
	int len = strlen(ahorcado->palabraAAdivinar);
	for(int i = 0; i < len; i++){
		if(ahorcado->palabraAAdivinar[i] == letra){
			ahorcado->restante[i] = letra;
			letraIncorrecta = false;
		}
	}
	if(letraIncorrecta){
		ahorcado->numIntentos--;
	}
}

int letrasRestantes(Ahorcado* ahorcado){
	int len = strlen(ahorcado->palabraAAdivinar);
	int contador = 0;
	for(int i = 0; i < len; i++){
		if(ahorcado->restante[i] == '_')
			contador++;
	}
	return contador;
}

int estadoDePartida(Ahorcado* ahorcado){
	if (ahorcado->numIntentos == 0)
		return SIN_INTENTOS;
	else if (letrasRestantes(ahorcado) == 0)
		return VICTORIA;
	else
		return SIN_NOVEDADES;

}

int hacerJugada(Ahorcado* ahorcado){
	printf("Palabra secreta: %s\n", ahorcado->restante);
	printf("Te quedan %i intentos\n", ahorcado->numIntentos);
	printf("Ingrese letra:\n");
	char letraRecibida = getchar();
	getchar();
	validarLetra(ahorcado, letraRecibida);
	return estadoDePartida(ahorcado);
}

int ahorcadoJugar(Ahorcado* ahorcado){
	printf("Tenemos la palabra '%s' con la cantidad de intentos %i\n", ahorcado->palabraAAdivinar, ahorcado->numIntentos);
	printf("Palabra por adivinar: %s\n", ahorcado->restante);
	int retJugar = hacerJugada(ahorcado);
	while(retJugar == 0){
		retJugar = hacerJugada(ahorcado);
	}
	if (retJugar == VICTORIA){
		printf("Ganaste!!\n");
	}else{
		printf("Perdiste! La palabra secreta era: %s\n",ahorcado->palabraAAdivinar);
	}
	return retJugar;
}







