#ifndef INICIALIZAR_KERNEL_H
#define INICIALIZAR_KERNEL_H

#include "k_gestor.h"

#include <commons/log.h>
#include <commons/config.h>


void inicializar_kernel(char* path);
void inicializar_logs(void);
void inicializar_configs(char* path);
void inicializar_listas(void);
void inicializar_semaforos(void);
void inicializar_pid_y_procesos_activos();

#endif 