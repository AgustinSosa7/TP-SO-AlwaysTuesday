#ifndef KERNEL_ENTRADASALIDA_H
#define KERNEL_ENTRADASALIDA_H

#include <../src/utils/utils_connections.h>
#include "k_gestor.h"
#include "pcb.h"

void atender_kernel_entradaSalida();

t_interfaz* identificar_io(t_buffer* buffer);
t_list* instrucciones_que_acepta(char* tipo);
void agregar_io(t_interfaz* interfaz);
void gestionar_procesos_io(t_interfaz* interfaz);

void atender_pedido_io(t_paquete_y_pcb* paquete_y_pcb);
void enviar_proceso_a_blocked(t_proceso_blocked* proceso_a_ejecutar, t_interfaz* interfaz);
void desbloquear_proceso(t_pcb* pcb);

t_peticion* recibir_peticion(t_paquete* paquete);
t_peticion_param* leer_parametros(t_paquete* paquete, char* instruccion);

t_interfaz* validar_peticion(t_peticion* peticion, t_pcb* pcb);
t_interfaz* existe_la_interfaz(char* interfaz, t_pcb* pcb);
bool esta_la_io(void* interfaz);
void esta_conectada_la_interfaz(t_pcb* pcb, t_interfaz* interfaz, char* instruccion);
void validar_interfaz_admite_instruccion(t_pcb* pcb, t_list* instrucciones_posibles, char* instruccion);
void enviar_peticion_a_interfaz(t_peticion* peticion, t_interfaz* interfaz, t_pcb* pcb);
void eliminar_peticion(t_peticion* peticion);

#endif 