#ifndef INICIALIZAR_KERNEL_H
#define INICIALIZAR_KERNEL_H

#include "k_gestor.h"
//#include "kernel.h"

#include <commons/log.h>
#include <commons/config.h>


void inicializar_kernel();
t_log* inicializar_logs(void);
t_config* inicializar_configs(void);

#endif 