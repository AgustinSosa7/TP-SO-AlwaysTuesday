#ifndef KERNEL_CPU_INTERRUPT_H_
#define KERNEL_CPU_INTERRUPT_H_

#include "k_gestor.h"

void enviar_interrupción_a_cpu(op_code tipo_interrupción);
//void atender_kernel_cpu_interrupt();


#endif 