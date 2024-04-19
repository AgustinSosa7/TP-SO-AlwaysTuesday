#ifndef ENTRADASALIDA_H_
#define ENTRADASALIDA_H_

#include "es_gestor.h"
#include "inicializar_entradasalida.h"

// Variables GLOBALES

t_log* entradasalida_logger;
t_log* entradasalida_log_debug;
t_config* entradasalida_config;

int fd_entradasalida;  //Agregar estos 3 a en el es_gestor.h
int fd_memoria;
int fd_kernel;      

char* PUERTO_ESCUCHA;     //Agregar al es_gestor.h
char* TIPO_INTERFAZ;
int TIEMPO_UNIDAD_TRABAJO; //Quizas en crear_concxion() los toma como char*
char* IP_KERNEL;
char* PUERTO_KERNEL;
char* IP_MEMORIA;
char* PUERTO_MEMORIA;
char* PATH_BASE_DIALFS;
int BLOCK_SIZE;
int BLOCK_COUNT;


#endif
