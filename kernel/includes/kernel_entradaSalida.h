#ifndef KERNEL_ENTRADASALIDA_H
#define KERNEL_ENTRADASALIDA_H

#include <../src/utils/utils_connections.h>
#include "k_gestor.h"
#include "pcb.h"


void enviar_proceso_a_blocked(t_peticion_pcb_interfaz* peticion_pcb_interfaz);

t_peticion* recibir_peticion(t_paquete* paquete);
t_peticion_param* leer_parametros(t_paquete* paquete, char* instruccion);

t_interfaz* validar_peticion(t_peticion* peticion, t_pcb* pcb);
t_interfaz* existe_la_interfaz(char* interfaz, t_pcb* pcb);
bool esta_la_io(void* interfaz);
bool esta_o_no(char* nombre_io, t_interfaz* io);
bool esta_conectada_la_interfaz(t_interfaz* interfaz, t_pcb* pcb);
void validar_interfaz_admite_instruccion(t_interfaz* interfaz, char* instruccion, t_pcb* pcb);
void enviar_peticion_a_interfaz(t_proceso_blocked* proceso_blocked, t_interfaz* interfaz);
void recibir_fin_peticion(t_interfaz* interfaz);
void desbloquear_proceso(t_pcb* un_pcb);
void eliminar_peticion(t_peticion* peticion);

#endif 
