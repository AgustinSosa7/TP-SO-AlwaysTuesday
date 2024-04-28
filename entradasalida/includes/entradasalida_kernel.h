#ifndef ENTRADASALIDA_KERNEL_H_
#define ENTRADASALIDA_KERNEL_H

#include "es_gestor.h"

#include <../src/utils/utils_connections.h>


typedef struct{
    u_int32_t tiempo_espera;
    char* archivo;
    char* registro1;
    char* registro2;
    char* registro3; 
} t_peticion_param;

typedef struct{
    char* instruccion;
    t_peticion_param* parametros;
} t_peticion;


void atender_entradasalida_kernel();
t_peticion* peticion_deserializar(t_buffer* buffer);
void procesar_peticion(char* instruccion, t_peticion_param* parametros);

#endif 