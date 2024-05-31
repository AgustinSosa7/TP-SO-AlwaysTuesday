#ifndef PCB_H_  
#define PCB_H_

#include "k_gestor.h"


void cambiar_estado(t_pcb* un_pcb, estado_pcb nuevo_estado);
void inicializar_registros(t_pcb* un_pcb);
int asignarPID();
t_pcb* crearPcb();

#endif 