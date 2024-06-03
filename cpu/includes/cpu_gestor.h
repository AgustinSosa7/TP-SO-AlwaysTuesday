#ifndef CPU_GESTOR_H
# define CPU_GESTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h> 

#include <../src/utils/utils_connections.h>
#include <commons/log.h>
#include <commons/config.h>

// Variables GLOBALES

extern t_log* cpu_logger;
extern t_log* cpu_log_debug;
extern t_config* cpu_config;

extern t_registros_cpu* registros_cpu;

extern int fd_cpu_dispatch;
extern int fd_cpu_interrupt;
extern int fd_kernel_dispatch;  
extern int fd_kernel_interrupt;
extern int fd_memoria;

extern char* IP_MEMORIA;
extern char* IP_CPU;
extern char* PUERTO_MEMORIA; //Quizas en crear_concxion() los toma como char*
extern char* PUERTO_ESCUCHA_DISPATCH;
extern char* PUERTO_ESCUCHA_INTERRUPT;
extern int CANTIDAD_ENTRADAS_TLB;
extern char* ALGORITMO_TLB;

#endif