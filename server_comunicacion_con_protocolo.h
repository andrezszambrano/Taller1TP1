#ifndef __SERVER_COMUNICACION_PROTOCOLO_H__
#define __SERVER_COMUNICACION_PROTOCOLO_H__

#include "common_socket.h"


//Se envían los bytes siguiendo el protocolo: el primer bit del primer byte
//estará en 1 sí la partida acabó. El resto de los 7 bits contienen la cantidad
//de intentos restantes. Los 2 bytes siguientes contienen el largo de la
//palabra a enviar. Por último se envían los bytes correspondientes a la
//palabra restante. Si hubo un error se imprime por stderr y se retorna -1.
//En caso de éxito se retorna el carácter recibido del cliente.
char protocoloServidorEnviarInfoYRecibirCaracter(socket_t* socket, 
												  int cantIntentos, 
												  uint16_t largoPalabra,
												  char** infoRestante);

//Se envían los bytes de la misma forma que la primera función, solo que se 
//añade modifica el bit más significativo del primer byte para enviar que la
//partida ya finalizó.
int protocoloServidorEnviarMensajeFinDePartida(socket_t* socket, 
												int cantIntentos, 
									 			uint16_t largoPalabra, 
									 			char** infoRestante);

#endif
