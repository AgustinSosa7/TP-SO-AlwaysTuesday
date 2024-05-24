#ifndef K_GESTOR_H_
#define K_GESTOR_H_


#include <stdlib.h>
#include <stdio.h>
#include <pthread.h> 

#include <../src/utils/utils_connections.h>
#include <commons/log.h>
#include <commons/config.h>

typedef struct{
    int tiempo_espera;
    char* archivo;
    char* registro1;
    char* registro2;
    char* registro3; 
} t_peticion_param;
typedef struct{
    char* instruccion;
    char* interfaz;
    t_peticion_param* parametros;
} t_peticion;



///////////////////////ESTRUCTURAS PCB////////////////////////////
// typedef struct{
//     int pid,
//     int pc,
//     int quantum,
//     t_registros_cpu registros,
//     estado_pcb estado
// } t_pcb;

//Ver si conviene ponerlo en el utils :D
typedef struct{  
	int pid;
	int program_counter;
    int QUANTUM;
	t_cpu* registros_CPU;
}t_pcb;



// typdef struct{
 
// } t_registros_cpu;

// typedef enum{
//     NEW,
//     READY,
//     EXEC,
//     BLOCKED,
//     EXIT
// } estado_pcb;

///////////////////////////////////////////////////////////////
typedef struct 
{
    char* nombre;
    int tipo_interfaz;
    int fd_interfaz;
    t_queue* cola_procesos_blocked;
} t_list_io;


///////////////////////////////////////////////////////////////
extern t_log* kernel_logger;
extern t_log* kernel_log_debug;
extern t_config* kernel_config;

extern int fd_kernel;
extern int fd_memoria;
extern int fd_entradasalida;
extern int fd_cpu_dispatch;
extern int fd_cpu_interrupt;


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