#ifndef ES_GESTOR_H_
#define ES_GESTOR_H_

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h> 
#include <fcntl.h>
#include<math.h>
#include <dirent.h>


#include <../src/utils/utils_connections.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/bitarray.h>
#include <sys/mman.h>

typedef struct{
    char* instruccion;
    t_peticion_param* parametros;
    int pid;
} t_peticion;


//Variables GLOBALES 
extern t_log* entradasalida_logger;
extern t_log* entradasalida_log_debug;
extern t_config* entradasalida_config;

extern int fd_entradasalida; 
extern int fd_archivoBloques;
extern int fd_archivoBitmap;
extern DIR* fd_directorio;
extern int fd_memoria;
extern int fd_kernel;

extern char* pre_bitmap;
extern t_bitarray* bitmap; 
extern char* bloquesEnMemoria;   

/////////////////SEMAFOROS/////////////
extern pthread_mutex_t mutex_bitmap;
extern pthread_mutex_t mutex_bloques;
///////////////////////////////////////
      

extern char* NOMBRE_INTERFAZ;
extern char* PUERTO_ESCUCHA;   
extern char* TIPO_INTERFAZ;
extern int TIEMPO_UNIDAD_TRABAJO; 
extern char* IP_KERNEL;
extern char* PUERTO_KERNEL;
extern char* IP_MEMORIA;
extern char* PUERTO_MEMORIA;
extern char* PATH_BASE_DIALFS;
extern char* PATH_BLOQUES;
extern char* PATH_BITMAP;
extern char* PATH_METADATA;
extern int BLOCK_SIZE;
extern int BLOCK_COUNT;
extern int RETRASO_COMPACTACION;

extern t_list* lista_registros_propisito_general;
extern t_list* lista_registros_extendidos;
extern t_list* lista_archivos_existentes;


#endif /* ES_GESTOR_H_ */