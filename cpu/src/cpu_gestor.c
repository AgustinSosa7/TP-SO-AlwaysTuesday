#include "../includes/cpu_gestor.h"

// Variables globales
t_log* cpu_logger;
t_log* cpu_log_debug;
t_config* cpu_config;

t_pcb* pcb_global;

int tamanio_pagina;
int tamanio_memoria;

bool dejar_de_ejecutar;
bool ocurrio_interrupcion;
int motivo_interrupcion;

// TLB
t_list* tlb;

// Semaforos
sem_t sem_ciclo_de_instruccion;
pthread_mutex_t mutex_ocurrio_interrupcion;

// Conexiones
int fd_cpu_dispatch;
int fd_cpu_interrupt;
int fd_kernel_dispatch;  
int fd_kernel_interrupt;
int fd_memoria;

// Configuracion
char* IP_MEMORIA;
char* IP_CPU;
char* PUERTO_MEMORIA;
char* PUERTO_ESCUCHA_DISPATCH;
char* PUERTO_ESCUCHA_INTERRUPT;
int CANTIDAD_ENTRADAS_TLB;
char* ALGORITMO_TLB;