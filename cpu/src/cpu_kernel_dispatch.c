# include "../includes/cpu_kernel_dispatch.h"

void atender_dispatch(){
    log_info(cpu_logger, "Atendiendo a DISPATCH...");

    bool control_key = 1;
    while (control_key) {
		int cod_op = recibir_operacion(fd_kernel_dispatch);
        t_paquete* paquete;
        
		switch (cod_op) {
            case PCB:
                paquete = recibir_paquete(fd_kernel_dispatch);
                log_info(cpu_logger,"Llego un nuevo proceso (PCB) para ser ejecutado en el CPU!");
                establecer_pcb_global(paquete, pcb_global);
                //imprimir_pcb(pcb_global, cpu_logger);
                sem_post(&sem_ciclo_de_instruccion);
                break;
            
            case -1:
                log_warning(cpu_logger, "Desconexión de KERNEL!");
                control_key = 0;
                exit(EXIT_SUCCESS);
                break;

            default:
                log_error(cpu_logger,"Operacion desconocida de KERNEL-DISPATCH");
                exit(EXIT_FAILURE);
                break;
		}
        eliminar_paquete(paquete);
    }

}