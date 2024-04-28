#ifndef KERNEL_CPU_DISPATCH_H_
#define KERNEL_CPU_DISPATCH_H_

#include <../src/utils/utils_connections.h>
#include "k_gestor.h"



typedef struct{
    uint32_t tiempo_espera;
    char* archivo;
    char* registro1;
    char* registro2;
    char* registro3; 
} t_peticion_param;
typedef struct{
    char* instruccion;
    char* interfaz;
    t_peticion_param* parametros;
} t_peticion;

void atender_kernel_cpu_dispatch();
t_peticion* peticion_deserializar(t_buffer* buffer);
t_peticion_param* leer_parametros(t_buffer* buffer, char* instruccion);
#endif 