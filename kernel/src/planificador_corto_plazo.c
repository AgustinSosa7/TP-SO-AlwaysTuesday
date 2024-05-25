#include <../includes/planificador_corto_plazo.h>

void planif_corto_plazo(){
        switch(ALGORITMO_PLANIFICACION)
        {
        case "FIFO":
                planif_fifo();
            break;
        case "RR":
                planif_RR();
            break;
            case "VRR":
                planif_VRR();
            break;
        default:
            break;
        }
    }


void planif_fifo(){
    if(!list_is_empty(lista_ready)){
        if(list_is_empty(lista_exec)){
            t_pcb* un_pcb = list_remove(lista_ready,0);
            cambiar_estado(un_pcb,EXEC);
            _enviar_pcb_a_CPU_por_dispatch(un_pcb);

            //recibir_pcb_con_motivo();
            //switch(motivo desalojo)
            //
        }
        
 }

void planif_RR(){
//enviar pcb a cpu por dispatch
//iniciar quantum
//enviar interrupcion a cpu por interrupt
//recibir_pcb_con_motivo();
 };
void planif_VRR(){
 };

