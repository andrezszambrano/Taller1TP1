#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

typedef struct {
    int fd;
} socket_t;

int socketInicializarBindYListen(socket_t* socket, const char* host, const char* servicio);

void socketDestruir(socket_t* socket);

int socketAceptar(socket_t* socketServidor, socket_t* socketCliente);

void socketConectar(socket_t* socket, const char* host, const char* service);

ssize_t socketEnviar(socket_t* socket, const char* buffer, size_t length);

ssize_t socketRecibir(socket_t* socket, char* buffer, size_t length);

#endif