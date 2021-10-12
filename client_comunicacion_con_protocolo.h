#ifndef __CLIENT_COMUNICACION_PROTOCOLO_H__
#define __CLIENT_COMUNICACION_PROTOCOLO_H__
	
#include "common_socket.h"


//El cliente recibe el mensaje del servidor. Sí falla la comunicación (no se 
//reciben los bytes correspondientes), se imprime el error en stderr y se 
//retorna -1. Si la partida terminó por derrota, se retorna 2, si terminó por
//victoria se regresa 1 y si no hay novedades se retorna 0. 
int protocoloClienteRecibirMensaje(socket_t* socket);

//Se envia al servidor uno o hasta 5 caracteres. Después se recibe el mensaje
//del servidor. En caso de error (ya sea al enviar o al recibir bytes), se 
//retorna -1. Si la partida terminó por derrota se retorna 2 y si terminó por
//victoria se retorna 1. En otro caso (no hubo error y la partida todavía no
//acaba), se retorna 0.
int protocoloClienteEnviarCaracteresYRecibirMensaje(socket_t* socket);

#endif
