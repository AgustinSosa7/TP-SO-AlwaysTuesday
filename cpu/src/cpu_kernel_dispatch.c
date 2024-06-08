# include "../includes/cpu_kernel_dispatch.h"


void atender_cpu_kernel_dispatch(){

    bool control_key = 1;
    while (control_key) {
		int cod_op = recibir_operacion(fd_kernel_dispatch);
        t_paquete* paquete = recibir_paquete(fd_kernel_dispatch);
        log_info(cpu_logger, "Se recibio algo de KERNEL_Dispatch : %d", cod_op);
	//	t_buffer* unBuffer;
		switch (cod_op) {
            case MENSAJE:
                //recibir_mensaje(cliente_fd);
                break;
            case PAQUETE:
                //
                break;
            case PCB:
                    //Recibe el paquete lo deserealiza y recibe el PCB
                    log_info(cpu_logger,"LLego el PCB");
                    t_pcb* un_pcb = recibir_pcb(paquete);
                    //Se prueba si llega bien el PCB
                    imprimir_pcb(un_pcb, cpu_logger);
                    free(un_pcb);
                    //signal para que siga el ciclo de instruccion
            break;
            case -1:
                log_error(cpu_logger, "Desconexión de KERNEL - Dispatch");
                control_key = 0;
            default:
                log_warning(cpu_logger,"Operacion desconocida de KERNEL - Dispatch");
                break;
		}
    }

}

//enviar pcb