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


char* recibir_instruccion_pseudocodigo(){
    op_code code_op = recibir_operacion(fd_memoria);
    t_paquete* paquete = recibir_paquete(fd_memoria);
    t_buffer* buffer = paquete->buffer;
    if(code_op == PSEUDOCODIGO)
    {
        log_info(cpu_log_debug, "leer_string_del_stream");
        char* instruccion_pseudocodigo = leer_string_del_stream(buffer);//REVISAR POR QUE NO FUNCIONA
        log_info(cpu_log_debug, "free");
        free(paquete);
        return instruccion_pseudocodigo;
    }
    else
    {   
        log_error(cpu_log_debug, "No se recibio un pseudocodigo.");
        free(paquete);
        exit(EXIT_FAILURE);
    }
};