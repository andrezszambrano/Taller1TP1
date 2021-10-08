#ifndef __SERVER_COMUNICACION_PROTOCOLO_H__
#define __SERVER_COMUNICACION_PROTOCOLO_H__

#include "common_socket.h"

char protocoloServidorEnviarInfoYRecibirCaracter(socket_t* socket, 
												  int cantIntentos, 
												  uint16_t largoPalabra,
												  char** infoRestante);

int protocoloServidorEnviarMensajeFinDePartida(socket_t* socket, 
												int cantIntentos, 
									 			uint16_t largoPalabra, 
									 			char** infoRestante,
									 			int estadoPartida);

int protocoloClienteRecibirMensaje(socket_t* socket);

int protocoloClienteEnviarCaracteresYRecibirMensaje(socket_t* socket);

#endif
