#include <../includes/planificador_largo_plazo.h>



void planif_largo_plazo(){
   while(1){
      sem_wait(sem_new_a_ready);
 	   sem_wait(sem_grado_multiprogram);
      if(!queue_is_empty(cola_new)){
      t_pcb* un_pcb = queue_pop(cola_new);
      cambiar_estado(un_pcb, READY);
      queue_push(cola_ready,un_pcb);
      log_info(kernel_logger,"PID: < %d > - Estado Anterior: < NEW > - Estado Actual: <READY>", un_pcb->pid);
      sem_post(sem_planificador_corto_plazo);
         }
      }     
   }
