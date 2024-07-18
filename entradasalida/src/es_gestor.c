#include "../includes/es_gestor.h"

// Variables GLOBALES

t_log* entradasalida_logger;
t_log* entradasalida_log_debug;
t_config* entradasalida_config;

int fd_entradasalida;  
int fd_archivoBloques;
int fd_archivoBitmap;
DIR* fd_directorio;
int fd_memoria;
int fd_kernel;
char* pre_bitmap;
t_bitarray* bitmap; 
char* bloquesEnMemoria;

/////////////////SEMAFOROS/////////////
pthread_mutex_t mutex_bitmap;
pthread_mutex_t mutex_bloques;
///////////////////////////////////////

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
t_list* lista_archivos_existentes;