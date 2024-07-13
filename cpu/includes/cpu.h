#ifndef CPU_H_
#define CPU_H_

#include <../src/utils/utils_connections.h>
#include "cpu_gestor.h"
#include "inicializar_cpu.h"
#include "cpu_kernel_dispatch.h"
#include "cpu_kernel_interrupt.h"
#include "ciclo_instruccion.h"
#include "cpu_memoria.h"

void conexion_cpu_kernel_dispatch();
void conexion_cpu_kernel_interrupt();
void limpiar_tlb();
void terminar_cpu();

#endif