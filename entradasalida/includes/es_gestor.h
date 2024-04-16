#ifndef ES_GESTOR_H_
#define ES_GESTOR_H_

#include <stdlib.h>
#include <stdio.h>

#include <commons/log.h>
#include <commons/config.h>

//#include <../src/utils/hello.h>
#include <../src/utils/utils_connections.h>

//Variables GLOBALES 
extern t_log* entradasalida_logger;
extern t_log* entradasalida_log_debug;
extern t_config* entradasalida_config;

extern int fd_entradasalida;  
extern int fd_memoria;
extern int fd_kernel;      

extern int PUERTO_ESCUCHA;     
extern char* TIPO_INTERFAZ;
extern int TIEMPO_UNIDAD_TRABAJO; //Quizas en crear_concxion() los toma como char*
extern char* IP_KERNEL;
extern int PUERTO_KERNEL;
extern char* IP_MEMORIA;
extern int PUERTO_MEMORIA;
extern char* PATH_BASE_DIALFS;
extern int BLOCK_SIZE;
extern int BLOCK_COUNT;


#endif /* ES_GESTOR_H_ */