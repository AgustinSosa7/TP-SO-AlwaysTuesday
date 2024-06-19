#ifndef ES_GESTOR_H_
#define ES_GESTOR_H_

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h> 
#include <fcntl.h>
#include<math.h>


#include <../src/utils/utils_connections.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/bitarray.h>
#include <sys/mman.h>

typedef struct{
    int tiempo_espera;
    char* archivo;
    int registroDireccion;
    int registroTamanio;
    char* registroPunteroArchivo; 
} t_peticion_param;

typedef struct{
    char* instruccion;
    t_peticion_param* parametros;
} t_peticion;


typedef struct{
    char* nombre;
	int tamanio;
	int bloque_inicial;
    t_config* archivo_fcb;
}t_fcb;

//Variables GLOBALES 
extern t_log* entradasalida_logger;
extern t_log* entradasalida_log_debug;
extern t_config* entradasalida_config;

extern int fd_entradasalida; 
extern int fd_archivoBloques;
extern int fd_archivoBitmap;
extern int fd_memoria;
extern int fd_kernel;
extern t_list * lista_struct_fcbs;

extern char* bitmap_swap;
extern t_bitarray* bitmapSWAP; 
extern void* bloquesEnMemoria;   
      

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


#endif /* ES_GESTOR_H_ */