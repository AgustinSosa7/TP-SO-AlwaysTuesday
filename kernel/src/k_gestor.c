#include "../includes/k_gestor.h"

bool iniciar_tiempo_VRR;

t_list* INSTRUCCIONES_GEN;
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
    printf("liberando recursos.\n");
    liberar_recursos(pcb);
    printf("liberando estructuras en memoria.\n");
    liberar_estructuras_en_memoria(FINALIZAR_PROCESO_MEMORIA, pcb->pid);
    log_warning(kernel_logger,"Finaliza el proceso <%d> - Motivo: <%s> \n",pcb->pid, enum_a_string_fin_de_proceso(motivo));
    free(pcb);
   // sem_post(&sem_planificador_corto_plazo);
    sem_post(&sem_grado_multiprogram);

}


void detener_planificacion(){
    pthread_mutex_lock(&mutex_detener_planificacion);
    if(flag_detener_planificacion){
        sem_wait(&sem_detener_planificacion);
    }
    pthread_mutex_unlock(&mutex_detener_planificacion);
}

void liberar_recursos(t_pcb* un_pcb){
    t_list* lista_de_recursos_encontrada = buscar_lista_de_recursos_pcb(un_pcb);
    if(lista_de_recursos_encontrada !=NULL){
        bool a = list_remove_element(lista_de_recursos_encontrada,un_pcb);
    } else {
        printf("No hay recursos para liberar.\n");
    }
}

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
    t_recursos* recurso;
    while(list_iterator_has_next(lista)){
        recurso = list_iterator_next(lista); 
        if(list_any_satisfy(recurso->lista_procesos_bloqueados, esta_el_pcb)){
            pcb_encontrado = list_find(recurso->lista_procesos_bloqueados, esta_el_pcb);
            return pcb_encontrado;
        }
        
    }

}
t_list* buscar_lista_de_recursos_pcb(t_pcb* pcb){
	bool esta_el_pcb(void* un_pcb){
		return encontre_el_pcb(un_pcb, pcb->pid);
	}
    t_list_iterator* lista = list_iterator_create(lista_recursos);
    while(list_iterator_has_next(lista)){
        t_recursos* recurso =list_iterator_next(lista); 
        if(list_any_satisfy(recurso->lista_procesos_bloqueados, esta_el_pcb)){
           return recurso->lista_procesos_bloqueados;
        } else {
            return NULL;
        }
    }
}

void liberar_estructuras_en_memoria(op_code code_op ,int pid){
    t_paquete* paquete = crear_paquete(code_op);
    agregar_int_a_paquete(paquete,pid);
    enviar_paquete(paquete,fd_memoria);
    eliminar_paquete(paquete);
}
