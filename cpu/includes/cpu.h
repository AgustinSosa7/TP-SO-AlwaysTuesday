#ifndef CPU_H_
#define CPU_H_

#include "cpu_gestor.h"
#include "inicializar_cpu.h"

//Variables GLOBALES

t_log* cpu_logger;
t_log* cpu_log_debug;
t_config* cpu_config;

int fd_cpu_dispatch;
int fd_cpu_interrupt;
int fd_kernel_dispatch;  
int fd_kernel_interrupt;
int fd_memoria;

char* IP_MEMORIA;
char* PUERTO_MEMORIA; //Quizas en crear_concxion() los toma como char*
char* PUERTO_ESCUCHA_DISPATCH;
char* PUERTO_ESCUCHA_INTERRUPT;
int CANTIDAD_ENTRADAS_TLB;
char* ALGORITMO_TLB;




/**
* @fn    decir_hola
* @brief Imprime un saludo al nombre que se pase por parámetro por consola.
*/
//void decir_hola(char* quien);

#endif