#ifndef CPU_H_
#define CPU_H_

#include <../src/utils/utils_connections.h>
#include "cpu_gestor.h"
#include "inicializar_cpu.h"
#include "cpu_kernel_dispatch.h"
#include "cpu_kernel_interrupt.h"
#include "cpu_memoria.h"
#include "fetch.h"
#include "decode.h"
#include "execute.h"

//Estructuras




t_log* cpu_logger;
t_log* cpu_log_debug;
t_config* cpu_config;

t_contexto_ejecucion* contexto_ejecucion; //CAMBIAR A PCB GLOBAL PREGUNTAR SI EL CAMBIO ESTA OK EL SABADO

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
cod_instruccion decodificacion_instruccion(char *instruccion);
bool codigo_inexistente(char *instruccion);
bool requiere_traduccion(char *instruccion);
void ejecucion_proceso(cod_instruccion instruccion, char** codigo);
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
