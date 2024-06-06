#include "../includes/k_gestor.h"

t_list* INSTRUCCIONES_GEN;
t_list* INSTRUCCIONES_STDIN;
t_list* INSTRUCCIONES_STDOUT;
t_list* INSTRUCCIONES_FS;
t_list* IOS_CONECTADOS;

t_list* lista_instrucciones;

 t_queue* cola_new;
 t_queue* cola_ready;
 t_queue* cola_ready_plus;
 t_list* lista_exec;
 t_queue* cola_exit;
////////////////SEMAFOROS/////////////////////////////////////////
 pthread_mutex_t* mutex_pid;
 pthread_mutex_t* mutex_new;
 pthread_mutex_t* mutex_ready;
 pthread_mutex_t* mutex_exec;
 pthread_mutex_t* mutex_ready_plus;
 pthread_mutex_t* mutex_exit;
 pthread_mutex_t* mutex_io;
 sem_t* sem_grado_multiprogram;
 sem_t* sem_new_a_ready;
 sem_t* sem_planificador_corto_plazo;
