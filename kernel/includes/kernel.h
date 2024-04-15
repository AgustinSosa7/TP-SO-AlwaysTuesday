#ifndef KERNEL_H_
#define KERNEL_H_

#include "k_gestor.h"
#include "inicializar_kernel.h"

#include <../src/utils/hello.h>
#include <../src/utils/utils_connections.h>

// Variables GLOBALES

t_log* kernel_logger;
t_log* kernel_log_debug;
t_config* kernel_config;

int fd_kernel;
int fd_memoria;
int fd_entradasalida;
int fd_cpu_dispatch;
int fd_cpu_interrupt;


int PUERTO_ESCUCHA;
char* IP_MEMORIA;
int PUERTO_MEMORIA;
char* IP_CPU;
int PUERTO_CPU_DISPATCH;
int PUERTO_CPU_INTERRUPT;
char* ALGORITMO_PLANIFICACION;
int QUANTUM;
char** RECURSOS;    // REVISAR EL TIPO EN EL ENUNCIADO 
char ** INSTANCIAS_RECURSOS; // POR FAVOR REVISAR EL TIPO EN EL ENUNCIADO
int GRADO_MULTIPROGRAMACION;

#endif