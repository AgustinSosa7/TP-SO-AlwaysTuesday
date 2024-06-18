# include "../includes/cpu_kernel_interrupt.h"

void atender_interrupciones(){
    log_info(cpu_logger, "Atendiendo a INTERRUPT...");

    bool control_key = 1;
    while (control_key) {
		int cod_op = recibir_operacion(fd_kernel_interrupt);
        t_paquete* paquete = recibir_paquete(fd_kernel_interrupt);
        t_buffer* buffer = paquete->buffer;
        
		switch (cod_op) {
            case SOLICITUD_INTERRUMPIR_PROCESO:
                log_debug(cpu_log_debug, "Recibi una interrupción (Fin de Quantum)!");
                motivo_interrupcion = leer_int_del_buffer(buffer);
                
                pthread_mutex_lock(&mutex_ocurrio_interrupcion);
			    ocurrio_interrupcion = true;
			    pthread_mutex_unlock(&mutex_ocurrio_interrupcion); 
                break;
            
            case -1:
                log_error(cpu_logger, "Desconexión de KERNEL - Interrupt");
                control_key = 0;
            
            default:
                log_warning(cpu_logger,"Operacion desconocida de KERNEL - Interrupt");
                exit(EXIT_FAILURE);
                break;
		}
    }
}