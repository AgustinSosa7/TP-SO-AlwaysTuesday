#ifndef PLANIFICADOR_CORTO_PLAZO_H_
#define PLANIFICADOR_CORTO_PLAZO_H_

#include "k_gestor.h"
#include "pcb.h"
#include <../src/utils/utils_connections.h>


void planif_corto_plazo();

void planif_fifo();
void planif_RR();
void planif_VRR();

void atender_pedido_io(t_paquete* paquete, t_pcb* pcb_recibido);
void enviar_proceso_a_blocked(t_proceso_blocked* proceso_a_ejecutar, t_interfaz* interfaz);


#endif 