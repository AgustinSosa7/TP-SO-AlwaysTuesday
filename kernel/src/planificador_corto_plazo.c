#include <../includes/planificador_corto_plazo.h>
#include "../includes/kernel_entradaSalida.h"
#include "../includes/kernel_cpu_dispatch.h"

void planif_corto_plazo()
{
    int ALGORITMO_PLANIFICACION = 1;   // CLARAMENTE DEBE DE RECIBIR DE ALGUN LADO EL ALGORITMO SOLICITADO
        switch (ALGORITMO_PLANIFICACION)
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



void planif_fifo_RR()
{

    if(!list_is_empty(lista_ready)){
        if(list_is_empty(lista_exec)){
            //semaforos 
            t_pcb* un_pcb = list_remove(lista_ready,0);
            cambiar_estado(un_pcb, EXEC);
            
            enviar_pcb_a(un_pcb,fd_cpu_dispatch);

            if(strcmp(ALGORITMO_PLANIFICACION,"RR") == 0){
                pthread_t hilo_quantum;
                pthread_create(&hilo_quantum, NULL, (void*)gestionar_quantum, un_pcb);
            }
             recibir_pcb_con_motivo();
        }
        
    }
}

 void gestionar_quantum_VRR(){
    
 }
// VRR
 void planif_VRR(){
    if(!list_is_empty(lista_ready_plus)){
        t_pcb* un_pcb = list_remove(lista_ready_plus, 0); // falta agregarle un numero correcto (el 0 no estoy seguro si va)
        cambiar_estado(un_pcb, READYPLUS);
        enviar_pcb_a(un_pcb,fd_cpu_dispatch);
        pthread_t hilo_quantum_VRR;
        pthread_create(&hilo_quantum_VRR, NULL, (void*)gestionar_quantum_VRR, un_pcb);
    }
 }


void gestionar_quantum(t_pcb* un_pcb){
// proceso finaliza antes de que termine el quantum y vuelve a entrar
    sleep(QUANTUM);
        if(contains_algo(lista_exec, un_pcb->pid)){ 
        //enviar interrupcion a cpu por interrupt
        
    }
}   








