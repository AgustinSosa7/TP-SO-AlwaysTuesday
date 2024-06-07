#include "../includes/kernel_cpu_interrupt.h"

void enviar_interrupción_a_cpu(op_code tipo_interrupción){
    t_paquete* un_paquete = crear_paquete(tipo_interrupción);
    // no tiene nada en el buffer.
	enviar_paquete(un_paquete,fd_cpu_interrupt);
    eliminar_paquete(un_paquete);
} 






