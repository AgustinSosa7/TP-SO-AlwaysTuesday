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
#include <commons/collections/queue.h>
#include <commons/temporal.h>
#include <commons/config.h>
#include <semaphore.h>

#include <pthread.h> 

#include <assert.h>
#include <readline/readline.h>


#define PUERTO "4444"
#define BACKLOG 100


typedef enum
{
	MENSAJE,
	PAQUETE,
	HANDSHAKE,
    //---- KERNEL - IO
    ME_IDENTIFICO,
	ATENDER_PETICION_INTERFAZ_KERNEL,
	ESTOY_CONECTADO,
    FIN_PETICION,
	//---- KERNEL - CPU
	PCB,
	EJECUTAR_PROCESO_KC,
    SOLICITUD_INTERRUMPIR_PROCESO,
	//---- KERNEL - MEMORIA
	CREAR_PROCESO,
    FINALIZAR_PROCESO_MEMORIA,
  DESALOJO_QUANTUM,
  PROCESO_EXIT,
  PEDIDO_IO,
  WAIT,
  SIGNAL,
  // ------ IO - MEMORIA
  GUARDAR_REGISTRO,
  PEDIR_REGISTRO,
  // ------ CPU - MEMORIA
  SOLICITUD_INFO_INICIAL_A_MEMORIA,
  RESPUESTA_INFO_INICIAL_A_CPU,
  SOLICITUD_MODIFICAR_TAMANIO,
  RESPUESTA_MODIFICAR_TAMANIO,
  PEDIDO_PSEUDOCODIGO,
  PSEUDOCODIGO,
  SOLICITUD_NUMERO_DE_MARCO_A_MEMORIA,
  RESPUESTA_NUMERO_DE_MARCO_A_CPU,
  SOLICITUD_LEER_VALOR_EN_MEMORIA,
  RESPUESTA_LEER_VALOR_EN_MEMORIA,
  SOLICITUD_ESCRIBIR_VALOR_EN_MEMORIA,
  RESPUESTA_ESCRIBIR_VALOR_EN_MEMORIA,
  DEVOLVER_PROCESO_POR_PAGEFAULT,
  DEVOLVER_PROCESO_POR_OUT_OF_MEMORY,
  DEVOLVER_PROCESO_POR_CORRECTA_FINALIZACION
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

typedef struct //Verificar tipo de datos
{
    u_int32_t PC;
    u_int8_t AX;
    u_int8_t BX;
    u_int8_t CX;
    u_int8_t DX;
    u_int32_t EAX;
    u_int32_t EBX;
    u_int32_t ECX;
    u_int32_t EDX;
    u_int32_t SI;
    u_int32_t DI;
}t_registros_cpu;



typedef enum{
    NEW,
    READY,
    EXEC,
    BLOCKED,
    EXIT,
    READYPLUS,
} estado_pcb;

typedef struct{
    int pid;
    int quantum;
    t_registros_cpu* registros_cpu;
    estado_pcb estado_pcb;
} t_pcb;

typedef enum{
    SUCCESS,
    INVALID_RESOURCE,
    INVALID_INTERFACE,
    OUT_OF_MEMORY, 
    INTERRUPTED_BY_USER,
} motivo_fin_de_proceso;

typedef enum{
    INTERRUPCION_POR_DESALOJO,
    INTERRUPCION_POR_KILL
} motivo_de_interrupcion;


extern t_log* logger;




/////////SEMAFOROS////////
// Funciones de Listas

bool contains_string(t_list* lista, char* elemento);
bool son_el_mismo_string(char* elemento_posible, char* elemento);
bool contains_algo(t_list* lista, void* elemento);
bool son_lo_mismo(void* elemento_posible, void* elemento);

/////////////////

bool validar_parametros_incorrectos_main(int argc, int parametros_esperados, char** argv);

// FUNCIONES CLIENTE
int crear_conexion(char* ip, char* puerto, t_log* logger);
void gestionar_handshake_como_cliente(int fd_conexion, const char* modulo_destino, t_log* logger);
void enviar_handshake(int conexion);
void liberar_conexion(int socket_cliente);


// FUNCIONES SERVIDOR
int iniciar_servidor(char* puerto, t_log* logger, char* msj_server);
int esperar_cliente(int, t_log*, const char*);
void gestionar_handshake_como_server(int conexion, t_log* logger, const char* modulo_destino);


// SERIALIZACION
void crear_buffer(t_paquete* paquete);
void eliminar_buffer(t_buffer *buffer);
t_paquete* crear_paquete(op_code code_op);
void* serializar_paquete(t_paquete* paquete, int bytes);
void enviar_mensaje(void* mensaje, int socket_cliente);
void enviar_bool_mensaje(bool mensaje, int socket_cliente);
void enviar_int_mensaje(int mensaje, int socket_cliente);
void enviar_mensaje_string(char* mensaje, int socket_cliente);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void eliminar_paquete(t_paquete* paquete);
void agregar_int_a_paquete(t_paquete* paquete, int valor);
void agregar_uint8_a_paquete(t_paquete* paquete, uint8_t valor);
void agregar_uint32_a_paquete(t_paquete* paquete, uint32_t valor);
void agregar_string_a_paquete(t_paquete* paquete, char* valor);
void agregar_registro_a_paquete(t_paquete* paquete, t_registros_cpu* registros_CPU);

//PCB
void agregar_pcb_a_paquete(t_pcb* un_pcb, t_paquete* un_paquete);
void enviar_pcb_a(t_pcb* un_pcb, int socket, op_code mensaje);
t_pcb* recibir_pcb(t_paquete* paquete);
void imprimir_pcb(t_pcb* un_pcb,t_log* un_logger);
char* enum_a_string(estado_pcb estado);

// DESERIALIZACION
void* recibir_mensaje(int socket_cliente);
bool recibir_bool_mensaje(int socket_cliente);
int recibir_int_mensaje(int socket_cliente);
char* recibir_mensaje_string(int socket_cliente);
op_code recibir_operacion(int);
t_buffer* recibir_buffer(int unSocket);
t_paquete* recibir_paquete(int unSocket);
int leer_int_del_buffer(t_buffer* buffer);
uint8_t leer_uint_8_del_buffer(t_buffer* buffer);
uint32_t leer_uint_32_del_buffer(t_buffer* buffer);
char* leer_string_del_buffer(t_buffer* buffer);
void leer_registros_del_buffer(t_buffer* buffer, t_registros_cpu* registros_CPU);

//Funciones Superpaquete
//t_paquete* crear_super_paquete(op_code code_op);
//void cargar_int_al_super_paquete(t_paquete* paquete, int numero);
//void cargar_choclo_al_super_paquete(t_paquete* paquete, void* choclo, int size);

//FIN DE PROCESO
char* enum_a_string_fin_de_proceso(motivo_fin_de_proceso motivo);

#endif /* UTILS_CONNECTIONS_H_ */
