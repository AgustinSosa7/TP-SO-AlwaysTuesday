#include "../includes/planificador_largo_plazo.h"



void planif_largo_plazo(){
   while(1){
      sem_wait(&sem_new_a_ready);
 	    sem_wait(&sem_grado_multiprogram);
      if(!list_is_empty(lista_new)){
      t_pcb* un_pcb = list_remove(lista_new,0);
      cambiar_estado(un_pcb, READY);
      log_info(kernel_logger,"PID: < %d > - Estado Anterior: < NEW > - Estado Actual: <READY>", un_pcb->pid);
      sem_post(&sem_planificador_corto_plazo);
         }
      }
   }
