#ifndef KERNEL_CPU_DISPATCH_H_
#define KERNEL_CPU_DISPATCH_H_

#include <../src/utils/utils_connections.h>
#include "k_gestor.h"


void atender_kernel_cpu_dispatch();
t_peticion* recibir_peticion(t_paquete* paquete);
t_peticion_param* leer_parametros(t_paquete* paquete, char* instruccion);
void eliminar_peticion(t_peticion* peticion);
void recibir_mensaje_fin_peticion();
void recibir_pcb_con_motivo();

#endif 