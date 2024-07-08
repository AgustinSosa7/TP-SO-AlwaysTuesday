#include "../includes/kernel_cpu_dispatch.h"

void recibir_pcb_con_motivo(){   

      detener_planificacion();
      printf("Esperando un PCB con MOTIVO.\n");
      int code_op;
      t_paquete* paquete;
      t_pcb* pcb_recibido;

      if(iniciar_tiempo_VRR){
            t_temporal* quantum_vrr = temporal_create();
            code_op = recibir_operacion(fd_cpu_dispatch);
            paquete = recibir_paquete(fd_cpu_dispatch);
            pcb_recibido = recibir_pcb(paquete); 
            log_info(kernel_logger, "Se recibio algo de CPU_Dispatch : %d", code_op);
            temporal_stop(quantum_vrr);
            pcb_recibido->tiempo_transcurrido = temporal_gettime(quantum_vrr);
            temporal_destroy(quantum_vrr);
      }else{    
 
            code_op = recibir_operacion(fd_cpu_dispatch);
            paquete = recibir_paquete(fd_cpu_dispatch);
            pcb_recibido = recibir_pcb(paquete); 
            log_info(kernel_logger, "Se recibio algo de CPU_Dispatch : %d", code_op);
      }

      detener_planificacion();
      switch (code_op){

      case DESALOJO:
            int motivo = leer_int_del_buffer(paquete->buffer);
            switch (motivo){
            case INTERRUPCION_POR_DESALOJO:
                  log_info(kernel_logger,"PID: <%d> - Desalojado por fin de Quantum.\n",pcb_recibido->pid);
                  t_pcb* un_pcb = list_remove(struct_exec->lista,0);
                  eliminar_pcb(un_pcb);
                  pcb_recibido->tiempo_transcurrido=0;
                  pcb_recibido->quantum = QUANTUM;
                  list_add(struct_exec->lista,pcb_recibido);
                  cambiar_de_estado_y_de_lista(EXEC,READY);
                  sem_post(&sem_planificador_corto_plazo);
                  break;
            case INTERRUPCION_POR_KILL:
                  cambiar_de_estado_y_de_lista(EXEC,EXIT);
                  eliminar_proceso(pcb_recibido,INTERRUPTED_BY_USER);
            default:
                  break;
            }  
      break;
      case PROCESO_EXIT:
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
            void* buff = paquete->buffer;
            char* recurso_solicitado = leer_string_del_buffer(buff);
            bool esta_o_no_el_recurso(void* recurso){
                 return(esta_el_recurso(recurso,recurso_solicitado));
            }
            
            if(list_any_satisfy(lista_recursos, esta_o_no_el_recurso)){
                  t_recursos* recurso = list_find(lista_recursos, esta_o_no_el_recurso);
                  //pthread_mutex_lock(&(recurso->mutex_recurso));
                  recurso->instancias = recurso->instancias -1;
                  if(recurso->instancias <0){
                       pthread_mutex_lock(&(struct_exec->mutex));
                       t_pcb* pcb = list_remove(struct_exec->lista,0);
                       pthread_mutex_unlock(&(struct_exec->mutex));
                       pcb_recibido->estado_pcb = BLOCKED;
                       if(pcb_recibido->tiempo_transcurrido < pcb_recibido->quantum){
                        pcb_recibido->quantum = (pcb_recibido->quantum -pcb_recibido->tiempo_transcurrido);
                       }
                       list_add(recurso->lista_procesos_bloqueados,pcb_recibido);
                       log_warning(kernel_logger, "PID: <%d> - Estado Anterior: <%s> - Estado Actual: <%s> \n",pcb_recibido->pid, enum_a_string(EXEC),enum_a_string(pcb_recibido->estado_pcb));
                       log_warning(kernel_logger,"PID: <%d> - Bloqueado por: <%s>\n",pcb_recibido->pid,recurso_solicitado);
                       sem_post(&sem_planificador_corto_plazo);
                  
                  } else{ 
                        list_add(recurso->lista_procesos_asignados,pcb_recibido);  
                        //t_pcb* pcb = list_get(recurso->lista_procesos_asignados,0);  
                        //log_warning(kernel_logger,"Lista de recuros tiene pcb:%d \n", pcb->pid);                         
                        log_warning(kernel_logger,"Se agrega a la lista de recursos: <%s>.\n",recurso_solicitado);
                        //t_pcb* pcb = list_get(lista_exec,0);
                        printf("Vuelvo a mandar el proceso <%d> a ejecutar\n",pcb_recibido->pid);
                        if(pcb_recibido->tiempo_transcurrido < pcb_recibido->quantum){
                        pcb_recibido->quantum = (pcb_recibido->quantum -pcb_recibido->tiempo_transcurrido);
                        }
                        enviar_pcb_a(pcb_recibido,fd_cpu_dispatch,PCB);
                        recibir_pcb_con_motivo();
                        //log_warning(kernel_logger,"Lista de exec, pid:%d \n", pcb->pid);
                  }
            } else{
                       cambiar_de_estado_y_de_lista(EXEC,EXIT);
                       eliminar_proceso(pcb_recibido, INVALID_RESOURCE);
            }
           break;
      case SIGNAL:
            void* buffer = paquete->buffer;
            char* recurso_solicitadoo = leer_string_del_buffer(buffer);
            bool estaa_o_no_el_recurso(void* recurso){
                 return(esta_el_recurso(recurso,recurso_solicitadoo));
            }
            if(list_any_satisfy(lista_recursos, estaa_o_no_el_recurso)){
                 t_recursos* recurso = list_find(lista_recursos, estaa_o_no_el_recurso);
                 recurso->instancias = recurso->instancias +1;
                 if(!list_is_empty(recurso->lista_procesos_bloqueados)){
                        t_pcb* un_pcb = list_remove(recurso->lista_procesos_bloqueados,0);
                        list_add(recurso->lista_procesos_asignados,un_pcb);
                        enviar_proceso_blocked_a_ready(un_pcb);
                        sem_post(&sem_planificador_corto_plazo);
                        
                  } 
                  if(pcb_recibido->tiempo_transcurrido < pcb_recibido->quantum){
                        pcb_recibido->quantum = (pcb_recibido->quantum -pcb_recibido->tiempo_transcurrido);
                       }
                  enviar_pcb_a(pcb_recibido,fd_cpu_dispatch,PCB);
                  recibir_pcb_con_motivo();
                  }else{
                        cambiar_de_estado_y_de_lista(EXEC,EXIT);
                        eliminar_proceso(pcb_recibido, INVALID_RESOURCE);
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
      //printf("nomre del recurso: %s\n",recurso->nombre_recurso);
      return(strcmp(recurso->nombre_recurso,recurso_solicitado)==0);
}





