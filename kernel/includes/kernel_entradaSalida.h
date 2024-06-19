#ifndef KERNEL_ENTRADASALIDA_H_
#define KERNEL_ENTRADASALIDA_H_


#include "k_gestor.h"
#include "pcb.h"

void enviar_proceso_a_blocked(t_peticion* peticion, t_pcb* pcb, t_interfaz* interfaz);

t_peticion* recibir_peticion(t_paquete* paquete);
t_peticion_param* leer_parametros(t_paquete* paquete, char* instruccion);

t_interfaz* validar_peticion(t_peticion* peticion);
t_interfaz* existe_la_interfaz(char* nombre_io);
bool esta_la_io(void* interfaz);
bool esta_o_no(char* nombre_io, t_interfaz* io);
bool esta_conectada_la_interfaz(t_interfaz* interfaz, t_pcb* pcb);
void eliminar_interfaz(t_interfaz* interfaz);
bool validar_interfaz_admite_instruccion(t_interfaz* interfaz, char* instruccion);
void enviar_proceso_execute_a_exit();
void enviar_cola_de_procesos_blocked_io_a_exit(t_interfaz* interfaz);
void enviar_proceso_de_blocked_io_a_exit(void* proceso);
void enviar_proceso_blocked_io_a_exit(t_proceso_blocked* proceso_blocked);

bool enviar_peticion_a_interfaz(t_proceso_blocked* proceso_blocked, t_interfaz* interfaz);
void agregar_parametros_a_paquete(t_paquete* paquete, t_peticion* peticion);
bool recibir_fin_peticion(t_interfaz* interfaz);
void desbloquear_proceso(t_interfaz* interfaz);
void enviar_proceso_a_ready_o_ready_plus(t_pcb* un_pcb);
void enviar_proceso_blocked_a_ready(t_pcb* un_pcb);
void enviar_proceso_blocked_a_ready_plus(t_pcb* un_pcb);
void eliminar_peticion(t_peticion* peticion);
void eliminar_parametros_segun_instruccion(char* instruccion, t_peticion_param* parametros);

#endif 
