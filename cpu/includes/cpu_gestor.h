#ifndef CPU_GESTOR_H
#define CPU_GESTOR_H

#include <../src/utils/utils_connections.h>     // La ruta de este include me parece que está mal...
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h> 
#include <commons/log.h>
#include <commons/config.h>

// Enum con las Instrucciones
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
    //WAIT,   ya definidos en utils.h
    //SIGNAL,
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

// Variables globales
extern t_log* cpu_logger;
extern t_log* cpu_log_debug;
extern t_config* cpu_config;

extern t_pcb* pcb_global;

extern char ** opcode_cpu;      // Chequear si me sirve o no esta variable.

extern int tamanio_pagina;
extern int tamanio_memoria;

extern bool dejar_de_ejecutar;
extern bool ocurrio_interrupcion;
extern int motivo_interrupcion;

// Estructuras
typedef struct {
    int direccion_fisica;
    int cantidad_bytes_podemos_escribir;
} t_direccion_a_operar;

// Listas
extern t_list* lista_direcciones_operables;

// Semaforos
extern sem_t sem_ciclo_de_instruccion;
extern pthread_mutex_t mutex_ocurrio_interrupcion;

// Conexiones
extern int fd_cpu_dispatch;
extern int fd_cpu_interrupt;
extern int fd_kernel_dispatch;  
extern int fd_kernel_interrupt;
extern int fd_memoria;

// Configuracion
extern char* IP_MEMORIA;
extern char* IP_CPU;
extern char* PUERTO_MEMORIA; 
extern char* PUERTO_ESCUCHA_DISPATCH;
extern char* PUERTO_ESCUCHA_INTERRUPT;
extern int CANTIDAD_ENTRADAS_TLB;
extern char* ALGORITMO_TLB;

#endif