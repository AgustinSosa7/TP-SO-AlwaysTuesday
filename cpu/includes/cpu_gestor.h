#ifndef CPU_GESTOR_H
#define CPU_GESTOR_H

#include <../src/utils/utils_connections.h>     // La ruta de este include me parece que está mal...
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h> 
#include <commons/log.h>
#include <commons/config.h>

// Variables globales
extern t_log* cpu_logger;
extern t_log* cpu_log_debug;
extern t_config* cpu_config;

extern t_pcb* pcb_global;

extern int tamanio_pagina;
extern int tamanio_memoria;

extern bool dejar_de_ejecutar;
extern bool ocurrio_interrupcion;
extern int motivo_interrupcion;

// TLB
extern t_list* tlb;

typedef struct {
    int pid;
    int nro_pagina;
    int nro_marco;
    t_temporal* tiempo_carga;
    t_temporal* tiempo_ultima_ref;
}t_entrada_tlb;

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