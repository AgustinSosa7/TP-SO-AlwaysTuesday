#ifndef K_GESTOR_H_
#define K_GESTOR_H_


#include <stdlib.h>
#include <stdio.h>

#include <commons/log.h>
#include <commons/config.h>


extern t_log* kernel_logger;
extern t_log* kernel_log_debug;
extern t_config* kernel_config;

//int fd_kernel;
//int fd_memoria;
//int fd_entradasalida;
//int fd_cpu_dispatch;
//int fd_cpu_interrupt;


extern char* PUERTO_ESCUCHA;
extern char* IP_MEMORIA;
extern char* PUERTO_MEMORIA; //Quizas en crear_concxion() los toma como char* a los int
extern char* IP_CPU;
extern char* PUERTO_CPU_DISPATCH;
extern char* PUERTO_CPU_INTERRUPT;
extern char* ALGORITMO_PLANIFICACION;
extern int QUANTUM;
extern char** RECURSOS;    // REVISAR EL TIPO EN EL ENUNCIADO 
extern char ** INSTANCIAS_RECURSOS; // POR FAVOR REVISAR EL TIPO EN EL ENUNCIADO
extern int GRADO_MULTIPROGRAMACION;
extern char * IP_KERNEL;

#endif