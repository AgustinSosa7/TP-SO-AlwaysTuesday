#ifndef ENTRADASALIDA_KERNEL_H_
#define ENTRADASALIDA_KERNEL_H

#include <../src/utils/utils_connections.h>
#include "es_gestor.h"



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

void identificarse_con_kernel(int socket, char* nombre_io);

void atender_entradasalida_kernel();

t_peticion* recibir_peticion(t_paquete* paquete);
void asignar_parametros_segun_tipo(t_peticion* peticion, void* stream);
void procesar_peticion(t_peticion* peticion);

char* recibir_instruccion(t_paquete* paquete);
void validar_tipo_instruccion(char* tipo_instruccion);

void finalizar_peticion(t_peticion* peticion);
void eliminar_peticion(t_peticion* peticion);



#endif 