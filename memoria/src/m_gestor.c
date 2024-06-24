#include "../includes/m_gestor.h"

// Variables GLOBALES

t_log* memoria_logger;
t_log* memoria_log_debug;
t_config* memoria_config;

t_list* procesos_memoria;

int fd_memoria;
//int fd_entradasalida;  // Estos cuatro falta pasarlos tambien al m_gestor.h poniendoles el extern al prinicipio.
int fd_cpu;            // Soy Ruka, uds dejaron esto acá? Estoy modularizando la Mem. PD: lo agregué
int fd_kernel;

char* server_name;


//CONFIG
char* PUERTO_ESCUCHA;
int TAM_MEMORIA;
int TAM_PAGINA;
char* PATH_INSTRUCCIONES;
int RETARDO_RESPUESTA;


/// EVALUAR SI REALMENTE SE UTILIZAN EN ESTE TP. (Me parece que no)
char* IP_MEMORIA;
char* IP_FILESYSTEM;
char* PUERTO_FILESYSTEM;
char* ALGORITMO_REEMPLAZO;