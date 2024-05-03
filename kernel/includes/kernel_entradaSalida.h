#ifndef KERNEL_ENTRADASALIDA_H
#define KERNEL_ENTRADASALIDA_H

#include <../src/utils/utils_connections.h>
#include "k_gestor.h"


void atender_kernel_entradaSalida();
void validar_peticion(t_peticion* peticion);
//void existe_la_interfaz(char* interfaz);
//void esta_conectada_la_interfaz(char* interfaz);
bool validar_interfaz_admite_instruccion(char* interfaz, char* instruccion);
void enviar_peticion_a_interfaz(t_peticion* peticion);
void recibir_mensaje_fin_peticion();

#endif 