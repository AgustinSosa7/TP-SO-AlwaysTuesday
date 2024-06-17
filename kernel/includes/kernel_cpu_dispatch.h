#ifndef KERNEL_CPU_DISPATCH_H_
#define KERNEL_CPU_DISPATCH_H_

#include "k_gestor.h"
#include "pcb.h"
#include "kernel_entradaSalida.h"

void recibir_pcb_con_motivo();
bool esta_el_recurso(t_recursos* recurso, char* recurso_solicitado);
#endif 