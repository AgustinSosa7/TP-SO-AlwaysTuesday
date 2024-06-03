#include <../includes/planificador_corto_plazo.h>
#include "../includes/kernel_entradaSalida.h"
#include "../includes/kernel_cpu_dispatch.h"
#include "../includes/kernel_cpu_interrupt.h"

void planif_corto_plazo()
{
    while(1){
        sem_wait(&sem_planificador_corto_plazo);
        int algor = 1;   
            switch (algor)
            {
            case FIFO:
                planif_fifo_RR();
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

    if(!queue_is_empty(cola_ready)){
        if(queue_is_empty(lista_exec)){

            t_pcb* un_pcb = queue_pop(cola_ready);
            cambiar_estado(un_pcb, EXEC);
            
            enviar_pcb_a(un_pcb,fd_cpu_dispatch);
            recibir_pcb_con_motivo();
            if(strcmp(ALGORITMO_PLANIFICACION,"RR") == 0){
                pthread_t hilo_quantum;
                pthread_create(&hilo_quantum, NULL, (void*)gestionar_quantum, un_pcb);
                pthread_detach(hilo_quantum);
                recibir_pcb_con_motivo();

            }
               
        }
        
    }
}
void gestionar_quantum(t_pcb* un_pcb){
    usleep(QUANTUM*1000);
        if(contains_algo(lista_exec, &(un_pcb->pid))){ 
        enviar_interrupción_a_cpu();


    }
}   

// VRR
  void planif_VRR(){
    t_pcb* un_pcb;
    if(!queue_is_empty(cola_ready_plus)){
        un_pcb = queue_pop(cola_ready_plus); 
        cambiar_estado(un_pcb,READYPLUS);
        enviar_pcb_a(un_pcb,fd_cpu_dispatch);
    
    } else if(!queue_is_empty(cola_ready)){
        un_pcb = queue_pop(cola_ready);
        cambiar_estado(un_pcb,READY);
        enviar_pcb_a(un_pcb,fd_cpu_dispatch);
    }
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
    int tiempo_transcurrido = temporal_gettime(quantum_vrr);
    temporal_destroy(quantum_vrr);
    // if(tiempo_transcurrido < un_pcb->quantum){
    //     un_pcb->quantum = un_pcb->quantum - tiempo_transcurrido;
    //     queue_push(cola_ready_plus,un_pcb);
    // } TENGO DUDAS SOBRE CUANDO AGREGARLO A LA COLA DE READY+


 }






