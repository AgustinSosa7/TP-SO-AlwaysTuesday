#include "../includes/kernel_cpu_interrupt.h"


void atender_kernel_cpu_interrupt(){
    bool control_key = 1;
    while (control_key) {
        int cod_op = recibir_operacion(fd_cpu_interrupt);
        switch (cod_op)
        {
        case MENSAJE:
            //
            break;
        case -1:
           // log_error(logger, "Desconexion de CPU - INTERRUPT");      
            control_key = 0;
        default:
           // log_warning(logger, "Operacion desconocida de CPU - INTERRUPT");
            break;
        }
    }
}