#include <../includes/planificador_corto_plazo.h>

void planif_corto_plazo(){
   
        switch (ALGORITMO_PLANIFICACION)
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

t_paquete* recibir_pcb_con_motivo(){
    int code_op = recibir_operacion(fd_cpu_dispatch);
    t_paquete* paquete = recibir_paquete(fd_cpu_dispatch);
    t_pcb* pcb_recibido = recibir_pcb(paquete);
    switch (code_op)
    {
    case DESALOJO_QUANTUM:
        /* code */
        break;
    case PROCESO_EXIT:
        /* code */
        break;
    case PEDIDO_IO:
        list_add(lista_blocked, pcb_recibido);
        break;
    default:
        break;
    }
}


 
  
 planif_fifo(){

    if(!list_is_empty(lista_ready)){
        if(list_is_empty(lista_exec)){
            t_pcb* un_pcb = list_remove(lista_ready,0);
            cambiar_estado(un_pcb,EXEC);
            //agregar msje
            _enviar_pcb_a_CPU_por_dispatch(un_pcb);

            //recibir_pcb_con_motivo();

            //switch(motivo desalojo)
            //
        }
        
 }

 planif_RR(){
//enviar pcb a cpu por dispatch
//iniciar quantum
//enviar interrupcion a cpu por interrupt
//recibir_pcb_con_motivo();
 }
 planif_VRR(){


 }

