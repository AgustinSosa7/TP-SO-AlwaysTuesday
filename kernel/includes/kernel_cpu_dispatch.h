#ifndef KERNEL_CPU_DISPATCH_H_
#define KERNEL_CPU_DISPATCH_H_

#include "k_gestor.h"

#include <../src/utils/utils_connections.h>

typedef struct{
    char* instruccion;
    char* interfaz;
    void *parametros;
} t_peticion;

void atender_kernel_cpu_dispatch();
t_peticion* peticion_deserializar(t_buffer* buffer);

#endif 