#include "../includes/k_gestor.h"

bool iniciar_tiempo_VRR;

t_list* INSTRUCCIONES_GENERICA;
t_list* INSTRUCCIONES_STDIN;
t_list* INSTRUCCIONES_STDOUT;
t_list* INSTRUCCIONES_FS;
t_list* IOS_CONECTADOS;

t_list* lista_instrucciones;
t_list* lista_recursos;

t_listas_estados* struct_exec;
t_listas_estados* struct_new;
t_listas_estados* struct_ready;
t_listas_estados* struct_ready_plus;
t_listas_estados* struct_exit;

////////////////SEMAFOROS/////////////////////////////////////////
pthread_mutex_t mutex_pid;
pthread_mutex_t mutex_new;
pthread_mutex_t mutex_ready;
pthread_mutex_t mutex_exec;
pthread_mutex_t mutex_ready_plus;
pthread_mutex_t mutex_exit;
pthread_mutex_t mutex_io;
pthread_mutex_t mutex_detener_planificacion;
pthread_mutex_t mutex_VRR;

sem_t sem_grado_multiprogram;
sem_t sem_new_a_ready;
sem_t sem_planificador_corto_plazo;
sem_t sem_detener_planificacion;
bool flag_detener_planificacion = false;

void enviar_interrupción_a_cpu(op_code tipo_interrupción, int motivo){
    t_paquete* un_paquete = crear_paquete(tipo_interrupción);
    agregar_int_a_paquete(un_paquete, motivo);
	enviar_paquete(un_paquete,fd_cpu_interrupt);
    eliminar_paquete(un_paquete);
} 
void eliminar_proceso(t_pcb* pcb, motivo_fin_de_proceso motivo){
    printf("liberando estructuras en memoria.\n");
    liberar_estructuras_en_memoria(FINALIZAR_PROCESO_MEMORIA, pcb->pid);
    log_warning(kernel_logger,"Finaliza el proceso <%d> - Motivo: <%s> \n",pcb->pid, enum_a_string_fin_de_proceso(motivo));
    sem_post(&sem_grado_multiprogram);
    printf("liberando recursos.\n");
    liberar_recursos(pcb);
}


void detener_planificacion(){
    pthread_mutex_lock(&mutex_detener_planificacion);
    if(flag_detener_planificacion){
        sem_wait(&sem_detener_planificacion);
    }
    pthread_mutex_unlock(&mutex_detener_planificacion);
}


/////////////FINALIZAR_PROCESO//////////////
t_pcb* buscar_pcb(int pid){
	t_pcb* pcb_encontrado;
	bool esta_el_pcb(void* pcb){
		return encontre_el_pcb(pcb, pid);
	}
    pthread_mutex_lock(&(struct_new->mutex));
	pcb_encontrado = list_find(struct_new->lista, esta_el_pcb);
    pthread_mutex_unlock(&(struct_new->mutex));
    if(pcb_encontrado != NULL){
       return pcb_encontrado;
    }
    pthread_mutex_lock(&(struct_ready->mutex));
    pcb_encontrado = list_find(struct_ready->lista, esta_el_pcb);
    pthread_mutex_unlock(&(struct_ready->mutex));
    if(pcb_encontrado != NULL){
       return pcb_encontrado;
    }
    pthread_mutex_lock(&(struct_ready_plus->mutex));
    pcb_encontrado = list_find(struct_ready_plus->lista, esta_el_pcb);
    pthread_mutex_unlock(&(struct_ready_plus->mutex));
    if(pcb_encontrado != NULL){
       return pcb_encontrado;
    }
    pthread_mutex_lock(&(struct_exec->mutex));
    pcb_encontrado = list_find(struct_exec->lista, esta_el_pcb);
    pthread_mutex_unlock(&(struct_exec->mutex));
    if(pcb_encontrado != NULL){
       return pcb_encontrado;   
    }
    pthread_mutex_lock(&(struct_exit->mutex));
    pcb_encontrado = list_find(struct_exit->lista, esta_el_pcb);
    pthread_mutex_unlock(&(struct_exit->mutex));
    if(pcb_encontrado != NULL){
       return pcb_encontrado;
    }
    pcb_encontrado = buscar_pcb_en_bloqueados(pid);
    return pcb_encontrado;

}
bool encontre_el_pcb(t_pcb* pcb, int pid){
    int pid_buscado= pcb->pid;
	return (pid_buscado == pid);
}

