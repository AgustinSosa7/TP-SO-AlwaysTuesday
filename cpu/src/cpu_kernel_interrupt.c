# include "../includes/cpu_kernel_interrupt.h"

void atender_interrupciones(){
    log_info(cpu_logger, "Atendiendo a INTERRUPT...");

    bool control_key = 1;
    while (control_key) {
		int cod_op = recibir_operacion(fd_kernel_interrupt);
        
		switch (cod_op) {
            case INTERRUPCION_FIN_QUANTUM:
                //enviar_pcb_a(un_pcb, fd_kernel_dispatch, DESALOJO_QUANTUM);
                break;
            case INTERRUPCION_FIN_PROCESO:
                //enviar_pcb_a(un_pcb, fd_kernel_dispatch, PROCESO_EXIT);
                break;
            case -1:
                log_error(cpu_logger, "Desconexión de KERNEL - Interrupt");
                control_key= 0;
            default:
                log_warning(cpu_logger,"Operacion desconocida de KERNEL - Interrupt");
                break;
		}
    }
}

