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


void enviar_proceso_a_blocked(t_proceso_blocked* proceso_a_ejecutar, t_interfaz* interfaz)
{
    proceso_a_ejecutar->un_pcb->estado_pcb = BLOCKED;
    
    //pthread_mutex_lock(interfaz->mutex_cola_blocked);
    queue_push(interfaz->cola_procesos_blocked, proceso_a_ejecutar->un_pcb);
    //pthread_mutex_unlock(interfaz->mutex_cola_blocked);
    signal(interfaz->semaforo_cola_procesos_blocked);
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
      t_peticion* peticion = recibir_peticion(paquete);  
      t_interfaz* interfaz = validar_peticion(peticion, pcb_recibido);
      t_proceso_blocked* proceso_a_ejecutar;
      proceso_a_ejecutar->un_pcb = pcb_recibido;
      proceso_a_ejecutar->peticion = peticion;
      enviar_proceso_a_blocked(proceso_a_ejecutar, interfaz);
      eliminar_peticion(peticion);
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
