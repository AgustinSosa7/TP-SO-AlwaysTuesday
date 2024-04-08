#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdlib.h>
#include <stdio.h>

#include <commons/log.h>
#include <commons/config.h>

// Variables GLOBALES

t_log* kernel_logger;
t_log* kernel_log_debug;
t_config* kernel_config;

int PUERTO_ESCUCHA;
char* IP_MEMORIA;
int PUERTO_MEMORIA; //Quizas en crear_concxion() los toma como char* a los int
char* IP_CPU;
int PUERTO_CPU_DISPATCH;
int PUERTO_CPU_INTERRUPT;
char* ALGORITMO_PLANIFICADOR;
int QUANTUM;
char** RECURSOS;    // REVISAR EL TIPO EN EL ENUNCIADO 
char ** INSTANCIAS_RECURSOS; // POR FAVOR REVISAR EL TIPO EN EL ENUNCIADO
int GRADO_MULTIPROGRAMACION;


/**
* @fn    decir_hola
* @brief Imprime un saludo al nombre que se pase por parámetro por consola.
*/
void decir_hola(char* quien);

#endif
