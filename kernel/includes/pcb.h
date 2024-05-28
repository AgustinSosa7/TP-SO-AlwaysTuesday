#ifndef PCB_H_  //Ver que onda :D
#define PCB_H_

#include "k_gestor.h"

void _enviar_pcb_a_CPU_por_dispatch(t_pcb* un_pcb);
//void _enviar_respuesta_instruccion_CPU_por_dispatch(int respuesta);
void cambiar_estado(t_pcb* un_pcb, estado_pcb nuevo_estado);
void inicializar_registros(t_pcb* un_pcb);
int asignarPID();
t_pcb* crearPcb();

#endif 