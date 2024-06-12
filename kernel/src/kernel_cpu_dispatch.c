#include "../includes/kernel_cpu_dispatch.h"

void recibir_pcb_con_motivo()
{     int control_key = 1;
      while (control_key){
      detener_planificacion();
      int code_op = recibir_operacion(fd_cpu_dispatch);
      t_paquete* paquete = recibir_paquete(fd_cpu_dispatch);
      t_pcb* pcb_recibido = recibir_pcb(paquete);
      log_info(kernel_logger, "Se recibio algo de CPU_Dispatch : %d", code_op);
      detener_planificacion();
      switch (code_op)
      {
      case DESALOJO_QUANTUM:
            log_info(kernel_logger,"PID: <%d> - Desalojado por fin de Quantum",pcb_recibido->pid);
            cambiar_de_estado_y_de_lista(EXEC,READY);
            
            break;
      case PROCESO_EXIT:
            cambiar_de_estado_y_de_lista(EXEC,EXIT);
            eliminar_proceso(pcb_recibido->pid);
            break;
      case PEDIDO_IO:          
            t_peticion* peticion = recibir_peticion(paquete);  
            t_interfaz* interfaz = validar_peticion(peticion, pcb_recibido);
            if(interfaz!=NULL){
            t_peticion_pcb_interfaz* peticion_pcb_interfaz = malloc(sizeof(t_peticion_pcb_interfaz));
            peticion_pcb_interfaz->peticion = peticion;
            peticion_pcb_interfaz->un_pcb = pcb_recibido;
            peticion_pcb_interfaz->interfaz = interfaz; 

            enviar_proceso_a_blocked(peticion_pcb_interfaz); 
            
            }

            break;
      //case WAIT:
      //      //preguntar por el parametro paquete->buffer
      //      char* recurso_solicitado = leer_string_del_stream(paquete->buffer);
      //      bool esta_o_no_el_recurso(t_recursos* recurso){
      //            return(esta_el_recurso(recurso,recurso_solicitado));
      //      }
      //      if(list_any_satisfy(lista_recursos, esta_o_no_el_recurso)){
      //           t_recurso* recurso = list_find(lista_recursos, esta_o_no_el_recurso);
      //           recurso->instancias = recurso->instancias -1;
      //            if(recurso->instancias <0){
      //                  t_pcb* pcb = list_remove_(lista_exec, 0);//o mejor list_remove_and_destroy_element ?
      //                  pcb_recibido->estado_pcb = BLOCKED;
      //                  queue_push(recurso->cola_recursos_bloqueados,pcb_recibido);
      //                  //signal sem plani corto plazo?
      //            }else{ //debería volver a ejecutar el mismo proceso.
//
      //            }
//
      //      } else{
      //                  cambiar_de_estado_y_de_lista(EXEC,EXIT);
      //                  eliminar_proceso(pcb_recibido->pid);
      //            }
      //      break;
      //case SIGNAL:
      //      char* recurso_solicitado = leer_string_del_stream(paquete->buffer);
      //      bool esta_o_no_el_recurso(t_recursos* recurso){
      //            return(esta_el_recurso(recurso,recurso_solicitado));
      //      }
      //      if(list_any_satisfy(lista_recursos, esta_o_no_el_recurso)){
      //           t_recurso* recurso = list_find(lista_recursos, esta_o_no_el_recurso);
      //           recurso->instancias = recurso->instancias +1;
      //           if(!queue_is_empty(recurso->cola_recursos_bloqueados)){
      //            t_pcb* un_pcb = queue_pop(recurso->cola_recursos_bloqueados);
      //            enviar_pcb_a
      //           }
      //      }else {
      //            cambiar_de_estado_y_de_lista(EXEC,EXIT);
      //             eliminar_proceso(pcb_recibido->pid);
      //            }
      //            //deberia hacer un semaforo que me bloquee en cpu y acá hago un post! supongo
      //      break;
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

bool esta_el_recurso(t_recursos* recurso, char* recurso_solicitado){
      return(strcmp(recurso->nombre_recurso,recurso_solicitado)==0);
}





