#ifndef KERNEL_H_
#define KERNEL_H_

#include "k_gestor.h"
#include "inicializar_kernel.h"
#include "planificador_largo_plazo.h"
#include "planificador_corto_plazo.h"
#include "crear_conexiones.h"
#include "kernel_memoria.h"
#include "kernel_entradaSalida.h"
#include "consola.h"



// Variables GLOBALES


int pid_global;
int tiempo_transcurrido;

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


#endif
