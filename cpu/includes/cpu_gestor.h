#ifndef CPU_GESTOR_H
# define CPU_GESTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h> 

#include <../src/utils/utils_connections.h>
#include <commons/log.h>
#include <commons/config.h>

// Variables GLOBALES

typedef enum 
{
    SET,
    MOV_IN,
    MOV_OUT,
    SUM,
    SUB,
    JNZ,
    RESIZE,
    COPY_STRING,
    WAIT,
    SIGNAL,
    IO_GEN_SLEEP,
    IO_STDIN_READ,
    IO_STDOUT_WRITE,
    IO_FS_CREATE,
    IO_FS_DELETE,
    IO_FS_TRUNCATE,
    IO_FS_WRITE,
    IO_FS_READ,
    EXIT_CPU
}cod_instruccion;

extern t_pcb* pcb_global;
extern char ** opcode_cpu;


extern t_log* cpu_logger;
extern t_log* cpu_log_debug;
extern t_config* cpu_config;


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