#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

typedef struct {
    int fd;
} socket_t;

int socketInicializarYConectarCliente(socket_t* socketCliente, const char* host, const char* servicio);

int socketInicializarServidorConBindYListen(socket_t* socketServidor, const char* host, const char* servicio);

void socketDestruir(socket_t* socket);

int socketAceptar(socket_t* socketServidor, socket_t* socketCliente);

ssize_t socketEnviar(socket_t* socket, void* buffer, size_t length);

ssize_t socketEnviarShort(socket_t* socket, int numAEnviar);

ssize_t socketRecibir(socket_t* socket, void* buffer, size_t length);

ssize_t socketRecibirShort(socket_t* socket, int* numARecibir);

#endif