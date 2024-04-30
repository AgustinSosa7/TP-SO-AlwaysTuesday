#ifndef KERNEL_ENTRADASALIDA_H
#define KERNEL_ENTRADASALIDA_H

#include "k_gestor.h"

typedef struct{
    int tiempo_espera;
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

void atender_kernel_entradaSalida();
void validar_peticion(t_peticion* peticion);
//void existe_la_interfaz(char* interfaz);
//void esta_conectada_la_interfaz(char* interfaz);
bool validar_interfaz_admite_instruccion(char* interfaz, char* instruccion);

#endif 