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


void enviar_proceso_a_blocked(t_pcb* un_pcb,t_queue* cola)
{
   // un_pcb->estado_pcb = BLOCKED; //no utilizamos la funcion "cambiar_estado" para no manejas 2 listas de procesos bloqueados
    queue_push(cola,un_pcb);
    // poner semaforos
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
    
}

 
void atender_pedido_io(t_paquete* paquete, t_pcb* pcb_recibido){
        // podría ser que todo este CASE sea un "hilo" para que cada vez que se pida hacer una interfaz
        // se cree un hilo que lo maneje por separado, ya que como existen n interfaces, deberían
        // existin n hilos que se comuniquen con dichas interfaces
            t_peticion* peticion = recibir_peticion(paquete);  
            t_interfaz* interfaz = validar_peticion(peticion, pcb_recibido);
            enviar_proceso_a_blocked(pcb_recibido, interfaz->cola_procesos_blocked);
            gestionar_lista_de_interfaz(peticion, interfaz); 
            eliminar_peticion(peticion);
            recibir_fin_peticion();
            desbloquear_proceso(interfaz,pcb_recibido);
      }

void desbloquear_proceso(t_interfaz* interfaz){
        // poner semaforos
        t_pcb* un_pcb = queue_pop(interfaz->cola_procesos_blocked);
        cambiar_estado(un_pcb, READY);
        // signal(io->semaforo_cola_procesos_blocked);
}

/*FALTA INCLUIR BIEN A TODAS LAS LIBRERIAS,HACER "gestionar_lista_de_interfaz(param)" 
y también arreglar argumentos del hilo atender_pedido_io(t_paquete* paquete, t_pcb* pcb_recibido)
Declarar todas las funciones nuevas que creamos, chequearlas bien.
mili:
hacer quantum
poner semáforos en todos lados
y más cosas que no recuerdo
mergear bien "atender_kernel_cpu_dispatch"

*/



