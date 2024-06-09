# include "../includes/cpu_kernel_dispatch.h"



void atender_cpu_kernel_dispatch(){
    log_info(cpu_logger, "Atendiendo a DISPATCH...");

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
                    imprimir_pcb( un_pcb,cpu_logger);
                    pcb_global = un_pcb;
                    pthread_t hilo_ciclo_instruccion;
                    pthread_create(&hilo_ciclo_instruccion, NULL, (void*)ciclo_instruccion, NULL);
                    pthread_detach(hilo_ciclo_instruccion); //Para mi es detach xq no sigue esperando si no que se activa del KERNEL , ver dps :D
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