t_pcb* buscar_pcb_en_bloqueados(int pid){
    t_pcb* pcb_encontrado;
	bool esta_el_pcb(void* pcb){
		return encontre_el_pcb(pcb, pid);
	}
    t_list_iterator* lista = list_iterator_create(lista_recursos);
    while(list_iterator_has_next(lista)){
        t_recursos* recurso = list_iterator_next(lista); 
        if(list_any_satisfy(recurso->lista_procesos_bloqueados, esta_el_pcb)){
            pcb_encontrado = list_find(recurso->lista_procesos_bloqueados, esta_el_pcb);
            return pcb_encontrado;
        }   
    }
    list_iterator_destroy(lista);
}

/////liberación de recursos
void liberar_recursos(t_pcb* un_pcb){
    remover_pcb_de_recursos_bloqueados(un_pcb);
    remover_pcb_de_recursos_asignados(un_pcb);
}
void remover_pcb_de_recursos_bloqueados(t_pcb* pcb){
    t_list_iterator* lista = list_iterator_create(lista_recursos);
    bool remover(void* un_pcb){
        return remover_pcb(un_pcb, pcb);
    }
    while(list_iterator_has_next(lista)){
        t_recursos* recurso =list_iterator_next(lista); 
        pthread_mutex_lock(&(recurso->mutex_recurso));
        list_remove_by_condition(recurso->lista_procesos_bloqueados,remover); 
        pthread_mutex_unlock(&(recurso->mutex_recurso));
    }
    list_iterator_destroy(lista);
}

bool remover_pcb(t_pcb* un_pcb, t_pcb* otro_pcb){
    return (un_pcb->pid==otro_pcb->pid);
}
void remover_pcb_de_recursos_asignados(t_pcb* pcb){
    t_list_iterator* lista = list_iterator_create(lista_recursos);
        bool removerr(void* un_pcb){
        return remover_pcb(un_pcb, pcb);
    }
    while(list_iterator_has_next(lista)){
        t_recursos* recurso = list_iterator_next(lista); 
        pthread_mutex_lock(&(recurso->mutex_recurso));
        if(list_any_satisfy(recurso->lista_procesos_asignados,removerr)){
        list_remove_by_condition(recurso->lista_procesos_asignados,removerr); 
        log_warning(kernel_logger,"Se liberó el proceso <%d> de la lista de recursos %s\n",pcb->pid,recurso->nombre_recurso);
            recurso->instancias = recurso->instancias +1;
            if(!list_is_empty(recurso->lista_procesos_bloqueados)){
                t_pcb* un_pcb = list_remove(recurso->lista_procesos_bloqueados,0);
                list_add(recurso->lista_procesos_asignados,un_pcb);
                enviar_proceso_blocked_a_ready(un_pcb);
                sem_post(&sem_planificador_corto_plazo);
            }
        }
        pthread_mutex_unlock(&(recurso->mutex_recurso));
    }
    list_iterator_destroy(lista);
}

void liberar_estructuras_en_memoria(op_code code_op ,int pid){
    t_paquete* paquete = crear_paquete(code_op);
    agregar_int_a_paquete(paquete,pid);
    enviar_paquete(paquete,fd_memoria);
    eliminar_paquete(paquete);
}

void enviar_proceso_blocked_a_ready(t_pcb* un_pcb){
      un_pcb->estado_pcb = READY;

      pthread_mutex_lock(&(struct_ready->mutex));
      list_add(struct_ready->lista,un_pcb); 
      pthread_mutex_unlock(&(struct_ready->mutex));
}
