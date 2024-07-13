# include "../includes/cpu_kernel_dispatch.h"

void atender_dispatch(){
    log_info(cpu_logger, "Atendiendo a DISPATCH...");

    bool control_key = 1;
    while (control_key) {
		int cod_op = recibir_operacion(fd_kernel_dispatch);
        t_paquete* paquete = recibir_paquete(fd_kernel_dispatch);
        log_info(cpu_logger, "Se recibio algo de KERNEL_Dispatch : %d", cod_op);

		switch (cod_op) {
            case PCB:
                    log_info(cpu_logger,"Llego un nuevo proceso (PCB) para ser ejecutado en el CPU!");
                    establecer_pcb_global(paquete, pcb_global);
                    //imprimir_pcb(pcb_global, cpu_logger);
                    sem_post(&sem_ciclo_de_instruccion);
                break;
            
            case -1:
                log_error(cpu_logger, "Desconexión de KERNEL - Dispatch");
                control_key = 0;
            
            default:
                log_warning(cpu_logger,"Operacion desconocida de KERNEL - Dispatch");
                exit(EXIT_FAILURE);
                break;
		}
        eliminar_paquete(paquete);
    }

}