#include "../includes/kernel_memoria.h"
#include <../src/utils/utils_connections.h>



void atender_kernel_memoria(){
    bool control_key = 1;
    while (control_key) {
        int cod_op = recibir_operacion(fd_memoria);
        switch (cod_op)
        {
        case MENSAJE:
            //
            break;
        case -1:
          //  log_error(logger, "Desconexion de MEMORIA ");      
            control_key = 0;
        default:
          //  log_warning(logger, "Operacion desconocida de MEMORIA");
            break;
        }
    }
}

void enviar_direccion_pesudocodigo(char* pseudocogido,int pid){
      // Se comentó lo que era para probar :D
      //log_info(kernel_log_debug,"creo paquete...");
      t_paquete* paquete = crear_paquete(CREAR_PROCESO);
      //log_info(kernel_log_debug,"agrego el string al paquete...");
      
      agregar_algo_a_paquete(paquete,pid,sizeof(pid));

      agregar_string_a_paquete(paquete, pseudocogido);  



      //log_info(kernel_log_debug,"envio el  paquete");
      enviar_paquete(paquete, fd_memoria);
      //log_info(kernel_log_debug,"elimino el paquetede memoria...");
      eliminar_paquete(paquete);
      //log_info(kernel_log_debug,"Listo c:");
}