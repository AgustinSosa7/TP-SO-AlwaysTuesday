#include "../includes/pcb.h"


t_pcb* crearPcb(){
	t_pcb* pcb = malloc(sizeof(t_pcb));
	pcb->pid = asignarPID();
    pcb->quantum = QUANTUM;
    pcb->registros_cpu = malloc(sizeof(t_registros_cpu));
	inicializar_registros(pcb);
    pcb->estado_pcb = NEW;
	return pcb;
}
//deberia poner una variable global PID=0
int asignarPID(){
	int a = 1;
	//pthread_mutex_lock(mutex_pid); ARREGLAR SEMAFORO
	pid_global ++;
	a = pid_global;
	//pthread_mutex_unlock(mutex_pid); ARREGLAR SEMAFORO
	return a;
}
void inicializar_registros(t_pcb* un_pcb){
	un_pcb->registros_cpu->PC= 0;
    un_pcb->registros_cpu->AX= 0;
    un_pcb->registros_cpu->BX= 0;
    un_pcb->registros_cpu->CX= 0;
    un_pcb->registros_cpu->DX= 0;
    un_pcb->registros_cpu->EAX= 0;
    un_pcb->registros_cpu->EBX= 0;
    un_pcb->registros_cpu->ECX= 0;
    un_pcb->registros_cpu->EDX= 0;
    un_pcb->registros_cpu->SI= 0;
    un_pcb->registros_cpu->DI= 0;
}

//solo funciona para sacar elementos de la posicion 0 y NO FUNCIONA PARA BLOCKED
void cambiar_de_estado_y_de_lista(estado_pcb estado_anterior, estado_pcb nuevo_estado){
    t_pcb* un_pcb = sacar_de_la_lista_vieja(estado_anterior);
	un_pcb->estado_pcb = nuevo_estado;
    log_warning(kernel_logger, "PID: <%d> - Estado Anterior: <%s> - Estado Actual: <%s> \n",un_pcb->pid, enum_a_string(estado_anterior),enum_a_string(nuevo_estado));

	switch(nuevo_estado)
    {
    case NEW:
        list_add(lista_new,un_pcb);
        break;
    case READY:
        list_add(lista_ready,un_pcb);
        //log info(Ingreso a Ready: “Cola Ready <COLA>: [<LISTA DE PIDS>]”)
        break;
    case EXEC:
        list_add(lista_exec,un_pcb);
        break;
    case EXIT:
        list_add(lista_exit,un_pcb); //Se deben liberar las estructuras en memoria
        break;
    case READYPLUS:
        list_add(lista_ready_plus,un_pcb);
        break;    
        
    default:
        break;
    }
    
}


t_pcb* sacar_de_la_lista_vieja(estado_pcb estado_anterior){
    t_pcb* un_pcb = list_remove(buscar_lista(estado_anterior),0);
    return un_pcb;
}

t_list* buscar_lista(estado_pcb estado_anterior){
    switch (estado_anterior)
    {
    case NEW:
        return lista_new;
        break;
    case READY:
        return lista_ready;
        break;
    case READYPLUS:
        return lista_ready_plus;
        break;
    case EXEC:
        return lista_exec;
        break;
    case EXIT:
        return lista_exit;
        break;
    default:
        break;
    }
    
}

