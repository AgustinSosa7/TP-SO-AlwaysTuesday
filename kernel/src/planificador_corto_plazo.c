#include "../includes/planificador_corto_plazo.h"
#include "../includes/kernel_entradaSalida.h"
#include "../includes/kernel_cpu_dispatch.h"
#include "../includes/kernel_cpu_interrupt.h"

void planif_corto_plazo()
{
    while(1){
        detener_planificacion();
        printf("WAIT sem_planificador_corto_plazo");
        sem_wait(&sem_planificador_corto_plazo);
        printf("entro al plani corto plazo.\n");
        algoritmos_enum algoritmo = algoritmo_string_a_enum(ALGORITMO_PLANIFICACION);   
            switch (algoritmo)
            {
            case FIFO:
                planif_fifo_RR();
                printf("YA hice fifardo");
                break;
            case RR:
                planif_fifo_RR();
                break;
            case  VRR:
                planif_VRR();
                break;
            default:
                break;
            }
        }
}

void planif_fifo_RR()
{
    printf("Estamos en fifo.\n");
    if(!list_is_empty(lista_ready)){
        if(list_is_empty(lista_exec)){
            cambiar_de_estado_y_de_lista(READY,EXEC);
            t_pcb* un_pcb = list_get(lista_exec,0);
            enviar_pcb_a(un_pcb, fd_cpu_dispatch, PCB);
            if(strcmp(ALGORITMO_PLANIFICACION,"RR") == 0){
                pthread_t hilo_quantum;
                pthread_create(&hilo_quantum, NULL, (void*)gestionar_quantum, un_pcb);
                pthread_detach(hilo_quantum);


            }
        recibir_pcb_con_motivo();  
        }
        
    }
}
void gestionar_quantum(t_pcb* un_pcb){
    usleep(un_pcb->quantum*1000);
        if(contains_algo(lista_exec, &(un_pcb->pid))){ 
        //enviar_interrupción_a_cpu(INTERRUPCION_FIN_QUANTUM); 
        un_pcb->quantum = QUANTUM;

    }
}   

// VRR
  void planif_VRR(){
    t_pcb* un_pcb;
    if(!list_is_empty(lista_ready_plus)){
        cambiar_de_estado_y_de_lista(READYPLUS,EXEC); 
    } else if(!list_is_empty(lista_ready)){
        cambiar_de_estado_y_de_lista(READY,EXEC); 
    }
    un_pcb = list_get(lista_exec,0);
    enviar_pcb_a(un_pcb,fd_cpu_dispatch,PCB);
    pthread_t hilo_quantum_VRR;
    pthread_create(&hilo_quantum_VRR, NULL, (void*)gestionar_quantum_VRR, un_pcb);
    pthread_detach(hilo_quantum_VRR);
 } 


 void gestionar_quantum_VRR(t_pcb* un_pcb){
    t_temporal* quantum_vrr = temporal_create();
    pthread_t hilo_quantum_vrr;
    pthread_create(&hilo_quantum_vrr, NULL, (void*)gestionar_quantum, un_pcb);
    pthread_detach(hilo_quantum_vrr);
    recibir_pcb_con_motivo();
    temporal_stop(quantum_vrr);
    tiempo_transcurrido = temporal_gettime(quantum_vrr);
    temporal_destroy(quantum_vrr);
 }



 algoritmos_enum algoritmo_string_a_enum(char* algoritmo_de_plani){
    if(strcmp("FIFO",algoritmo_de_plani)==0){
        return FIFO;
    } else if(strcmp("RR",algoritmo_de_plani)==0){
        return RR;
    } else {
        return VRR;
    }

 }
