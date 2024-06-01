#ifndef KERNEL_H_
#define KERNEL_H_

#include "k_gestor.h"
#include "inicializar_kernel.h"
#include "kernel_cpu_dispatch.h"
#include "kernel_cpu_interrupt.h"
#include "kernel_entradaSalida.h"
#include "kernel_memoria.h"
#include "crear_conexiones.h"
#include "consola.h"
#include <../src/utils/utils_connections.h>


// Variables GLOBALES

t_list* IOS_CONECTADOS;

t_log* kernel_logger;
t_log* kernel_log_debug;
t_config* kernel_config;

int fd_kernel;
int fd_memoria;
int fd_entradasalida;
int fd_cpu_dispatch;
int fd_cpu_interrupt;


char* PUERTO_ESCUCHA;
char* IP_MEMORIA;
char* PUERTO_MEMORIA;
char* IP_CPU;
char* PUERTO_CPU_DISPATCH;
char* PUERTO_CPU_INTERRUPT;
char* ALGORITMO_PLANIFICACION;
int QUANTUM;
char** RECURSOS;    // REVISAR EL TIPO EN EL ENUNCIADO 
char ** INSTANCIAS_RECURSOS; // POR FAVOR REVISAR EL TIPO EN EL ENUNCIADO
int GRADO_MULTIPROGRAMACION;
char * IP_KERNEL;


t_list* lista_new;
t_list* lista_ready;
t_list* lista_ready_plus;
t_list* lista_exec;
t_list* lista_exit;


#endif
