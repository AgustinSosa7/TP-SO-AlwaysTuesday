#include "../includes/kernel_entradaSalida.h"


void atender_kernel_entradaSalida(){
    bool control_key = 1;
    while (control_key) {
        int cod_op = recibir_operacion(fd_entradasalida);
        switch (cod_op)
        {
        case MENSAJE:
            //
            break;
        case -1:
            log_error(cpu_logger, "Desconexion de ENTRADASALIDA");      
            control_key = 0;
        default:
            log_warning(cpu_logger, "Operacion desconocida de ENTRADASALIDA");
            break;
        }
    }
}