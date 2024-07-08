#ifndef FINALIZAR_KERNEL_H_
#define FINALIZAR_KERNEL_H_

#include "k_gestor.h"
#include "pcb.h"

void finalizar_kernel();
void eliminar_listas();
void eliminar_pcbs_y_listas();
void eliminar_lista_instrucciones_posibles();
void eliminar_recursos();
void eliminar_recurso(t_recursos* recurso);
#endif 