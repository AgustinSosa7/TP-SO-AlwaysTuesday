#include "../includes/kernel_cpu_dispatch.h"

void recibir_pcb_con_motivo()
{     
      detener_planificacion();
      printf("Esperando un PCB con MOTIVO.\n");
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
            sem_post(&sem_planificador_corto_plazo);
            
            break;
      case DEVOLVER_PROCESO_POR_CORRECTA_FINALIZACION:
            cambiar_de_estado_y_de_lista(EXEC,EXIT);
            eliminar_proceso(pcb_recibido,SUCCESS);
            break;
      case PEDIDO_IO:          
            t_peticion* peticion = recibir_peticion(paquete);  
            t_interfaz* interfaz = validar_peticion(peticion);
            if(interfaz!=NULL){
            enviar_proceso_a_blocked(peticion,pcb_recibido,interfaz); 
            sem_post(&sem_planificador_corto_plazo);
            }

            break;
      case WAIT:
           //preguntar por el parametro paquete->buffer
            char* recurso_solicitado = leer_string_del_buffer(paquete->buffer);
            bool esta_o_no_el_recurso(void* recurso){
                 return(esta_el_recurso(recurso,recurso_solicitado));
            }
            if(list_any_satisfy(lista_recursos, esta_o_no_el_recurso)){
                  t_recursos* recurso = list_find(lista_recursos, esta_o_no_el_recurso);
                  recurso->instancias = recurso->instancias -1;
                  if(recurso->instancias <0){
                       list_remove_element(lista_exec,pcb_recibido);
                       pcb_recibido->estado_pcb = BLOCKED;
                       list_add(recurso->lista_procesos_bloqueados,pcb_recibido);
                       sem_post(&sem_planificador_corto_plazo);
                  } else{ 
                        list_add(recurso->lista_procesos_asignados,pcb_recibido->pid);
                        enviar_pcb_a(pcb_recibido,fd_cpu_dispatch,PCB);
                  }
            } else{
                       cambiar_de_estado_y_de_lista(EXEC,EXIT);
                       eliminar_proceso(pcb_recibido, INVALID_RESOURCE);
            }
           break;
      case SIGNAL:
            char* recurso_solicitadoo = leer_string_del_buffer(paquete->buffer);
            bool estaa_o_no_el_recurso(void* recurso){
                 return(esta_el_recurso(recurso,recurso_solicitadoo));
            }
            if(list_any_satisfy(lista_recursos, estaa_o_no_el_recurso)){
                 t_recursos* recurso = list_find(lista_recursos, estaa_o_no_el_recurso);
                 recurso->instancias = recurso->instancias +1;
                 if(!list_is_empty(recurso->lista_procesos_bloqueados)){
                        t_pcb* un_pcb = list_remove(recurso->lista_procesos_bloqueados,0);
                        list_add(recurso->lista_procesos_asignados,un_pcb->pid);
                        enviar_proceso_blocked_a_ready(un_pcb);
                        enviar_pcb_a(pcb_recibido,fd_cpu_dispatch,PCB);
                  } else{
                        cambiar_de_estado_y_de_lista(EXEC,EXIT);
                        eliminar_proceso(pcb_recibido, INVALID_RESOURCE);
                  }
            }
            break;
      case DEVOLVER_PROCESO_POR_PAGEFAULT:
            break;
      case DEVOLVER_PROCESO_POR_OUT_OF_MEMORY:
            break;

      case -1:
            log_error(kernel_logger, "Desconexion de CPU - DISPATCH");      
            break;
      default:
            log_warning(kernel_logger, "Operacion desconocida de CPU - DISPATCH");
            break;
        }
        eliminar_paquete(paquete);
}

bool esta_el_recurso(t_recursos* recurso, char* recurso_solicitado){
      return(strcmp(recurso->nombre_recurso,recurso_solicitado)==0);
}





