#ifndef UTILS_CONNECTIONS_H_
#define UTILS_CONNECTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include <commons/log.h>
#include <commons/string.h>
#include <commons/collections/list.h>

#include <assert.h>

#define PUERTO "4444"
#define BACKLOG 100


typedef enum
{
	MENSAJE,
	PAQUETE,
	HANDSHAKE,
	ATENDER_PETICION_INTERFAZ_KERNEL
}op_code;

typedef struct
{
	int size;
	int offset;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

extern t_log* logger;

// FUNCIONES CLIENTE

void gestionar_conexion_como_cliente(char* ip, char* puerto, t_log* logger, const char* modulo);
int crear_conexion(char* ip, char* puerto, t_log* logger);
void enviar_mensaje(char* mensaje, int socket_cliente);
t_paquete* crear_paquete(void);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void liberar_conexion(int socket_cliente);
void eliminar_paquete(t_paquete* paquete);

void gestionar_handshake_como_cliente(int fd_conexion, const char* modulo_destino, t_log* logger);
void enviar_handshake(int conexion);

// FUNCIONES SERVIDOR
void* recibir_buffer(int*, int);

void gestionar_conexion_como_server(char* ip, char* puerto, t_log* logger, const char* modulo);
void gestionar_conexion_como_server_memoria(int fd_memoria, t_log* logger, const char* modulo);
int iniciar_servidor(char* puerto, t_log* logger, char* msj_server);
int esperar_cliente(int, t_log*, const char*);
t_list* recibir_paquete(int); 
void recibir_mensaje(int);
int recibir_operacion(int);

void gestionar_handshake_como_server(int conexion, t_log* logger);


// SERIALIZACION

t_buffer *buffer_create(uint32_t size);
void buffer_destroy(t_buffer *buffer);

void buffer_add(t_buffer *buffer, void *data, uint32_t size);
void buffer_add_uint32(t_buffer *buffer, uint32_t data);
void buffer_add_uint8(t_buffer *buffer, uint8_t data);
void buffer_add_string(t_buffer *buffer, uint32_t length, char *string);

// DESERIALIZACION

void buffer_read(t_buffer *buffer, void *data, uint32_t size);
uint32_t buffer_read_uint32(t_buffer *buffer);
uint8_t buffer_read_uint8(t_buffer *buffer);
char *buffer_read_string(t_buffer *buffer, uint32_t length);

t_paquete* deserializar_paquete(int unSocket);



#endif /* UTILS_CONNECTIONS_H_ */
