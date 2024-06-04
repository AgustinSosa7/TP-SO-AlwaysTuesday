#include "../includes/kernel_cpu_interrupt.h"

void enviar_interrupción_a_cpu(){
    t_paquete* un_paquete = crear_paquete(INTERRUPCION);
    //agregar_algo_a_paquete(un_paquete,& );
	enviar_paquete(un_paquete,fd_cpu_dispatch);
    eliminar_paquete(un_paquete);
}

// void atender_kernel_cpu_interrupt(){
//     bool control_key = 1;
//     while (control_key) {
//         int cod_op = recibir_operacion(fd_cpu_interrupt);
//         switch (cod_op)
//         {
//         case MENSAJE:
//             //
//             break;
//         case -1:
//            // log_error(logger, "Desconexion de CPU - INTERRUPT");      
//             control_key = 0;
//         default:
//            // log_warning(logger, "Operacion desconocida de CPU - INTERRUPT");
//             break;
//         }
//     }
// }



