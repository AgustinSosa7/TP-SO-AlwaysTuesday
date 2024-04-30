#ifndef ENTRADASALIDA_KERNEL_H_
#define ENTRADASALIDA_KERNEL_H

#include "es_gestor.h"

#include <../src/utils/utils_connections.h>




typedef struct{
    int tiempo_espera;
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
t_peticion* recibir_peticion(t_paquete* paquete);
void procesar_peticion(char* instruccion, t_peticion_param* parametros);
char* recibir_instruccion(t_paquete* paquete);
void validar_instruccion(char* tipo_instruccion);
bool la_instruccion_esta_en_la_lista(char* instruccion_posible, char* instruccion);


#endif 