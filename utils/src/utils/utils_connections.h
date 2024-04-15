#ifndef UTILS_CONNECTIONS_H_
#define UTILS_CONNECTIONS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>

#include<commons/log.h>
#include<commons/string.h>
#include<commons/collections/list.h>

#include<assert.h>

#define PUERTO "4444"

typedef enum
{
	MENSAJE,
	PAQUETE
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

extern t_log* logger;

// FUNCIONES CLIENTE

int crear_conexion(char* ip, int puerto);
void enviar_mensaje(char* mensaje, int socket_cliente);
t_paquete* crear_paquete(void);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void liberar_conexion(int socket_cliente);
void eliminar_paquete(t_paquete* paquete);


// FUNCIONES SERVIDOR
void* recibir_buffer(int*, int);

int iniciar_servidor(int puerto, t_log* logger, char* msj_server);
int esperar_cliente(int, t_log*, char*);
t_list* recibir_paquete(int); 
void recibir_mensaje(int);
int recibir_operacion(int);


#endif /* UTILS_CONNECTIONS_H_ */