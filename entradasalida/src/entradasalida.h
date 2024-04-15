#ifndef ENTRADASALIDA_H_
#define ENTRADASALIDA_H_

#include <stdlib.h>
#include <stdio.h>

#include <commons/log.h>
#include <commons/config.h>

#include <../src/utils/hello.h>
#include <../src/utils/utils_connections.h>

// Variables GLOBALES

t_log* entradasalida_logger;
t_log* entradasalida_log_debug;
t_config* entradasalida_config;

int fd_entradasalida;  //Agregar estos 3 a en el e_gestor.h
int fd_memoria;
int fd_kernel;      

char* PUERTO_ESCUCHA;     //Agregar al e_gestor.h
char* TIPO_INTERFAZ;
int TIEMPO_UNIDAD_TRABAJO; //Quizas en crear_concxion() los toma como char*
char* IP_KERNEL;
int PUERTO_KERNEL;
char* IP_MEMORIA;
int PUERTO_MEMORIA;
char* PATH_BASE_DIALFS;
int BLOCK_SIZE;
int BLOCK_COUNT;

/**
* @fn    decir_hola
* @brief Imprime un saludo al nombre que se pase por parámetro por consola.
*/
void decir_hola(char* quien);

#endif
