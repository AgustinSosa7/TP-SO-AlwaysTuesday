#include "../includes/es_gestor.h"

// Variables GLOBALES

t_log* entradasalida_logger;
t_log* entradasalida_log_debug;
t_config* entradasalida_config;

int fd_entradasalida;  
int fd_archivoBloques;
int fd_archivoBitmap; // debería ser INT?
int fd_memoria;
int fd_kernel;
t_list* lista_struct_fcbs;
char* bitmap_swap;
t_bitarray* bitmapSWAP; 
void* bloquesEnMemoria;

char* NOMBRE_INTERFAZ;
char* PUERTO_ESCUCHA;    
char* TIPO_INTERFAZ;
t_list* INSTRUCCIONES_POSIBLES;
int TIEMPO_UNIDAD_TRABAJO; 
char* IP_KERNEL;
char* PUERTO_KERNEL;
char* IP_MEMORIA;
char* PUERTO_MEMORIA;
char* PATH_BASE_DIALFS;
char* PATH_BLOQUES;
char* PATH_BITMAP;
char* PATH_METADATA;
int BLOCK_SIZE;
int BLOCK_COUNT;
int RETRASO_COMPACTACION;

t_list* lista_registros_propisito_general;
t_list* lista_registros_extendidos;