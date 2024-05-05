#ifndef MEMORIA_H_
#define MEMORIA_H_

#include "m_gestor.h"
#include "inicializar_memoria.h"
#include "memoria_procesos.h"

typedef struct
{
	int pid;
	t_list* instrucciones;
} t_proceso;

//  Lista de instrucciones provisorias
t_list* lista_de_instrucciones;

// Variables GLOBALES

t_log* memoria_logger;
t_log* memoria_log_debug;
t_config* memoria_config;

int fd_memoria;
int fd_entradasalida;  // Estos cuatro falta pasarlos tambien al m_gestor.h poniendoles el extern al prinicipio.
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




#endif /* MEMORIA_H_*/
