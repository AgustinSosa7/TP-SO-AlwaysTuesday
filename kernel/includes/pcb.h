#ifndef PCB_H_  
#define PCB_H_

#include "k_gestor.h"


t_pcb* crearPcb();
int asignarPID();
void inicializar_registros(t_pcb* un_pcb);
void cambiar_de_estado_y_de_lista(estado_pcb estado_anterior, estado_pcb nuevo_estado);
t_pcb* sacar_de_la_lista_vieja(estado_pcb estado_anterior);
t_listas_estados* buscar_lista(estado_pcb estado_anterior);
void eliminar_pcb(t_pcb* un_pcb);
void imprimir_lista_ready(t_listas_estados* lista_a_mostrar);

#endif 