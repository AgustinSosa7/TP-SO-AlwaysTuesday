#include "../includes/planificador_corto_plazo.h"
#include "../includes/kernel_entradaSalida.h"
#include "../includes/kernel_cpu_dispatch.h"
#include "../includes/kernel_cpu_interrupt.h"

void planif_corto_plazo()
{
    while(1){
        detener_planificacion();
        printf("WAIT del plani CORTO plazo.\n");
        sem_wait(&sem_planificador_corto_plazo);
        printf("Entro al plani corto plazo.\n");
        algoritmos_enum algoritmo = algoritmo_string_a_enum(ALGORITMO_PLANIFICACION);   
            switch ( algoritmo)
            {
            case FIFO:
                planif_fifo_RR();
                printf("FIFO realizado.\n");
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
    printf("Entro a FIFO/RR.\n");
    if(!list_is_empty(struct_ready->lista)){
        if(list_is_empty(struct_exec->lista)){
            cambiar_de_estado_y_de_lista(READY,EXEC);
            t_pcb* un_pcb = list_get(struct_exec->lista,0);
            enviar_pcb_a(un_pcb, fd_cpu_dispatch, PCB);
            printf("Envie el pcb a DISPATCH. \n");
            if(strcmp(ALGORITMO_PLANIFICACION,"RR") == 0){
                pthread_t hilo_quantum;
                pthread_create(&hilo_quantum, NULL, (void*)gestionar_quantum, un_pcb);
                pthread_detach(hilo_quantum);


            }
        recibir_pcb_con_motivo();  
        } else{printf("La cola de EXECUTE estaba OCUPADA :(\n");}
    } else{printf("La cola de READY estaba vacia :(\n");}
}
void gestionar_quantum(t_pcb* un_pcb){
    int contador_inicial = un_pcb->contador;
    usleep(un_pcb->quantum*1000);
    if(!list_is_empty(struct_exec->lista)){
        bool validar(void* otro_pcb){
        return mismo_contador(otro_pcb,contador_inicial);
    }
        if(contains_algo(struct_exec->lista, &(un_pcb->pid))&& (list_any_satisfy(struct_exec->lista,validar)) ){ 
        enviar_interrupción_a_cpu(SOLICITUD_INTERRUMPIR_PROCESO, INTERRUPCION_POR_DESALOJO); 
    }
    }
}   

bool mismo_contador(t_pcb* otro_pcb,int contador){
    return((otro_pcb->contador)==contador);
}
// VRR
void planif_VRR(){
    t_pcb* un_pcb;
    if(!list_is_empty(struct_ready_plus->lista)){
        if(list_is_empty(struct_exec->lista)){
        cambiar_de_estado_y_de_lista(READYPLUS,EXEC);
        pthread_mutex_lock(&(struct_exec->mutex));
        un_pcb = list_get(struct_exec->lista,0);
        pthread_mutex_unlock(&(struct_exec->mutex));
        enviar_pcb_a(un_pcb,fd_cpu_dispatch,PCB);
        pthread_t hilo_quantum_vrr;
        pthread_create(&hilo_quantum_vrr, NULL, (void*)gestionar_quantum, un_pcb);
        pthread_detach(hilo_quantum_vrr);
        pthread_mutex_lock(&mutex_VRR);
        iniciar_tiempo_VRR = true;
        pthread_mutex_unlock(&mutex_VRR);
        recibir_pcb_con_motivo();
        }
    } else if(!list_is_empty(struct_ready->lista)){
        if(list_is_empty(struct_exec->lista)){
        cambiar_de_estado_y_de_lista(READY,EXEC);
        pthread_mutex_lock(&(struct_exec->mutex));
        un_pcb = list_get(struct_exec->lista,0);
        pthread_mutex_unlock(&(struct_exec->mutex));
        enviar_pcb_a(un_pcb,fd_cpu_dispatch,PCB);
        pthread_t hilo_quantum_vrr;
        pthread_create(&hilo_quantum_vrr, NULL, (void*)gestionar_quantum, un_pcb);
        pthread_detach(hilo_quantum_vrr);
        pthread_mutex_lock(&mutex_VRR);
        iniciar_tiempo_VRR = true;
        pthread_mutex_unlock(&mutex_VRR);
        recibir_pcb_con_motivo();
        }
    }
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
