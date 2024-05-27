#ifndef KERNEL_ENTRADASALIDA_H
#define KERNEL_ENTRADASALIDA_H

#include <../src/utils/utils_connections.h>
#include "k_gestor.h"


void atender_kernel_entradaSalida();
void validar_peticion(t_peticion* peticion, t_pcb* pcb);
t_interfaz* existe_la_interfaz(char* interfaz, t_pcb* pcb);
bool esta_la_io(void* interfaz);
bool esta_conectada_la_interfaz(char* interfaz, t_interfaz* interfaz);
void validar_interfaz_admite_instruccion(char* interfaz, char* instruccion, t_pcb* pcb);
void enviar_peticion_a_interfaz(t_peticion* peticion);
void gestionar_lista_de_interfaz(t_peticion* peticion, t_interfaz* interfaz);
void recibir_fin_peticion();

#endif 