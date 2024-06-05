#ifndef CPU_H_
#define CPU_H_

#include <../src/utils/utils_connections.h>
#include "cpu_gestor.h"
#include "inicializar_cpu.h"
#include "cpu_kernel_dispatch.h"
#include "cpu_kernel_interrupt.h"
#include "cpu_memoria.h"
#include "decode.h"
#include "execute.h"


t_log* cpu_logger;
t_log* cpu_log_debug;
t_config* cpu_config;

t_registros_cpu* registros_cpu;

int fd_cpu_dispatch;
int fd_cpu_interrupt;
int fd_kernel_dispatch;  
int fd_kernel_interrupt;
int fd_memoria;

char* IP_MEMORIA;
char* IP_CPU;
char* PUERTO_MEMORIA; //Quizas en crear_concxion() los toma como char*
char* PUERTO_ESCUCHA_DISPATCH;
char* PUERTO_ESCUCHA_INTERRUPT;
int CANTIDAD_ENTRADAS_TLB;
char* ALGORITMO_TLB;

void ciclo_instruccion();
char* decodificacion_instruccion(t_list *lista_de_instrucciones);
bool codigo_inexistente(char *instruccion);
bool requiere_traduccion(char *instruccion);
void ejecucion_proceso(char *codigo);
cod_instruccion identificador_instruccion(char* codigo);

//semaforo_t semaforo_control_fetch_decode;
//semaforo_t semaforo_control_decode_execute;


/*
void iniciar_semaforo();
void comenzar_ciclo_instruccion();
void ciclo_instruccion_decode();
void ciclo_instruccion_fetch();
bool preguntando_si_hay_interrupciones_vigentes();

void ciclo_instruccion_execute();
bool validador_de_header(char* header_string);
uint32_t* detectar_registro(char* RX);
t_paquete* alistar_paquete_de_desalojo(op_code code_op);
void enviarPaqueteKernel(char* motivo);
void enviarPaqueteKernelConInfoExtra(t_paquete* infoExtra);*/


#endif
