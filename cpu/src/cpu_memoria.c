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