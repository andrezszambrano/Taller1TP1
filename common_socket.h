#ifndef __CLIENT_SOCKET_H__
#define __CLIENT_SOCKET_H__

#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

typedef struct {
    int fd;
} socket_t;


//Se inicia un socket cliente, y se le conecta al servidor y al host indicado.
//En caso de éxito se regresa 0, en caso de error se regresa -1.
int socketInicializarYConectarCliente(socket_t* socketCliente, const char* host,
                                     const char* servicio);

//Se inicia un socket servidor, dado el servidor y host indicado. Se hace un 
//bind y un listen. En caso de éxito se regresa 0, en caso de error se 
//regresa -1.
int socketInicializarServidorConBindYListen(socket_t* socketServidor, 
                                            const char* host,
                                             const char* servicio);

//Se apaga (shutdown) la lectura y escriturael socket pasado por parámetro, 
//para después cerrarlo (close) liberando todos sus recursos.
void socketDestruir(socket_t* socket);

//El socket servidor (inicializado previamente con Bind y Listen) espera 
//para aceptar el socket cliente. Una vez se acepta un cliente, el parámetro
//socketCliente se modifica respectivamente. 
// En caso de éxito se regresa 0, en caso de error se regresa -1.
int socketAceptar(socket_t* socketServidor, socket_t* socketCliente);

//Se envia los primeros length bytes del buffer pasados por parámetro, al
//socket.  En caso de éxito se regresa la cantidad de bytes mandados (length),
//en caso de error se regresa -1.
ssize_t socketEnviar(socket_t* socket, char* buffer, size_t length);

//Se envian 16 bits (2 bytes) de información en caso de éxito, en caso de
//error se regresa -1.
//ssize_t socketEnviarShort(socket_t* socket, uint16_t numAEnviar);

//Se guardan length bytes en el buffer pasados por parámetro, al
//socket.  En caso de éxito se regresa la cantidad de bytes recibidos  (length),
//en caso de error se regresa -1.
ssize_t socketRecibir(socket_t* socket, char* buffer, size_t length);

//Se reciben 16 bits (2 bytes) de información en caso de éxito, en caso de
//error se regresa -1.
//ssize_t socketRecibirShort(socket_t* socket, uint16_t* numARecibir);

#endif
