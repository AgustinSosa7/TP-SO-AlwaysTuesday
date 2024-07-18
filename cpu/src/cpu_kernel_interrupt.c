# include "../includes/cpu_kernel_interrupt.h"

void atender_interrupt(){
    log_info(cpu_logger, "Atendiendo a INTERRUPT...");

    bool control_key = 1;
    while (control_key) {
		int cod_op = recibir_operacion(fd_kernel_interrupt);
        t_paquete* paquete;
        
		switch (cod_op) {
            case SOLICITUD_INTERRUMPIR_PROCESO:
                paquete = recibir_paquete(fd_kernel_interrupt);
                log_warning(cpu_logger, "Recibi una interrupción desde Kernel!");

                motivo_interrupcion = leer_int_del_buffer(paquete->buffer);
                imprimir_motivo_de_interrupcion(motivo_interrupcion);

                pthread_mutex_lock(&mutex_ocurrio_interrupcion);
			    ocurrio_interrupcion = true;
			    pthread_mutex_unlock(&mutex_ocurrio_interrupcion); 
                break;
            
            case -1:
                log_warning(cpu_logger, "Desconexión de KERNEL!");
                control_key = 0;
                exit(EXIT_SUCCESS);
                break;

            default:
                log_error(cpu_logger,"Operacion desconocida de KERNEL-INTERRUPT");
                exit(EXIT_FAILURE);
                break;
		}
        eliminar_paquete(paquete);
    }
}

void imprimir_motivo_de_interrupcion (int motivo)
{
    if(motivo == 0) {
        log_warning(cpu_logger, "Motivo de la interrupción = Fin de Quantum (Desalojo)");}
    else if(motivo == 1) {
        log_warning(cpu_logger, "Motivo de la interrupción = Terminar proceso (Kill)");}
}