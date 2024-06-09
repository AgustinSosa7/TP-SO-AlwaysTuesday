#ifndef INICIALIZAR_CPU_H
#define INICIALIZAR_CPU_H

#include "cpu_gestor.h"

#include <commons/log.h>
#include <commons/config.h>

extern pthread_t hilo_kernel_interrupt;
void inicializar_cpu(char* path);
void inicializar_logs();
void iniciar_config(char* path);
void imprimir_config();
void inicializar_estructuras();

#endif



