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

 void gestionar_quantum_VRR(){
    
 }
// VRR
 void planif_VRR(){
    if(!list_is_empty(lista_ready_plus)){
        t_pcb* un_pcb = list_remove(lista_ready_plus);
        cambiar_estado(un_pcb, READYPLUS);
        enviar_pcb_a(un_pcb,fd_cpu_dispatch);
        pthread_t hilo_quantum_VRR;
        pthread_create(&hilo_quantum_VRR, NULL, (void*)gestionar_quantum_VRR, un_pcb);
    }
 }

void recibir_pcb_con_motivo()
{ //falta declarar funcion en .h
    int code_op = recibir_operacion(fd_cpu_dispatch);
    t_paquete* paquete = recibir_paquete(fd_cpu_dispatch);
    t_pcb* pcb_recibido = recibir_pcb(paquete);
    log_info(kernel_logger, "Se recibio algo de CPU_Dispatch : %d", code_op);
    switch (code_op)
    {
    case DESALOJO_QUANTUM:
        log_info(kernel_logger,"PID: <%d> - Desalojado por fin de Quantum",pcb->pid);
        cambiar_estado(pcb_recibido, READY);
        
        break;
    case PROCESO_EXIT:
        /* code */
        break;
    case PEDIDO_IO:               
        //pthread_t pedido_io;
        //pthread_create(&pedido_io, NULL, atender_pedido_io,(paquete,pcb_recibido)); //verificar como se envian estos parametros
        //pthread_detach(pedido_io);
        atender_pedido_io(paquete,pcb_recibido);
        break;
    default:
        break;
    }
}

void gestionar_quantum(t_pcb* un_pcb){
// proceso finaliza antes de que termine el quantum y vuelve a entrar
    sleep(QUANTUM);
        if(contains_algo(lista_exec, un_pcb->pid)){
        //enviar interrupcion a cpu por interrupt
        
    }
    

void enviar_proceso_a_blocked(t_proceso_blocked* proceso_a_ejecutar, t_interfaz* interfaz)
{
    proceso_a_ejecutar->un_pcb->estado_pcb = BLOCKED;
    
    //pthread_mutex_lock(interfaz->mutex_cola_blocked);
    queue_push(interfaz->cola_procesos_blocked, proceso_a_ejecutar->un_pcb);
    //pthread_mutex_unlock(interfaz->mutex_cola_blocked);
    signal(interfaz->semaforo_cola_procesos_blocked);
}

 
void atender_pedido_io(t_paquete* paquete, t_pcb* pcb_recibido){
      t_peticion* peticion = recibir_peticion(paquete);  
      t_interfaz* interfaz = validar_peticion(peticion, pcb_recibido);
      t_proceso_blocked* proceso_a_ejecutar;
      proceso_a_ejecutar->un_pcb = pcb_recibido;
      proceso_a_ejecutar->peticion = peticion;
      enviar_proceso_a_blocked(proceso_a_ejecutar, interfaz);
      eliminar_peticion(peticion);
}






