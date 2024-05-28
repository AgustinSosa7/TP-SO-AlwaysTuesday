#include "../includes/cpu_memoria.h"


void atender_cpu_memoria(){
    bool control_key = 1;
    while (control_key) {
        int cod_op = recibir_operacion(fd_memoria);
        switch (cod_op)
        {
        case MENSAJE:
            //
            break;
        case -1:
            log_error(cpu_logger, "Desconexion de Memoria");      
            control_key = 0;
        default:
            log_warning(cpu_logger, "Operacion desconocida de Memoria");
            break;
        }
    }
}


char* recibir_direccion_pseudocodigo(){ //CODIGO REPETIDO EN MEMORIA_PROCESOS
    op_code code_op = recibir_operacion(fd_memoria);
    t_paquete* paquete = recibir_paquete(fd_memoria);
    t_buffer* buffer = paquete->buffer;
    log_info(cpu_log_debug, "Recibi un paquete de size: %d",paquete->buffer->size);
    log_info(cpu_log_debug, "Recibi un paquete con el op_code: %d",code_op);
    if(code_op == PSEUDOCODIGO)
    {
        log_info(cpu_log_debug, "leer_string_del_stream");
        char* direccion_pseudocodigo = leer_string_del_stream(buffer);//REVISAR POR QUE NO FUNCIONA
        log_info(cpu_log_debug, "free");
        free(paquete);
        return direccion_pseudocodigo;
    }
    else
    {   
        log_error(cpu_log_debug, "No se recibio un pseudocodigo.");
        free(paquete);
        exit(EXIT_FAILURE);
    }
};