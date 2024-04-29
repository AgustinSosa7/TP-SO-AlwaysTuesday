# include "../includes/cpu_kernel_dispatch.h"


void atender_cpu_kernel_dispatch(){

    bool control_key = 1;
    while (control_key) {
		int cod_op = recibir_operacion(fd_kernel_dispatch);
		t_buffer* unBuffer;
		switch (cod_op) {
            case MENSAJE:
                //recibir_mensaje(cliente_fd);
                break;
            case PAQUETE:
                //
                break;
			case EJECUTAR_PROCESO_KC: //Me debe llegar: [---][PID][Ticket][PC_program_counter][AX][BX][CX][DX]
				//unBuffer = recibiendo_super_paquete(fd_kernel_dispatch);
				//ejecutar_en_un_hilo_nuevo_detach((void*)atender_proceso_del_kernel, unBuffer);
				log_info(cpu_logger, "Se ejecuta algo XD");
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
/*
void atender_cpu_dispatch(){
	fd_kernel_dispatch = esperar_cliente(cpu_logger, "Kernel por dispatch", server_fd_cpu_dispatch);
	gestionar_handshake_como_server(fd_kernel_dispatch, cpu_logger);
	log_warning(cpu_logger, "::::::::::: KERNEL CONECTADO POR DISPATCH ::::::::::::");
	while(1){
		int cod_op = recibir_operacion(fd_kernel_dispatch);
		t_buffer* unBuffer;
		//log_info(cpu_logger, "Se recibio algo de KERNEL");

		switch (cod_op) {
		case EJECUTAR_PROCESO_KC: //Me debe llegar: [---][PID][Ticket][PC_program_counter][AX][BX][CX][DX]
			unBuffer = recibiendo_super_paquete(fd_kernel_dispatch);
			ejecutar_en_un_hilo_nuevo_detach((void*)atender_proceso_del_kernel, unBuffer);

			break;
		case RESPUESTA_INSTRUCCION_KC: //POr aca contesta el kernel los pedido de SIGNAL/WAIT
			unBuffer = recibiendo_super_paquete(fd_kernel_dispatch);
			//TODO: completar todos los caminos posibles de las respuestas a las instrucciones

			// recibo motivo[string] , estado[int]
			//char* instruccion = recibir_string_del_buffer(unBuffer);  por ahora no es necesario, si se agrega avisar a luca
			int estadoInstruccion = recibir_int_del_buffer(unBuffer);
			if(estadoInstruccion == -1){
				log_warning(cpu_logger, "PID: <%d> - Voy a tener que desaloajr %d",contexto->proceso_pid,  hay_que_desalojar_sin_mensaje);

				hay_que_desalojar_sin_mensaje = true;
			}
			sem_post(&sem_control_respuesta_kernel);
//			if(strcmp(instruccion, "WAIT") == 0){
//				if(estadoInstruccion == -1){
//					hay_que_desalojar_sin_mensaje = true;
//				}else{
//					sem_post(&sem_control_respuesta_kernel);
//				}
//			}else if(strcmp(instruccion, "SIGNAL") == 0){
//				if(estadoInstruccion == -1){
//					hay_que_desalojar_sin_mensaje = true;
//				}else{
//					sem_post(&sem_control_respuesta_kernel);
//				}
//			}else if(strcmp(instruccion, "F_OPEN") == 0){
//				if(estadoInstruccion == -1){
//					hay_que_desalojar_sin_mensaje = true;
//				}else{
//					sem_post(&sem_control_respuesta_kernel);
//				}
//			}else if(strcmp(instruccion, "F_CLOSE") == 0){
//				if(estadoInstruccion == -1){
//					hay_que_desalojar_sin_mensaje = true;
//				}else{
//					sem_post(&sem_control_respuesta_kernel);
//				}
//			}else if(strcmp(instruccion, "F_SEEK") == 0){
//				if(estadoInstruccion == -1){
//					hay_que_desalojar_sin_mensaje = true;
//				}else{
//					sem_post(&sem_control_respuesta_kernel);
//				}
//			}
			break;
		case MENSAJES_POR_CONSOLA: //Por aca contesta el kernel los pedido de SIGNAL/WAIT
			unBuffer = recibiendo_super_paquete(fd_kernel_dispatch);
			//post(sem)
			//atender_mensajes_kernel_v2(unBuffer, "Dispatch");
			break;
		case -1:
			log_error(cpu_logger, "[DESCONEXION]: KERNEL_Dispatch");
			exit(EXIT_FAILURE);
			break;
		default:
			log_warning(cpu_logger, "Operacion desconocida KERNEL_Dispatch");
			//free(unBuffer);
			break;
		}
	}
	log_warning(cpu_logger, "Saliendo del hilo de CPU_DISPATCH - KERNEL");
}
*/