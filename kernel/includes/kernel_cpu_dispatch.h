#ifndef KERNEL_CPU_DISPATCH_H_
#define KERNEL_CPU_DISPATCH_H_

#include <../src/utils/utils_connections.h>
#include "k_gestor.h"
#include "kernel_entradasalida.h"

void recibir_pcb_con_motivo();
t_peticion* recibir_peticion(t_paquete* paquete);
t_peticion_param* leer_parametros(t_paquete* paquete, char* instruccion);
void eliminar_peticion(t_peticion* peticion);

#endif 