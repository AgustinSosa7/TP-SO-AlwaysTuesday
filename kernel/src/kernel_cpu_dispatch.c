#include "../includes/kernel_cpu_dispatch.h"

void recibir_pcb_con_motivo()
{     int control_key = 1;
      while (control_key){
      int code_op = recibir_operacion(fd_cpu_dispatch);
      t_paquete* paquete = recibir_paquete(fd_cpu_dispatch);
      t_pcb* pcb_recibido = recibir_pcb(paquete);
      log_info(kernel_logger, "Se recibio algo de CPU_Dispatch : %d", code_op);
      switch (code_op)
      {
      case DESALOJO_QUANTUM:
            log_info(kernel_logger,"PID: <%d> - Desalojado por fin de Quantum",pcb_recibido->pid);
            cambiar_estado(pcb_recibido, READY);
            
            break;
      case PROCESO_EXIT:
            /* code */
            break;
      case PEDIDO_IO:          
            t_paquete_y_pcb* paquete_y_pcb = malloc(sizeof(t_paquete_y_pcb));
            paquete_y_pcb->paquete = paquete;
            paquete_y_pcb->un_pcb = pcb_recibido;   
            /*pthread_t pedido_io;
            pthread_create(&pedido_io, NULL, atender_pedido_io, paquete_y_pcb); //verificar como se envian estos parametros
            pthread_detach(pedido_io);*/
            break;
      case -1:
            log_error(kernel_logger, "Desconexion de CPU - DISPATCH");      
            control_key = 0;
            break;
      default:
            log_warning(kernel_logger, "Operacion desconocida de CPU - DISPATCH");
            break;
        }
        eliminar_paquete(paquete);    
      }
}










