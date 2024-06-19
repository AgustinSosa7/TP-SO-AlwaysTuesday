#include "../includes/k_gestor.h"

t_list* INSTRUCCIONES_GEN;
t_list* INSTRUCCIONES_STDIN;
t_list* INSTRUCCIONES_STDOUT;
t_list* INSTRUCCIONES_FS;
t_list* IOS_CONECTADOS;

t_list* lista_instrucciones;

t_list* lista_new;
t_list* lista_ready;
t_list* lista_ready_plus;
t_list* lista_exec;
t_list* lista_exit;

t_list* lista_recursos;

////////////////SEMAFOROS/////////////////////////////////////////
pthread_mutex_t mutex_pid;
pthread_mutex_t mutex_new;
pthread_mutex_t mutex_ready;
pthread_mutex_t mutex_exec;
pthread_mutex_t mutex_ready_plus;
pthread_mutex_t mutex_exit;
pthread_mutex_t mutex_io;
pthread_mutex_t mutex_detener_planificacion;

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
    liberar_estructuras_en_memoria(FINALIZAR_PROCESO_MEMORIA,pcb->pid); //Esta hardcodeado el motivo, no enviarlo como parametro
    log_warning(kernel_logger,"Finaliza el proceso <%d> - Motivo: <%s> \n",pcb->pid, enum_a_string_fin_de_proceso(motivo));

    pthread_mutex_lock(&mutex_new);
    sem_post(&sem_grado_multiprogram);
    pthread_mutex_unlock(&mutex_new);
}


void detener_planificacion(){
    pthread_mutex_lock(&mutex_detener_planificacion);
    if(flag_detener_planificacion){
        sem_wait(&sem_detener_planificacion);
    }
    pthread_mutex_unlock(&mutex_detener_planificacion);
}

void liberar_recursos(t_pcb* un_pcb){
    t_list*lista_de_recursos_encontrada = buscar_lista_de_recursos_pcb(un_pcb);
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
	pcb_encontrado = list_find(lista_new, esta_el_pcb);
    if(pcb_encontrado != NULL){
       return pcb_encontrado;
    }
    pcb_encontrado = list_find(lista_ready, esta_el_pcb);
    if(pcb_encontrado != NULL){
       return pcb_encontrado;
    }
    pcb_encontrado = list_find(lista_ready_plus, esta_el_pcb);
    if(pcb_encontrado != NULL){
       return pcb_encontrado;
    }
    pcb_encontrado = list_find(lista_exec, esta_el_pcb);
    if(pcb_encontrado != NULL){
       return pcb_encontrado;   
    }
    pcb_encontrado = list_find(lista_exit, esta_el_pcb);
    if(pcb_encontrado != NULL){
       return pcb_encontrado;
    }
    pcb_encontrado = buscar_pcb_en_bloqueados(pid);
    return pcb_encontrado;

}
bool encontre_el_pcb(t_pcb* pcb, int pid){
	return (pcb->pid == pid);
}

t_pcb* buscar_pcb_en_bloqueados(int pid){
    t_pcb* pcb_encontrado = NULL;
	bool esta_el_pcb(void* pcb){
		return encontre_el_pcb(pcb, pid);
	}
    t_list_iterator* lista = list_iterator_create(lista_recursos);
    while(list_iterator_has_next(lista)){
        t_recursos* recurso = list_iterator_next(lista); 
        if(list_any_satisfy(recurso->lista_procesos_bloqueados, esta_el_pcb)){
            pcb_encontrado = list_find(recurso->lista_procesos_bloqueados, esta_el_pcb);
            
        }
        return pcb_encontrado;
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

void liberar_estructuras_en_memoria(int pid){
    t_paquete* paquete = crear_paquete(FINALIZAR_PROCESO_MEMORIA);
    agregar_int_a_paquete(paquete,pid);
    enviar_paquete(paquete,fd_memoria);
    eliminar_paquete(paquete);
}