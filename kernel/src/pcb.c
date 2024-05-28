#include "../includes/pcb.h"
#include <../src/utils/utils_connections.h>


#include <semaphore.h>

t_pcb* crearPcb(){
	t_pcb* pcb = malloc(sizeof(t_pcb*));
	pcb->pid = asignarPID();
    //pcb->program_counter = 0; Esto no se repite en los registros???? 
    pcb->quantum= QUANTUM;
    pcb->registros_cpu = malloc(sizeof(t_registros_cpu*));
	inicializar_registros(pcb);
    pcb->estado_pcb = NEW;
	return pcb;
}
//deberia poner una variable global PID=0
int asignarPID(){
	int a;
	//pthread_mutex_lock(mutex_pid); ARREGLAR SEMAFORO
	pid_global ++;
	a = pid_global;
	//pthread_mutex_unlock(mutex_pid); ARREGLAR SEMAFORO
	return a;
}

void cambiar_estado(t_pcb* un_pcb, estado_pcb* nuevo_estado){
	un_pcb->estado_pcb = nuevo_estado;
	switch (nuevo_estado)
    {
    case NEW:
        list_add(lista_new,un_pcb);

        break;
    case READY:
        list_add(lista_ready,un_pcb);
        break;
    case EXEC:
        list_add(lista_exec,un_pcb);
        break;
    case EXIT:
        list_add(lista_exit,un_pcb);
        break;

    default:
        break;
    }
    
}

void inicializar_registros(t_pcb* un_pcb){
	un_pcb->registros_cpu->AX= 0;
}


    