#ifndef INICIALIZAR_KERNEL_H
#define INICIALIZAR_KERNEL_H

#include "k_gestor.h"

#include <commons/log.h>
#include <commons/config.h>

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


void inicializar_logs(void);
void inicializar_configs(char* path);
void inicializar_listas_y_colas(void);
void inicializar_listas_instrucciones();
t_list* asignar_instrucciones_posibles(char* tipo);
void inicializar_semaforos(void);
void inicializar_pid();
void inicializar_kernel(char* path);


#endif 