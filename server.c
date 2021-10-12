#include <stdlib.h>
#include "server.h"
#include "server_ahorcado.h"
#include "server_controla_partidas.h"
#include "server_comunicacion_con_protocolo.h"

#define ARGUMENTOS_REQUERIDOS 3
#define ERROR -1
#define EXITO 0
#define VICTORIA 1
#define DERROTA 2
#define SIN_PALABRAS 1


int servidorInicializar(Servidor* servidor, char* argv[], int cantArgumentos) {
	if (!servidor || cantArgumentos != ARGUMENTOS_REQUERIDOS)
		return ERROR;
	int cantIntentos = (int)strtol(argv[1], NULL, 10);
	controlaPartidasInicializar(&(servidor->controlaPartidas), cantIntentos,
								 argv[2]);
	int aux = socketInicializarServidorConBindYListen(
												&(servidor->socketServidor),
												 NULL, argv[0]);
	if (aux == ERROR){
		controlaPartidasDestruir(&(servidor->controlaPartidas));
		return ERROR;  
	}
	return EXITO;
}

int servidorProcesarLinea(Servidor* servidor) {
	if (!servidor)
		return ERROR;
	ControlaPartidas* controlador = &(servidor->controlaPartidas);
	socket_t* socketCliente = &(servidor->socketCliente);
	uint16_t largoPalabra = (uint16_t)controlaPartidasEmpezarNuevaPartida(
							controlador);
	if (largoPalabra == SIN_PALABRAS)
		return SIN_PALABRAS;
	int aux = socketAceptar(&(servidor->socketServidor), 
						 &(servidor->socketCliente));
	if (aux == ERROR)
		return ERROR;
	char* restante = controlaPartidasPalabraRestante(controlador);
	int cantIntentos = controlaPartidasIntentosPorPartida(controlador);
	char caracter = protocoloServidorEnviarInfoYRecibirCaracter(socketCliente,
														 cantIntentos,
														 largoPalabra,
														 &restante);
	if (caracter == ERROR)
		return ERROR;
	int intentosRestantes = controlaPartidasJugarCaracter(controlador, 
															 caracter);
	int estadoPartida = controlaPartidasActualizarYDarEstadoActualDePartida(
																controlador);
	restante = controlaPartidasPalabraRestante(controlador);
	while (estadoPartida != VICTORIA && estadoPartida != DERROTA){
		caracter = protocoloServidorEnviarInfoYRecibirCaracter(socketCliente,
														 intentosRestantes,
														 largoPalabra,
														 &restante);
		if (caracter == ERROR)
			return ERROR;
		intentosRestantes = controlaPartidasJugarCaracter(controlador, 
															 caracter);
		estadoPartida = controlaPartidasActualizarYDarEstadoActualDePartida(
																controlador);
		restante = controlaPartidasPalabraRestante(controlador);
	}
	aux = protocoloServidorEnviarMensajeFinDePartida(socketCliente, 
													  intentosRestantes,
													  largoPalabra,
													  &restante);
	socketDestruir(socketCliente);
	if (aux == ERROR)
		return ERROR;
	return EXITO;
}

void servidorMostrarResultados(Servidor* servidor) {
	if (!servidor)
		return;
	controlaPartidasResumen(&(servidor->controlaPartidas));
}

void servidorDestruir(Servidor* servidor) {
	if (!servidor)
		return;
	controlaPartidasDestruir(&(servidor->controlaPartidas));
	socketDestruir(&(servidor->socketServidor));
}
