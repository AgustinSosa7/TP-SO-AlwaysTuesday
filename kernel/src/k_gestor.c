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

void enviar_interrupción_a_cpu(op_code tipo_interrupción){
    t_paquete* un_paquete = crear_paquete(tipo_interrupción);
    // no tiene nada en el buffer.
	enviar_paquete(un_paquete,fd_cpu_interrupt);
    eliminar_paquete(un_paquete);
} 
void eliminar_proceso(int pid, motivo_fin_de_proceso motivo){
    //solicitar a la memoria que libere todas las estructuras asociadas 
    //liberar recursos
    //enviar mensaje a lucas(Op CODE FIN DE PROCESO,pid);
    log_info(kernel_logger,"Finaliza el proceso <%d> - Motivo: <%s> \n",pid, enum_a_string_fin_de_proceso(motivo));

    pthread_mutex_lock(&mutex_new);
    if(!list_is_empty(lista_new)){
    sem_post(&sem_grado_multiprogram);
    }
    pthread_mutex_unlock(&mutex_new);
}


void detener_planificacion(){
    pthread_mutex_lock(&mutex_detener_planificacion);
    if(flag_detener_planificacion){
        sem_wait(&sem_detener_planificacion);
    }
    pthread_mutex_unlock(&mutex_detener_planificacion);
}

void liberar_recursos(int pid){
    t_pcb* pcb_encontrado = buscar_pcb(pid);
    //
}

t_pcb* buscar_pcb(int pid){
	t_pcb* pcb_encontrado;
	bool esta_el_pcb(void* pcb){
		return encontre_el_pcb(pcb, pid);
	}
	
	if(list_any_satisfy(lista_new, esta_el_pcb)){
		pcb_encontrado = list_find(lista_new, esta_el_pcb);
	} else if(list_any_satisfy(lista_ready, esta_el_pcb)){
		pcb_encontrado = list_find(lista_ready, esta_el_pcb);
	} else if(list_any_satisfy(lista_ready_plus, esta_el_pcb)){
		pcb_encontrado = list_find(lista_ready_plus, esta_el_pcb);
	//} else if(list_any_satisfy(lista_bloqued,esta_el_pcb)){
	//pcb_encontrado = list_find(lista_bloqued, esta_el_pcb);
	//} preguntar por la queue de bloqued
	}else{
		printf("no se encontró el pcb con PID : %d \n", pid);
	}
	return pcb_encontrado;

}

bool encontre_el_pcb(t_pcb* pcb, int pid){
	return (pcb->pid == pid);
}
