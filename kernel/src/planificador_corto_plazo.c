#include <../includes/planificador_corto_plazo.h>
#include "../includes/kernel_entradaSalida.h"
#include "../includes/kernel_cpu_dispatch.h"

void planif_corto_plazo()
{
    int ALGORITMO_PLANIFICACION = 1;   // CLARAMENTE DEBE DE RECIBIR DE ALGUN LADO EL ALGORITMO SOLICITADO
        switch (ALGORITMO_PLANIFICACION)
        {
        case FIFO:
            planif_fifo();
            break;
        case RR:
            planif_RR();
            break;
        case  VRR:
            planif_VRR();
            break;
        default:
            break;
        }
    }

t_paquete* recibir_pcb_con_motivo()
{ //falta declarar funcion en .h
    int code_op = recibir_operacion(fd_cpu_dispatch);
    t_paquete* paquete = recibir_paquete(fd_cpu_dispatch);
    t_pcb* pcb_recibido = recibir_pcb(paquete);
    log_info(kernel_logger, "Se recibio algo de CPU_Dispatch : %d", code_op);
    switch (code_op)
    {
    case DESALOJO_QUANTUM:
        /* code */
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


void enviar_proceso_a_blocked(t_pcb* un_pcb,t_queue* cola)
{
   // un_pcb->estado_pcb = BLOCKED; //no utilizamos la funcion "cambiar_estado" para no manejas 2 listas de procesos bloqueados
    queue_push(cola,un_pcb);
    // poner semaforos
}

  
void planif_fifo()
{

    if(!list_is_empty(lista_ready)){
        if(list_is_empty(lista_exec)){
            //semaforos 
            t_pcb* un_pcb = list_remove(lista_ready,0);
            cambiar_estado(un_pcb, EXEC);
            //agregar msje
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
 }
 void planif_VRR(){


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
