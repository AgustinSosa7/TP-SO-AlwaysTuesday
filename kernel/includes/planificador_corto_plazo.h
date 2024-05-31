#ifndef PLANIFICADOR_CORTO_PLAZO_H_
#define PLANIFICADOR_CORTO_PLAZO_H_

#include "k_gestor.h"
#include "pcb.h"
#include <../src/utils/utils_connections.h>



void planif_corto_plazo();
void planif_fifo_RR();
void planif_VRR();
void gestionar_quantum_VRR();
void gestionar_quantum(t_pcb* un_pcb);



#endif 