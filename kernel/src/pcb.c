#include "../includes/pcb.h"


t_pcb* crearPcb(){
	t_pcb* pcb = malloc(sizeof(t_pcb));
	pcb->pid = asignarPID();
    pcb->quantum = QUANTUM;
    pcb->registros_cpu = malloc(sizeof(t_registros_cpu));
	inicializar_registros(pcb);
    pcb->estado_pcb = NEW;
    pcb->contador = 0;
    pcb->tiempo_transcurrido = 0;
	return pcb;
}
//deberia poner una variable global PID=0
int asignarPID(){
	int a;
	pthread_mutex_lock(&mutex_pid);
	pid_global ++;
	a = pid_global;
	pthread_mutex_unlock(&mutex_pid);
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
    log_info(kernel_logger,"Cambio de Estado: PID: <%d> - Estado Anterior: <%s> - Estado Actual: <%s> \n",un_pcb->pid, enum_a_string(estado_anterior),enum_a_string(nuevo_estado));

	switch(nuevo_estado)
    {
    case NEW:
        pthread_mutex_lock(&(struct_new->mutex));
        list_add(struct_new->lista,un_pcb);
        pthread_mutex_unlock(&(struct_new->mutex));
        break;
    case READY:
        pthread_mutex_lock(&(struct_ready->mutex));
        list_add(struct_ready->lista,un_pcb);
        log_info(kernel_logger,"Cola Ready:\n");
        imprimir_lista_ready(struct_ready);
        pthread_mutex_unlock(&(struct_ready->mutex));
        break;
    case EXEC:
        pthread_mutex_lock(&(struct_exec->mutex));
        list_add(struct_exec->lista,un_pcb);
        pthread_mutex_unlock(&(struct_exec->mutex));
        break;
    case EXIT:
        pthread_mutex_lock(&(struct_exit->mutex));
        list_add(struct_exit->lista,un_pcb); 
        pthread_mutex_unlock(&(struct_exit->mutex));
        break;
    case READYPLUS:
        pthread_mutex_lock(&(struct_ready_plus->mutex));
        list_add(struct_ready_plus->lista,un_pcb);
        pthread_mutex_unlock(&(struct_ready_plus->mutex));
        break;    
        
    default:
        break;
    }
    
}


t_pcb* sacar_de_la_lista_vieja(estado_pcb estado_anterior){
    t_listas_estados* struct_lista = buscar_lista(estado_anterior);
    pthread_mutex_lock(&(struct_lista->mutex));
    t_pcb* un_pcb = list_remove(struct_lista->lista,0);
    pthread_mutex_unlock(&(struct_lista->mutex));
    return un_pcb;
}

t_listas_estados* buscar_lista(estado_pcb estado_anterior){
    switch (estado_anterior)
    {
    case NEW:
        return struct_new;
        break;
    case READY:
        return struct_ready;
        break;
    case READYPLUS:
        return struct_ready_plus;
        break;
    case EXEC:
        return struct_exec;
        break;
    case EXIT:
        return struct_exit;
        break;
    default:
        break;
    }
    
}

void eliminar_pcb(t_pcb* un_pcb){
    free(un_pcb->registros_cpu);
    free(un_pcb);
}

